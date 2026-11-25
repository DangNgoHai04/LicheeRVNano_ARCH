# Hướng Dẫn Đấu Nối, Nạp Firmware, Bring-up và Nhận Dữ Liệu Camera Nhiệt Tiny1C Trên Board LicheeRV-Nano (SG2002)

Để đọc luồng mã nguồn trước khi bring-up, xem [lộ trình T3 → App](LO_TRINH_DOC_T3_APP.md). Kết quả chạy module thật ghi tại [nhật ký 18/11](Implementation/09_board_bringup_2026-11-18.md).

Tài liệu này hướng dẫn từng bước từ **đấu nối phần cứng**, **biên dịch & nạp `fip.bin` cho nhân RTOS C906**, **khởi chạy cầu nối trên Linux của board**, cho tới **nhận và xem dữ liệu trực tiếp từ cảm biến Tiny1C trên máy tính**.

---

## Bước 1: Đấu nối phần cứng (Tiny1C $\leftrightarrow$ LicheeRV-Nano)

Driver Tầng 0–T3 được thiết kế cho cấu hình kết nối giữa module **InfiRay Tiny1C (ASIC-2121W)** và **LicheeRV-Nano (SG2002)** như sau:

| Nhóm tín hiệu | Chân trên Tiny1C | Chân trên SG2002 / LicheeRV-Nano | Cấu hình trong Driver T0–T3 |
| :--- | :--- | :--- | :--- |
| **Kênh điều khiển I2C (Soft-I2C Bus 5)** | `I2C_SCL` | **`GPIOA15`** | Cấu hình mặc định `100 kHz`, lần bring-up 18/11 dùng `20 kHz`, hỗ trợ clock stretching tới `20 ms`. Địa chỉ Slave: `0x78` (8-bit) / `0x3C` (7-bit). Cần pull-up cho SCL/SDA về đúng miền điện áp I/O của cả board và module; xác nhận VDDIO thực tế trước khi nối, không mặc định kéo lên 3.3 V. |
| | `I2C_SDA` | **`GPIOA27`** | |
| **Kênh dữ liệu ảnh/nhiệt VoSPI (SPI0 qua MIPI DSI D-PHY Bypass)** | `SPI_SCK` | **FPC2 chân 15**, `GPIOC16` (`PAD_MIPI_TXM2`) | SPI0 Master, **SPI Mode 3 (`CPOL=1, CPHA=1`)**, `rx_sample_dly = 1`. Firmware đang chạy đặt SCK `11.71875 MHz` (BAUDR=16) cho cả chế độ đơn và kép. |
| | `SPI_MISO` *(Dữ liệu từ Cam về Board)* | **FPC2 chân 17**, `GPIOC15` (`PAD_MIPI_TXP1`) | Chân nhận của SG2002 (`SPI0_SDI`), module Tiny1C phải lái tín hiệu này khi được chọn. |
| | `SPI_MOSI` *(Lệnh 0xAA/0xCC + 0x55 từ Board sang Cam)* | **FPC2 chân 18**, `GPIOC14` (`PAD_MIPI_TXM1`) | Chân xuất của SG2002 (`SPI0_SDO`). |
| | `SPI_CS` *(Chip Select)* | **FPC2 chân 14**, `GPIOC17` (`PAD_MIPI_TXP2`) | Chân pad có chức năng `SPI0_CS_X`, firmware hiện mux sang GPIOC17 để điều khiển CS bằng phần mềm; giữ LOW suốt gói Header 512B hoặc gói Payload 96KB. |
| **Nguồn & Mass** | `VDD` / `GND` | FPC2 chân **26/29** là `VDD1V8_SYS`; chân **9/10/13/16/19/22/25/28** là GND | Xác nhận nguồn và VDDIO đúng theo datasheet module, dùng GND chung. Không đưa tín hiệu 3,3 V trực tiếp vào các chân SPI 1,8 V của FPC2. |

