/*
 * Cầu nối dữ liệu Camera nhiệt trên Linux (thermal_bridge.c).
 *
 * Nhiệm vụ:
 *   1. Mở khóa độc quyền `/run/thermal-camera.lock` để đảm bảo chỉ có duy nhất 1 tiến trình
 *      Linux giao tiếp với tác vụ RTOS `thermal_service_task`.
 *   2. Gửi lệnh đồng bộ `SEND_WAIT` (`_IOW('r', 4, unsigned long)`) qua `/dev/cvi-rtos-cmdqu`
 *      tới `IP_CAMERA` (7) trên nhân RTOS C906 và nhận lại địa chỉ vật lý của bộ đệm `reply`.
 *   3. Ánh xạ vùng nhớ DDR dành riêng của RTOS (`0x8fe00000`, kích thước `0x200000`) qua
 *      `/dev/mem` (chế độ chỉ đọc `PROT_READ`, `O_SYNC`), kiểm tra tính hợp lệ của Header
 *      64-byte (`THM1`) và sao chép toàn bộ khung hình sang bộ đệm cục bộ trước khi phát
 *      qua kết nối TCP tới trình xem (`thermal_viewer.py`).
 *   4. Hỗ trợ chế độ giả lập `--simulate` chạy trực tiếp trên máy tính Host (không cần board)
 *      với dữ liệu ảnh YUYV422 và dữ liệu nhiệt độ Y16 chuẩn xác để kiểm thử toàn tuyến.
 */
#define _POSIX_C_SOURCE 200809L
#include "thermal_protocol.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

/* Cấu trúc 8-byte tương thích ABI với cmdqu_t trong osdrv/interdrv/rtos_cmdqu/rtos_cmdqu.h */
typedef struct {
    uint8_t ip, cmd_block;
    uint16_t resv;
    uint32_t param;
} mailbox_t;
_Static_assert(sizeof(mailbox_t) == 8, "mailbox ABI");

#define SEND_WAIT _IOW('r', 4, unsigned long)

static struct { thermal_header_t h; uint8_t data[THERMAL_MAX_PAYLOAD]; } packet;
static int dev = -1, mem = -1, sim = 0, owned = 0, dma = 0;
static uint32_t base = 0x8fe00000U, span = 0x200000U, seq;
static uint8_t *mapped;
static unsigned mode = 1;
static int started;

static uint32_t millis(void) {
    struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint32_t)((uint64_t)t.tv_sec * 1000 + t.tv_nsec / 1000000);
}

/* Kiểm tra nghiêm ngặt định dạng ABI của Header 64-byte trước khi đọc Payload */
static int valid_header(const thermal_header_t *h) {
    if (h->magic != THERMAL_MAGIC || h->version != 1 || h->header_bytes != 64 ||
        h->mode > 2 || h->width != 256 || h->height != (h->mode == 2 ? 384U : 192U)) return 0;
    if (h->payload_bytes && (h->status != 0 ||
        h->payload_bytes != (h->mode == 2 ? 196608U : 98304U))) return 0;
    return 1;
}

/*
 * Tạo khung hình giả lập cho chế độ `--simulate`:
 *   - Khung ảnh YUYV422 (98,304 byte): Dải gradient độ chói Y chuyển động theo seq, U=V=128.
 *   - Khung nhiệt độ Y16 (98,304 byte): Nền nhiệt gradient 22.0°C..25.0°C theo chiều dọc
 *     kèm vùng tâm nóng hình tròn 38.0°C di chuyển ngang theo seq, mã hóa đúng công thức
 *     raw_u16 = (Celsius + 273.15) * 64 ở định dạng Little-Endian.
 */