**Xác định đầu chân 1 trước khi đấu dây:** FPC2 dùng đầu nối HDGC
`0.3K-QX-31PWB` (31 tiếp điểm, bottom contact). Trong ảnh đầu nối nhìn trực
diện với ký hiệu `A` ở bên trái và `31` ở bên phải, **chân 1 ở đầu A**, rồi
đếm dần sang phải đến chân 31. Footprint PCB cũng đặt pad tín hiệu `1` bên
trái và `31` bên phải theo góc nhìn tương ứng. Hai pad hông `32/33` là chân
giữ cơ khí, không phải tiếp điểm số 32/33 của cáp. Không suy hướng trái/phải
từ ảnh 3D đã xoay hoặc từ mặt lưng cáp. Xác nhận bằng đo thông mạch khi
**tắt nguồn**: từ pad PCB số 15 đến dây SCK đang dùng, pad 14 đến CS, pad 17
đến MISO, pad 18 đến MOSI. Không dùng pad hông `32/33` làm chân nguồn 3,3 V; sơ đồ board ghi hai
pad hông nối GND.

---

## Bước 2: Biên dịch & Đóng gói toàn bộ các tệp trên Máy tính (Host PC)

Tại thư mục gốc của workspace (`/home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test`), chạy tuần tự 4 lệnh sau:

```bash
# 1. Build cấu hình I²C 20 kHz đã dùng trong lần bring-up ngày 18/11
THERMAL_I2C_FREQ_HZ=20000 ./Doc_md/Tiny-1C/App/scripts/build_firmware.sh

# 2. Biên dịch chương trình cầu nối thermal_bridge (bản tĩnh cho Linux RISC-V trên board)
make -C Doc_md/Tiny-1C/App all linux test

# 3. Biên dịch module kernel Mailbox đã vá lỗi timeout (/dev/cvi-rtos-cmdqu)
./Doc_md/Tiny-1C/App/scripts/build_mailbox.sh

# 4. Đóng gói cvirtos.bin mới vào file fip.bin của board (tạo ra fip-thermal.bin)
# Lưu ý: Nếu board của bạn đang chạy một file fip.bin riêng trên thẻ nhớ, hãy truyền đường dẫn
# file fip.bin đang chạy tốt của board vào tham số --base để giữ nguyên DDR_PARAM/BL2/U-Boot!
python3 Doc_md/Tiny-1C/App/scripts/package_fip.py \
    --base install/soc_sg2002_licheervnano_sd/fip_spl.bin \
    --rtos Doc_md/Tiny-1C/App/build/cvirtos.bin \
    --output Doc_md/Tiny-1C/App/build/fip-thermal.bin
```

Sau khi chạy xong, toàn bộ các tệp cần nạp nằm tại [`Doc_md/Tiny-1C/App/build/`](App/build):
1. [`fip-thermal.bin`](App/build/fip-thermal.bin): Chứa Bootloader + **RTOS C906 mới (`cvirtos.bin`)** có `thermal_service_task`.
2. [`thermal_bridge`](App/build/thermal_bridge): Binary tĩnh chạy trên Linux của board.
3. [`soph_rtos_cmdqu.ko`](App/build/soph_rtos_cmdqu.ko): Kernel module Mailbox (nếu board cần nạp lại module).

---

## Bước 3: Nạp `fip-thermal.bin` (và `boot.sd` nếu cập nhật DTS) lên Board LicheeRV-Nano

Giả sử địa chỉ IP của board LicheeRV-Nano là `<BOARD_IP>` — **ưu tiên dùng IP USB Ethernet** (ví dụ **`10.250.50.1`**, ping `< 1 ms`, tìm bằng `ip -br a` / `ip route | grep enx` trên PC hoặc `ip -br a` trên UART board) vì nhanh và không bị rớt như Wi-Fi.

> **Lưu ý khi nạp lại Linux Kernel / Device Tree (`boot.sd`):**
> Sau khi chạy `build_kernel`, **không** chép file `install/soc_sg2002_licheervnano_sd/boot.sd` (bị `raw2cimg.py` bọc 128 byte `CIMG` khiến U-Boot báo `Wrong Image Format for bootm command`). Hãy chép file FIT Image thuần **`install/soc_sg2002_licheervnano_sd/rawimages/boot.sd`** (bản sao `boot.itb`) thành `/boot/boot.sd` trên thẻ nhớ. Sau khi boot, kiểm tra `cat /proc/device-tree/*spi*4180000*/status` ra `disabled` là Linux đã nhả hẳn `spi0`.

### Cách A: Nạp qua mạng (`scp` / `ssh`) ngay khi board đang bật

```bash
# 1. Chép fip-thermal.bin, thermal_bridge và soph_rtos_cmdqu.ko sang /root/ trên board
scp Doc_md/Tiny-1C/App/build/fip-thermal.bin \
    Doc_md/Tiny-1C/App/build/thermal_bridge \
    Doc_md/Tiny-1C/App/build/soph_rtos_cmdqu.ko \
    root@<BOARD_IP>:/root/
```

Sau đó đăng nhập `ssh root@<BOARD_IP>` vào board để **sao lưu `fip.bin` cũ** và cập nhật `fip.bin` mới vào phân vùng boot của thẻ nhớ (`/boot` hoặc `/dev/mmcblk0p1`):

```bash
# Chạy trên Board LicheeRV-Nano:
mount | grep mmcblk0p1 || mount /dev/mmcblk0p1 /boot

# BẮT BUỘC: Sao lưu file fip.bin gốc đang chạy tốt để có thể khôi phục bất cứ lúc nào
cp /boot/fip.bin /boot/fip.bin.bak

# Ghi fip-thermal.bin thành /boot/fip.bin và đồng bộ xuống thẻ nhớ
cp /root/fip-thermal.bin /boot/fip.bin
sync

# Khởi động lại board để Bootloader nạp nhân RTOS C906 mới vào 0x8fe00000
reboot
```

*(**Cách B**: Bạn cũng có thể rút thẻ nhớ MicroSD cắm vào máy tính, đổi tên `fip.bin` trên phân vùng FAT32 đầu tiên thành `fip.bin.bak`, rồi chép `Doc_md/Tiny-1C/App/build/fip-thermal.bin` vào phân vùng đó với tên `fip.bin`, sau đó cắm lại thẻ vào board và cấp nguồn).*

---

## Bước 4: Bring-up Camera nhiệt trên Board LicheeRV-Nano

Sau khi board khởi động lại với `fip.bin` mới, đăng nhập `ssh root@<BOARD_IP>` và thực hiện:

### 4.1. Kiểm tra điều kiện phần cứng & nạp module Mailbox

```bash
# 1. Kiểm tra vùng nhớ RTOS (0x8fe00000..0x8fffffff) không bị Linux chiếm dụng
grep -E '8fe00000|8ff' /proc/iomem || echo "OK: Vung nho RTOS 0x8fe00000 duoc bao toan"

# 2. Kiểm tra thiết bị Mailbox /dev/cvi-rtos-cmdqu đã tồn tại chưa
ls -l /dev/cvi-rtos-cmdqu
# Nếu chưa có (hoặc muốn dùng bản soph_rtos_cmdqu.ko mới biên dịch cùng kernel):
# rmmod soph_rtos_cmdqu 2>/dev/null || true
# insmod /root/soph_rtos_cmdqu.ko
```