static void fake_frame(void) {
    unsigned image = mode != 1;
    if (image) for (unsigned i = 0; i < 98304; i += 4) {
        packet.data[i] = packet.data[i+2] = (uint8_t)(32 + ((i/4 + seq) % 180));
        packet.data[i+1] = packet.data[i+3] = 128;
    }
    if (mode != 0) {
        uint8_t *out = packet.data + (mode == 2 ? 98304 : 0);
        for (unsigned y = 0; y < 192; ++y) for (unsigned x = 0; x < 256; ++x) {
            int dx = (int)x - (int)(64 + seq % 128), dy = (int)y - 96;
            double c = dx*dx + dy*dy < 900 ? 38.0 : 22.0 + y / 64.0;
            uint16_t raw = (uint16_t)((c + 273.15) * 64);
            unsigned n = 2*(y*256+x); out[n] = raw & 255; out[n+1] = raw >> 8;
        }
    }
}

/*
 * Thực hiện giao dịch tuần tự với RTOS (hoặc bộ giả lập):
 * Yêu cầu tiếp theo chỉ được gửi sau khi đã sao chép xong toàn bộ dữ liệu phản hồi
 * của yêu cầu trước đó về bộ đệm cục bộ `packet`.
 */
static int transact(unsigned cmd, uint32_t param) {
    if (sim) {
        memset(&packet.h, 0, sizeof(packet.h));
        packet.h.magic = THERMAL_MAGIC; packet.h.version = 1; packet.h.header_bytes = 64;
        packet.h.flags = THERMAL_FLAG_SIMULATED | THERMAL_FLAG_HIGH_GAIN;
        if (cmd == THERMAL_START) { mode = param & 255; started = 1; }
        if (cmd == THERMAL_STOP) started = 0;
        if (cmd == THERMAL_CAPTURE) {
            if (!started) packet.h.status = -11;
            else { ++seq; fake_frame(); packet.h.payload_bytes = mode == 2 ? 196608 : 98304; }
        }
        packet.h.seq = seq; packet.h.mode = mode; packet.h.width = 256;
        packet.h.height = mode == 2 ? 384 : 192; packet.h.timestamp_ms = millis();
        packet.h.index_image = packet.h.index_temp = seq & 65535;
        packet.h.vtemp = 2960;
        return 0;
    }
    mailbox_t q = {THERMAL_IP, (uint8_t)(cmd | 0x80), 30000, param};
    if (ioctl(dev, SEND_WAIT, &q) < 0) { perror("mailbox SEND_WAIT (no automatic retry)"); return -1; }
    if (q.ip != THERMAL_IP || (q.cmd_block & 127) != cmd ||
        q.param < base || (uint64_t)q.param + 64 > (uint64_t)base + span || (q.param & 63)) {
        fprintf(stderr, "Invalid RTOS reply address/ABI: 0x%08x\n", q.param); return -1;
    }
    uint32_t offset = q.param - base;
    __sync_synchronize();
    memcpy(&packet.h, mapped + offset, 64);
    if (!valid_header(&packet.h) || (uint64_t)offset + 64 + packet.h.payload_bytes > span) {
        fprintf(stderr, "Invalid thermal header or memory bounds\n"); return -1;
    }
    memcpy(packet.data, mapped + offset + 64, packet.h.payload_bytes);
    return 0;
}

static int send_all(int fd, const void *buf, size_t n) {
    const uint8_t *p = buf;
    while (n) {
        ssize_t k = send(fd, p, n, MSG_NOSIGNAL);
        if (k < 0 && errno == EINTR) continue;
        if (k <= 0) return -1;
        p += k; n -= (size_t)k;
    }
    return 0;
}

static void usage(const char *p) {
    fprintf(stderr, "Usage: %s [--simulate | --resources-owned [--dma-reserved]] "
        "[--bind IPv4] [--port 9000] [--rtos-base 0x8fe00000] [--rtos-size 0x200000]\n", p);
}

static uint32_t number(const char *s) {
    char *end; errno = 0; unsigned long v = strtoul(s, &end, 0);
    if (errno || *end || !*s || v > UINT32_MAX) { fprintf(stderr,"Bad number: %s\n",s); exit(2); }
    return (uint32_t)v;
}