> **Lưu ý an toàn phần cứng trước khi chạy**:
> - Đảm bảo Linux không có dịch vụ nào đang điều khiển các chân thuộc bank `GPIOA` (`GPIOA15/GPIOA27`) hoặc đang xuất hình ra màn hình `MIPI DSI` (vì SPI0 dùng đường Digital Bypass qua các chân `PAD_MIPI_TX*` là `GPIOC16/17/14/15`).
> - Khi board đã dành riêng kênh DMA 0/1 và tài nguyên DMAC/remap/IRQ liên lõi cho SPI0, `--dma-reserved` cho phép chọn AUTO lúc khởi tạo; nếu không truyền `--dma-reserved`, SPI0 sẽ chạy Polling FIFO mặc định.

### 4.2. Khởi chạy `thermal_bridge` trên Board

```bash
chmod +x /root/thermal_bridge

# Nếu đã có tiến trình thermal_bridge chạy nền trước đó (tránh lỗi camera owner lock):
killall thermal_bridge 2>/dev/null || true

# Chạy ở chế độ Polling SPI0 an toàn (lắng nghe trên mọi IP của board tại cổng 9000):
/root/thermal_bridge --resources-owned --bind 0.0.0.0 --port 9000

# Hoặc nếu đã dành riêng DMA cho SPI0 trên RTOS:
# /root/thermal_bridge --resources-owned --dma-reserved --bind 0.0.0.0 --port 9000
```

Khi khởi chạy thành công, trên terminal của board sẽ in ra:
```text
Tiny1C bridge 0.0.0.0:9000 [HARDWARE]
```

---

## Bước 5: Nhận dữ liệu, Kiểm tra Bring-up và Xem hình ảnh/nhiệt độ trên PC

Từ máy tính (Host PC) kết nối cùng mạng với board (`<BOARD_IP>`), bạn có **3 cách** để kích hoạt bring-up cảm biến và xem dữ liệu truyền về:

### Cách 1: Kiểm tra Bring-up & Xem giải mã chi tiết từng gói tin + Bản đồ nhiệt trên Terminal (`--cli`)

Chạy lệnh sau trên PC để gửi lệnh khởi tạo Camera (`THERMAL_START`), thu khung hình (`THERMAL_CAPTURE`) ở cả 3 chế độ (`MODE_TEMP`, `MODE_IMAGE`, `MODE_DUAL`), in bảng nhiệt độ °C và lưu file ảnh/raw ra thư mục `captured_frames`:

```bash
# Chỉ kiểm tra riêng chế độ Nhiệt độ (MODE_TEMP - 1 khung hình):
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py <BOARD_IP> --port 9000 \
    --cli --mode temp --frames 1 --dump-dir ./captured_frames

# Hoặc kiểm tra tuần tự cả 3 chế độ (Nhiệt Y16 + Ảnh YUYV + Kép Dual):
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py <BOARD_IP> --port 9000 \
    --cli --mode all --frames 1 --dump-dir ./captured_frames
```

**Cách đọc kết quả Bring-up trên màn hình Terminal**:
- Khi kết nối thành công với cảm biến thật, dòng trạng thái trong báo cáo sẽ hiển thị:
  - `status / seq : 0 / 1` (`status = 0` tức là `IR_SUCCESS`: nhân RTOS đã khởi tạo thành công I2C5 tại `0x3C`, bật `preview_start` và thu được khung VoSPI qua SPI0 với bit `frame_valid == 1`).
  - `flags : 0x08 (HIGH_GAIN)` khi metadata báo high gain. Đối chiếu log bridge `[HARDWARE]` và cờ `SIMULATED` để phân biệt với giả lập.
  - Bảng nhiệt độ hiển thị rõ `Min °C`, `Max °C`, `Tâm (128,96) °C` kèm bản đồ nhiệt trực quan ngay trên Terminal và lưu các file `.bin`, `.json`, `.ppm`, `.bmp` trong `./captured_frames/`.

### Cách 2: Xem Video Nhiệt trực tiếp trên Trình duyệt Web (`--web 8080`)

Nếu muốn xem luồng video nhiệt liên tục và bấm chuyển chế độ / hiệu chuẩn màn trập (`FFC`) trực tiếp mà không cần cài `tkinter`:

```bash
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py <BOARD_IP> --port 9000 --fps 8 --web 8080
```
Mở trình duyệt web trên PC tại địa chỉ **`http://127.0.0.1:8080`**:
- Xem trực tiếp luồng ảnh nhiệt / ảnh YUYV / ảnh kép `256×384`.
- Bấm các nút **Chế độ Nhiệt (Y16)**, **Chế độ Ảnh (YUYV)**, **Chế độ Kép (Ảnh + Nhiệt)**, **Hiệu chuẩn Màn trập (FFC)**.
- Theo dõi trực tiếp các trường `seq`, `index_image`, `index_temp`, `vtemp`, `min_c`, `max_c`, `center_c` cập nhật theo thời gian thực.

### Cách 3: Xem trên cửa sổ ứng dụng Desktop Tkinter (nếu PC đã cài `python3-tk`)

```bash
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py <BOARD_IP> --port 9000 --fps 8
```

---

## Bước 6: Bảng chẩn đoán nhanh mã lỗi khi Bring-up trên phần cứng thật

Nếu cảm biến chưa lên hình ngay lần đầu, nhìn vào mã `status` trả về từ `thermal_viewer.py` để biết chính xác nguyên nhân ở tầng nào:

| Mã `status` / Lỗi | Tên mã lỗi | Nguyên nhân vật lý & Cách khắc phục |
| :---: | :--- | :--- |
| **`0`** | `IR_SUCCESS` | Thao tác thành công. Với CAPTURE, header và payload đã qua kiểm tra của driver. |
| **`-5`** | `IR_NOT_GET_DEVICE_INFO` | Nhân RTOS không nhận được ACK/phản hồi I2C từ chip Tiny1C tại địa chỉ `0x3C` trên `GPIOA15` (`SCL`) / `GPIOA27` (`SDA`). Kiểm tra nguồn camera, dây, pull-up về đúng VDDIO và GND chung. |
| **`-16`** | `IR_CHECK_DONE_FAIL` | Thanh ghi trạng thái `0x0200` của ASIC báo lỗi (ví dụ `0x16 PARAM_ERR` hoặc `0x0a UNKNOWN_CMD_ERR` khi module kẹt ở ROM bootloader `10/08/19`) hoặc timeout. Thử ngắt/cấp lại nguồn riêng cho module Tiny1C. |
| **`-11`** | `IR_START_STREAMING_FAIL` | Khởi tạo SPI0 thất bại hoặc gọi `CAPTURE` khi chưa `START`. |
| **`-2`** *(khi `CAPTURE`)* | `vospi header_err` | Lỗi truyền nhận hoặc header toàn `0xFF` trên SPI0 (FPC2 chân `15/14/18/17`, thường do hở mạch/nhầm chân `MISO` hoặc lệch mức áp `1.8V`). |
| **`-4`** *(khi `CAPTURE`)* | `vospi frame_valid == 0` | Header được chọn có bit 0 của `frame_valid` bằng 0. Đọc mẫu RX, kiểm tra offset, timing/camera và trạng thái MISO. |
| **`-3`** *(khi `CAPTURE`)* | `vospi payload_err` | Đọc gói Header 512B thành công nhưng lỗi/timeout hoặc toàn `0xFF` khi đọc gói Payload 96KB. Kiểm tra chất lượng dây `SPI_SCK` và `GND` chung. |
| **`camera owner lock`** | `EAGAIN` trên board | Đã có một tiến trình `thermal_bridge` khác chạy nền. Chạy `killall thermal_bridge` rồi khởi động lại với `--bind 0.0.0.0`. |
| **`Connection refused` / `No route to host`** | Lỗi mạng PC → Board | Kiểm tra `thermal_bridge` có đang dùng `--bind 0.0.0.0` không và ưu tiên chuyển từ Wi-Fi sang IP USB Ethernet (ví dụ `10.250.50.1`). |