int main(int argc, char **argv) {
    const char *bind_ip = "127.0.0.1"; unsigned port = 9000;
    for (int i=1; i<argc; ++i) {
        if (!strcmp(argv[i],"--simulate")) sim = 1;
        else if (!strcmp(argv[i],"--resources-owned")) owned = 1;
        else if (!strcmp(argv[i],"--dma-reserved")) dma = 1;
        else if (!strcmp(argv[i],"--bind") && i+1<argc) bind_ip = argv[++i];
        else if (!strcmp(argv[i],"--port") && i+1<argc) port = number(argv[++i]);
        else if (!strcmp(argv[i],"--rtos-base") && i+1<argc) base = number(argv[++i]);
        else if (!strcmp(argv[i],"--rtos-size") && i+1<argc) span = number(argv[++i]);
        else { usage(argv[0]); return 2; }
    }
    if (!port || port > 65535 || (!sim && !owned) || !span || (base & 4095)) { usage(argv[0]); return 2; }
    int lock = -1;
    if (!sim) {
        lock = open("/run/thermal-camera.lock", O_CREAT|O_RDWR, 0600);
        if (lock < 0 || flock(lock, LOCK_EX|LOCK_NB)) { perror("camera owner lock"); return 1; }
        dev = open("/dev/cvi-rtos-cmdqu", O_RDWR);
        mem = open("/dev/mem", O_RDONLY|O_SYNC);
        if (dev < 0 || mem < 0) { perror("open camera devices"); return 1; }
        mapped = mmap(NULL, span, PROT_READ, MAP_SHARED, mem, (off_t)base);
        if (mapped == MAP_FAILED) { perror("map reserved RTOS DDR"); return 1; }
    }
    signal(SIGPIPE, SIG_IGN);
    int server = socket(AF_INET, SOCK_STREAM, 0), one = 1;
    if (server < 0) { perror("socket"); return 1; }
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    struct sockaddr_in addr = {.sin_family=AF_INET, .sin_port=htons((uint16_t)port)};
    if (inet_pton(AF_INET,bind_ip,&addr.sin_addr)!=1 || bind(server,(void*)&addr,sizeof(addr)) || listen(server,1)) {
        perror("listen"); return 1;
    }
    printf("Tiny1C bridge %s:%u [%s]\n",bind_ip,port,sim?"SIMULATED":"HARDWARE"); fflush(stdout);
    for (;;) {
        int client = accept(server,NULL,NULL);
        if (client < 0) { if(errno==EINTR) continue; perror("accept"); return 1; }
        setsockopt(client, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
        struct timeval timeout = {35,0};
        setsockopt(client,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
        setsockopt(client,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));
        for (;;) {
            /*
             * Nhận lệnh 1-byte từ Client TCP:
             *   't' -> THERMAL_START ở MODE_TEMP  (1)
             *   'i' -> THERMAL_START ở MODE_IMAGE (0)
             *   'd' -> THERMAL_START ở MODE_DUAL  (2)
             *   'c' -> THERMAL_CAPTURE (thu 1 khung hình)
             *   'f' -> THERMAL_FFC (hiệu chuẩn màn trập)
             *   's' -> THERMAL_STOP (dừng luồng)
             *   '?' -> THERMAL_INFO (truy vấn trạng thái)
             */
            char op; ssize_t n = recv(client,&op,1,0);
            if (n < 0 && errno==EINTR) continue;
            if (n != 1) break;
            unsigned cmd; uint32_t param = 0;
            switch(op) {
            case 't': case 'i': case 'd':
                cmd=THERMAL_START; param=(op=='i'?0U:op=='t'?1U:2U)|THERMAL_GPIO_OWNED|(dma?THERMAL_DMA_OWNED:0); break;
            case 'c': cmd=THERMAL_CAPTURE; break;
            case 'f': cmd=THERMAL_FFC; break;
            case 's': cmd=THERMAL_STOP; break;
            case '?': cmd=THERMAL_INFO; break;
            default: goto disconnected;
            }
            if (transact(cmd,param)) break; /* Khi lỗi/timeout: đóng kết nối client hiện tại nhưng giữ server sống */
            if (send_all(client,&packet,64 + packet.h.payload_bytes)) break;
        }
    disconnected:
        close(client);
        (void)transact(THERMAL_STOP,0);
    }
}
