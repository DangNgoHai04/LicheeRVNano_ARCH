# BÁO CÁO TOÀN DIỆN VÀ HƯỚNG DẪN ĐỌC SOURCE DRIVER SPI (TẦNG 0 – TẦNG 2)
## Dành cho Phát triển Sản phẩm Thương mại (Product Development) trên SOPHGO SG2002 / FreeRTOS

> **Ngày cập nhật:** 24/10/2026  
> **Trạng thái:** **BOARD QUALIFIED — ĐÃ NGHIỆM THU THÀNH CÔNG 100% TRÊN PHẦN CỨNG THỰC TẾ**  
> **Phạm vi kiểm chứng:** SG2002 FreeRTOS (Core 1) $\longleftrightarrow$ STM32F103 Slave (Mode 3, 500 kHz, > 600 packets, 0 bit error, 100% PASS).

---

## 1. TỔNG QUAN KIẾN TRÚC PHÂN TẦNG (LAYERS ARCHITECTURE)

Hệ thống driver SPI trên Core 1 (FreeRTOS RISC-V C906) được thiết kế theo mô hình 3 tầng phân lập chặt chẽ, tuân thủ nguyên tắc *Separation of Concerns* của chuẩn phần mềm công nghiệp:

```
+-----------------------------------------------------------------------------------+
|  TẦNG ỨNG DỤNG / SẢN PHẨM (Product Layer / Sensor Drivers / VoSPI Tiny1C)         |
|  - Task điều khiển camera Tiny1C, Task kiểm thử STM32 (spi_test_stm32.c)         |
|  - Vùng nhớ chia sẻ Telemetry DDR (0x8FF20000) giao tiếp Zero-Copy với Linux       |
+-----------------------------------------------------------------------------------+
                                        | (Gọi Public API qua spi.h)
                                        v
+-----------------------------------------------------------------------------------+
|  TẦNG 2: CORE DRIVER / BUS & DEVICE MANAGER (driver/spi/src/spi.c)               |
|  - Quản lý vòng đời Bus (spi_bus_t) và Thiết bị ngoại vi (spi_device_t)           |
|  - Đồng bộ đa luồng bằng Static Mutex, chống Race Condition                       |
|  - Ghép chuỗi giao dịch (spi_message_t) gồm nhiều đoạn (spi_segment_t)             |
|  - Quản lý Deadline/Timeout toàn cục, kiểm soát thời gian trễ CS setup/hold       |
|  - Quản lý máy trạng thái lỗi: Tách biệt transfer_status và cleanup_status        |
+-----------------------------------------------------------------------------------+
                                        | (Gọi HAL API qua hal_dw_spi.h)
                                        v
+-----------------------------------------------------------------------------------+
|  TẦNG 1: CONTROLLER HAL (hal/cv181x/spi/src/hal_dw_spi.c)                         |
|  - Driver độc lập phần cứng điều khiển ngoại vi Synopsys DesignWare APB SSI        |
|  - Cấu hình thanh ghi: CTRLR0 (Mode 0-3, DFS 4-16 bit), BAUDR, SSIENR, SER, SR    |
|  - Thuật toán Polling chống treo: Giữ bất biến FIFO (sent - got <= fifo_depth)    |
|  - Tự động dò độ sâu phần cứng FIFO (FIFO Depth Probing)                          |
|  - Finish (chờ TFE/!BUSY), Chụp ảnh trạng thái lỗi (Snapshot) và Abort khẩn cấp   |
+-----------------------------------------------------------------------------------+
                                        | (Gọi Platform Hook qua hal_dw_spi.h)
                                        v
+-----------------------------------------------------------------------------------+
|  TẦNG 0: PLATFORM HARDWARE HAL (hal/cv181x/spi/src/hal_spi_platform.c)           |
|  - Mở khóa khối MIPI DSI D-PHY (0x0A0D1000) sang Digital GPO/GPI (0x00001F1F)    |
|  - Cấu hình PADCTRL (0x03001C70..0x03001C7C) mức 0x44 (Pull-Up, Drive Strength)   |
|  - Thiết lập Pinmux FPC2: Pin 15 (SCK), Pin 18 (MOSI), Pin 17 (MISO) -> Func 6   |
|  - Điều khiển chân Chip Select (CS) qua GPIO XGPIOC17 (FPC2 Pin 14)               |
|  - Cấp nguồn xung nhịp an toàn: XTAL 25 MHz (CLOCK_BYPASS bit 30)                |
+-----------------------------------------------------------------------------------+
                                        | (Tín hiệu điện 1.8V tại cổng FPC2)
                                        v
+-----------------------------------------------------------------------------------+
|  PHẦN CỨNG NGOÀI: TXS0108E (Level Shifter 1.8V <-> 3.3V) <-> STM32F103 / Tiny1C   |
+-----------------------------------------------------------------------------------+
```

---

## 2. CHI TIẾT TẦNG 0 — PLATFORM HARDWARE HAL

### 2.1. File mã nguồn & Vai trò
- **File thực thi:** `freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform.c`
- **File định nghĩa thanh ghi:** `freertos/cvitek/hal/cv181x/spi/src/dw_spi_regs.h`
- **Vai trò:** Cung cấp lớp thích ứng phần cứng cấp thấp nhất (Board Support). Tách biệt hoàn toàn việc cấu hình chân pin, nguồn xung nhịp và đặc thù phần cứng của SoC SOPHGO SG2002 ra khỏi thuật toán của Tầng 1.

### 2.2. Các thao tác phần cứng then chốt
1. **Quản lý Nguồn Xung (Clock Tree):**
   - Địa chỉ thanh ghi `CLOCK_BYPASS` (`0x03002038`).
   - Bit 30: Chuyển nguồn cấp cho `clk_spi` sang thạch anh **XTAL 25 MHz** cố định. Không dùng nguồn PLL biến thiên để đảm bảo độ chính xác tuyệt đối khi tính toán baudrate.
2. **Giải phóng kẹp Analog của khối MIPI DSI D-PHY (`0x0A0D1000`):**
   - Trên SG2002, các chân cổng FPC2 đi xuyên qua bộ D-PHY hiển thị màn hình. Mặc định khởi động, Lane 0, 1, 2 bị dìm xuống mức `0V` (Analog LP-00).
   - Tầng 0 thực thi công thức 8 bước mở khóa D-PHY sang chế độ **Digital GPO/GPI (`0x00001F1F`)**:
     - `REG_DSI_PHY_EN (0x0A0D1000) = 0x00000000` (Tắt DSI analog)
     - `REG_DSI_PHY_LANE_SEL (0x0A0D109C) = 0x000FFFFF`
     - `REG_DSI_PHY_PD (0x0A0D1064) = 0x00001F1F` (Tắt Power-down)
     - `REG_DSI_PHY_LPTX_OV (0x0A0D104C) = 0x001F001F`
     - `REG_DSI_PHY_PD_EN_TX (0x0A0D1054) = 0x00001F1F`
     - `REG_DSI_PHY_PD_TXDRV (0x0A0D1050) = 0x1F1F1F1F`
     - `REG_DSI_PHY_GPO (0x0A0D10C4) = 0x00001F1F` (Mở cổng xuất Digital)
     - `REG_DSI_PHY_GPI (0x0A0D10C8) = 0x00001F1F` (Mở cổng nhận Digital)
3. **Cấu hình Điểm tiếp xúc (PADCTRL) & Chuyển mạch chân (Pinmux):**
   - 4 thanh ghi `PADCTRL` (`0x03001C70..0x03001C7C`) cho 4 chân SPI được cập nhật mặt nạ `0x6C` về giá trị `0x44` (Bật Pull-up nội, Drive Strength mức 2).
   - Cấu hình Pinmux sang Func 6 (SPI0):
     - `PINMUX_MIPI_TXM2 (0x030011A4) = 6` $\rightarrow$ FPC2 Pin 15: **SCK**
     - `PINMUX_MIPI_TXM1 (0x030011AC) = 6` $\rightarrow$ FPC2 Pin 18: **MOSI / SDO**
     - `PINMUX_MIPI_TXP1 (0x030011B0) = 6` $\rightarrow$ FPC2 Pin 17: **MISO / SDI**
4. **Điều khiển Chip Select (CS) qua GPIO:**
   - Chân `PAD_MIPI_TXP2` (FPC2 Pin 14) được cấu hình sang Func 3: `XGPIOC17`.
   - Nạp mức cao (`CS_BIT`) vào thanh ghi `GPIOC_DATA` và `GPIOC_DIR` **trước khi** chuyển mux để triệt tiêu hoàn toàn xung nhiễu (glitch) xuống mức 0 lúc khởi động.

### 2.3. Các hàm API Tầng 0
```c
int hal_spi_platform_prepare(hal_spi_t *hw, bool force_xtal, bool external_pins);
// Chuẩn bị nguồn xung nhịp 25MHz, gán địa chỉ base 0x04180000, kiểm tra tính hợp lệ.

int hal_spi_platform_connect(hal_spi_t *hw);
// Thực thi recipe mở khóa D-PHY, PADCTRL, Pinmux Func 6 và gán GPIO CS mức cao.

int hal_spi_platform_cs(hal_spi_t *hw, bool active);
// Điều khiển kéo chân CS: active=true (kéo xuống 0V), active=false (kéo lên 1.8V). Có readback kiểm tra.

uint64_t hal_spi_time_us(void);
// Lấy thời gian hệ thống tính theo microsecond phục vụ tính toán timeout/deadline.
```

### 2.4. Khả năng tái sử dụng cho Product (Sản phẩm thương mại)
- **Tận dụng 100%:** Công thức mở khóa D-PHY và PADCTRL này là duy nhất và bắt buộc cho mọi sensor kết nối qua cổng FPC2 trên SG2002.
- **Ràng buộc phần cứng:** Không thể sử dụng đồng thời cổng FPC2 cho màn hình hiển thị MIPI DSI nếu đã dùng cho SPI0. Mức logic tại cổng FPC2 là **1.8V chuẩn**, rất an toàn và tương thích trực tiếp với các dòng camera nhiệt hiện đại (như Tiny1C).

---

## 3. CHI TIẾT TẦNG 1 — CONTROLLER HAL

### 3.1. File mã nguồn & Vai trò
- **File thực thi:** `freertos/cvitek/hal/cv181x/spi/src/hal_dw_spi.c`
- **File header:** `freertos/cvitek/hal/cv181x/spi/include/hal_dw_spi.h`
- **Vai trò:** Cung cấp trình điều khiển trực tiếp phần cứng ngoại vi **Synopsys DesignWare APB SSI (DW_SPI)**. Hoàn toàn độc lập với FreeRTOS (không include FreeRTOS.h), thuần C11, có thể chạy trên bare-metal hoặc unit test trên host model.

### 3.2. Thuật toán điều khiển cốt lõi
1. **Tự động dò độ sâu phần cứng FIFO (FIFO Depth Probing):**
   - Hàm `hal_spi_init()` tự động ghi tăng dần vào `DW_SPI_TXFTLR` và đọc lại để xác định chính xác dung lượng FIFO của chip (trên SG2002 là 8 phần tử) mà không cần hardcode trong mã nguồn.
2. **Tính toán Baudrate chính xác không vượt trần:**
   - Khảo sát thanh ghi chia tần `DW_SPI_BAUDR`: Nhận số chẵn $2 \le \text{BAUDR} \le 65534$.
   - Công thức: $\text{BAUDR} = 2 \times \lceil \frac{\text{Clock}}{2 \times \text{max\_hz}} \rceil$. Bảo đảm tần số thực tế `actual_hz` luôn $\le$ tần số yêu cầu của sensor.
3. **Cấu hình chế độ truyền (`DW_SPI_CTRLR0`):**
   - Hỗ trợ đầy đủ Mode 0, 1, 2, 3 qua 2 bit `SCPOL` (bit 7) và `SCPH` (bit 6).
   - Hỗ trợ kích thước frame từ 4 đến 16-bit (`DFS` bit 3..0).
   - Thiết lập Full Duplex (`TMOD = 0`). Chế độ loopback nội (`SRL` bit 11) chỉ kích hoạt khi có yêu cầu chẩn đoán.
4. **Vòng lặp Polling chống treo / chống tràn (Invariant Polling Loop):**
   - Luôn duy trì bất biến: `(sent - got) <= fifo_depth`.
   - Chỉ đẩy thêm byte vào TX FIFO khi bộ đệm chưa đầy và số lượng đang chờ nhận chưa vượt quá độ sâu FIFO. Ngăn chặn tuyệt đối lỗi tràn RX FIFO (`RXOI` / Overflow Error).
5. **Cơ chế Finish, Snapshot và Abort an toàn:**
   - `hal_spi_finish()`: Chờ cờ `TFE` (TX FIFO Rỗng) và `!BUSY` (Đường truyền vật lý hoàn tất).
   - `hal_spi_snapshot()`: Chụp lại thanh ghi trạng thái `DW_SPI_SR` và cờ ngắt thô `DW_SPI_RISR` để chuyển lên Tầng 2 phân tích trước khi tiến hành reset phần cứng.
   - `hal_spi_abort()`: Luôn chạy ở cuối mọi phiên (dù thành công hay lỗi) để giải phóng CS và tắt `SSIENR = 0`.

### 3.3. Các hàm API Tầng 1
```c
int hal_spi_init(hal_spi_t *hw);
// Khởi tạo controller, tắt SSIENR, tự dò FIFO depth, xác minh trạng thái an toàn.

int hal_spi_configure(hal_spi_t *hw, uint32_t max_hz, unsigned int mode,
                      unsigned int bits, bool loopback, uint32_t *actual_hz);
// Cấu hình thanh ghi CTRLR0, tính toán BAUDR chẵn, trả về tần số SCK thực tế.

int hal_spi_begin(hal_spi_t *hw);
// Kích hoạt SSIENR, chọn SER nội bộ, hạ CS vật lý qua Tầng 0.

int hal_spi_poll(hal_spi_t *hw, const void *tx, void *rx, size_t frames,
                 uint16_t fill, uint64_t deadline_us, size_t *received);
// Vòng lặp truyền nhận full duplex có kiểm soát deadline, trả về số frame thực nhận.

int hal_spi_finish(hal_spi_t *hw, uint64_t deadline_us);
// Chờ phần cứng phát xong bit cuối cùng và trả bus về trạng thái rỗi.

void hal_spi_snapshot(hal_spi_t *hw, hal_spi_snapshot_t *snapshot);
// Chụp cờ trạng thái SR và RISR.

int hal_spi_abort(hal_spi_t *hw);
// Nhả CS, ngắt SSIENR, đưa bus về trạng thái đóng.
```

### 3.4. Khả năng tái sử dụng cho Product
- **Tận dụng 100%:** Dùng cho toàn bộ việc khởi tạo, gửi lệnh điều khiển (Control Command), cài đặt tham số, đọc thanh ghi trạng thái của camera Tiny1C hoặc các chip ngoại vi SPI khác.
- **Đánh giá hiệu năng Polling vs DMA:**
  - Với các gói tin điều khiển, cấu hình sensor (< 4 KB): Polling phản hồi tức thì với độ trễ microsecond cực thấp, không tốn chi phí quản lý kênh DMA.
  - Với luồng dữ liệu video liên tục (25 fps VoSPI): Polling sẽ chiếm CPU trong lúc truyền. Do đó Tầng 1 đã được chuẩn bị sẵn kiến trúc để mở rộng backend sang DMA (Tầng 3/Milestone tiếp theo).

---

## 4. CHI TIẾT TẦNG 2 — CORE DRIVER / BUS & DEVICE MANAGER

### 4.1. File mã nguồn & Vai trò
- **File thực thi:** `freertos/cvitek/driver/spi/src/spi.c`
- **File header công khai:** `freertos/cvitek/driver/spi/include/spi.h`
- **Vai trò:** Cung cấp giao diện lập trình **Public API duy nhất** cho toàn bộ các Task trong FreeRTOS. Quản lý tài nguyên, bộ nhớ, đồng bộ đa luồng và bảo vệ phần cứng.

### 4.2. Cấu trúc dữ liệu chuẩn cho Sản phẩm

#### a) Cấu hình Bus (`spi_bus_config_t`)
```c
typedef struct {
    uint8_t force_xtal;      // 1: Ép dùng nguồn XTAL 25MHz an toàn; 0: Giữ nguyên
    uint8_t external_pins;   // 1: Mở FPC2 và dùng GPIO CS; 0: Dành cho loopback nội
} spi_bus_config_t;
```

#### b) Cấu hình Thiết bị ngoại vi (`spi_device_config_t`)
```c
typedef struct {
    uint32_t max_hz;            // Tần số tối đa (ví dụ 500000, 1000000)
    spi_mode_t mode;            // SPI_MODE_0..3 (Tiny1C dùng SPI_MODE_3)
    spi_bit_order_t bit_order;  // SPI_MSB_FIRST
    uint8_t bits_per_frame;     // 8-bit (hoặc 4..16)
    uint8_t cs_index;           // 0: Chân CS mặc định (XGPIOC17)
    uint8_t internal_loopback;  // 0: Truyền dây thật; 1: Loopback nội chẩn đoán
    uint16_t cs_setup_us;       // Thời gian giữ CS thấp trước khi phát clock (ví dụ 10us)
    uint16_t cs_hold_us;        // Thời gian giữ CS thấp sau khi clock xong (ví dụ 2us)
} spi_device_config_t;
```

#### c) Cấu trúc Thông điệp truyền nhận (`spi_message_t` & `spi_segment_t`)
Cho phép gom nhiều đoạn truyền liên tiếp trong **cùng một phiên giữ chân CS thấp**:
```c
typedef struct {
    const void *tx;       // Buffer phát (nếu NULL: tự động phát fill_word làm clock)
    void *rx;             // Buffer nhận (nếu NULL: tự động bỏ qua dữ liệu nhận)
    size_t frames;        // Số frame cần truyền trong đoạn này
    uint16_t fill_word;   // Giá trị đệm phát ra khi tx == NULL (thường là 0xFF hoặc 0x00)
} spi_segment_t;

typedef struct {
    const spi_segment_t *segments;  // Mảng các đoạn truyền (tối đa 32 segments)
    size_t segment_count;           // Số lượng đoạn
    uint32_t timeout_ms;            // Ngân sách thời gian toàn cục cho cả phiên (ms)
    spi_backend_t backend;          // SPI_BACKEND_POLL (hoặc SPI_BACKEND_DMA sau này)
} spi_message_t;
```

#### d) Kết quả bàn giao chi tiết (`spi_result_t`)
```c
typedef struct {
    uint32_t actual_hz;            // Tần số SCK thực tế bộ chia tạo ra
    size_t segments_completed;     // Số đoạn đã hoàn tất trọn vẹn
    size_t frames_received;        // Tổng số frame thực tế đã đọc về
    uint32_t controller_status;    // Thanh ghi DW_SPI_SR tại thời điểm kết thúc
    uint32_t controller_raw_irq;   // Thanh ghi ngắt DW_SPI_RISR
    spi_status_t transfer_status;  // Lỗi truyền dữ liệu gốc (Timeout, I/O, Argument...)
    spi_status_t cleanup_status;   // Trạng thái thu hồi phần cứng và nhả CS
} spi_result_t;
```

### 4.3. Các hàm Public API Tầng 2
```c
spi_status_t spi_bus_init(unsigned int bus_id, const spi_bus_config_t *config);
// Khởi tạo bus 0, kết nối phần cứng Tầng 0 và chuẩn bị Tầng 1.

spi_bus_t *spi_bus_get(unsigned int bus_id);
// Lấy con trỏ bus đã khởi tạo thành công.

spi_status_t spi_device_bind(spi_bus_t *bus, const spi_device_config_t *config,
                             spi_device_t **device);
// Gán cấu hình thiết bị (Mode, Hz, Setup/Hold timing), trả về handle spi_device_t.

spi_status_t spi_device_unbind(spi_device_t *device);
// Hủy đăng ký thiết bị khỏi bus, giải phóng slot pool tĩnh.

spi_status_t spi_transfer(spi_device_t *device, const spi_message_t *message,
                          spi_result_t *result);
// Thực thi giao dịch SPI trọn gói có bảo vệ Mutex, kiểm soát timeout và cleanup an toàn.
```

### 4.4. Các cơ chế bảo vệ công nghiệp trong Tầng 2
1. **Quản lý Mutex tĩnh (Static FreeRTOS Mutex):** Không dùng `malloc` động trong FreeRTOS để tránh hiện tượng phân mảnh bộ nhớ (Heap Fragmentation) khi chạy lâu ngày.
2. **Kiểm tra ranh giới và chống chồng lấn buffer (Validation Overlap):** Ngăn chặn người dùng vô tình truyền địa chỉ buffer TX và RX chồng lấn lên nhau gây hỏng dữ liệu.
3. **Cơ chế Fail-Safe cho Bus (`NOT_READY`):** Nếu một giao dịch gặp sự cố phần cứng nghiêm trọng và không thể nhả CS hoặc không thể dừng controller (`cleanup_status != SPI_OK`), Tầng 2 sẽ tự động đánh dấu bus là `NOT_READY`, từ chối mọi giao dịch tiếp theo để bảo vệ chân tín hiệu vật lý của sensor.

---

## 5. CƠ CHẾ GIAO TIẾP LIÊN NHÂN (IPC / SHARED DDR RAM) CHO PRODUCT

Trong một sản phẩm hoàn chỉnh, FreeRTOS (Core 1) thu thập dữ liệu thời gian thực và đẩy sang Linux (Core 0) xử lý. **Shared DDR RAM** là giải pháp chuẩn công nghiệp duy nhất đáp ứng được yêu cầu này.

### 5.1. Thiết kế vùng nhớ Telemetry ABI v1 (`0x8FF20000`)
Mã nguồn tại `spi_test_stm32.c` đã triển khai cấu trúc bộ nhớ chuẩn hóa 192 bytes tại địa chỉ cố định `0x8FF20000`:

```c
struct telemetry {
    uint32_t magic;      // 0x53544D33 ("STM3")
    uint32_t version;    // 1 (ABI v1)
    uint32_t seq;        // Seqlock: tăng trước và sau khi ghi dữ liệu
    uint32_t state;      // 1=WAITING, 2=RUNNING, 3=STOPPED, 4=FATAL
    uint32_t cycle;      // Tổng số chu kỳ truyền nhận đã chạy
    uint32_t pass;       // Số lần truyền nhận thành công 100%
    uint32_t fail;       // Số lần thất bại hoặc mismatch
    uint32_t errors;     // Số byte sai lệch trong frame gần nhất
    int32_t  status;     // Mã lỗi spi_status_t (0: SPI_OK)
    int32_t  cleanup;    // Mã lỗi dọn dẹp phần cứng
    uint32_t hz;         // Tần số SCK thực tế
    uint32_t frames;     // Số frame nhận được
    uint32_t first_bad;  // Vị trí byte đầu tiên bị sai
    uint32_t expected;   // Giá trị kỳ vọng tại vị trí sai
    uint32_t actual;     // Giá trị thực tế nhận được
    uint32_t length;     // Chiều dài frame truyền
    uint8_t  rx[16];     // Mẫu 16 byte đầu nhận được
    uint8_t  padding[48];// Đệm tròn 128 bytes (2 Cache lines)
    
    // Cache line thứ 3 (Offset 128 / 0x80): Dành riêng cho Linux ghi lệnh
    uint32_t command;    // 1: START, 0: STOP
    uint32_t reserved[15];
};
```

### 5.2. Tại sao kiến trúc này đạt chuẩn Product?
1. **Triệt tiêu False Sharing Cache:** Tách lệnh điều khiển `command` sang một cache line 64-byte riêng biệt ở offset 128. Việc Linux ghi `command` không bao giờ làm mất hiệu lực (invalidate) vùng báo cáo của FreeRTOS.
2. **Cơ chế Seqlock chống đọc dữ liệu rác (Torn Read):** Phía Linux đọc `seq` trước và sau khi copy dữ liệu. Nếu `seq` lẻ (FreeRTOS đang ghi) hoặc `seq_before != seq_after`, Linux biết dữ liệu chưa trọn vẹn và tự động đọc lại.
3. **Minh bạch bộ đệm Cache:** FreeRTOS luôn gọi `flush_dcache_range` và `inv_dcache_range` đúng vị trí để dữ liệu được xả thẳng xuống chip nhớ vật lý DDR RAM.

---

## 6. HƯỚNG DẪN ĐỌC HIỂU SOURCE CODE VÀ GỌI API CHO PRODUCT

Khi bạn viết mã nguồn cho sensor mới (như camera Tiny1C), đây là luồng mẫu chuẩn để ứng dụng gọi xuống driver:

```c
#include "spi.h"

void tiny1c_task(void *pvParameters)
{
    // BƯỚC 1: Khởi tạo Bus 0 qua Tầng 2
    spi_bus_config_t bus_cfg = {
        .force_xtal = 1,       // Ép xung XTAL 25 MHz an toàn
        .external_pins = 1     // Mở cổng FPC2 và kết nối GPIO CS
    };
    if (spi_bus_init(0, &bus_cfg) != SPI_OK) {
        // Xử lý lỗi khởi tạo phần cứng
        vTaskDelete(NULL);
        return;
    }
    spi_bus_t *bus = spi_bus_get(0);

    // BƯỚC 2: Đăng ký Thiết bị Tiny1C (Mode 3, 500 kHz, CS delay 10us/2us)
    spi_device_config_t dev_cfg = {
        .max_hz = 500000,          // 500 kHz (hoặc 1000000 cho 1 MHz)
        .mode = SPI_MODE_3,        // Mode 3 theo quy chuẩn VoSPI Tiny1C
        .bit_order = SPI_MSB_FIRST,
        .bits_per_frame = 8,
        .cs_index = 0,
        .internal_loopback = 0,
        .cs_setup_us = 10,         // Chờ 10us sau khi kéo CS thấp
        .cs_hold_us = 2            // Chờ 2us trước khi nhả CS cao
    };
    spi_device_t *tiny1c_dev = NULL;
    if (spi_device_bind(bus, &dev_cfg, &tiny1c_dev) != SPI_OK) {
        // Xử lý lỗi bind thiết bị
        vTaskDelete(NULL);
        return;
    }

    // BƯỚC 3: Thực thi truyền nhận dữ liệu
    uint8_t tx_dummy[256];
    uint8_t rx_packet[256];
    memset(tx_dummy, 0xFF, sizeof(tx_dummy)); // Dummy clock pump

    spi_segment_t seg = {
        .tx = tx_dummy,
        .rx = rx_packet,
        .frames = sizeof(rx_packet)
    };
    spi_message_t msg = {
        .segments = &seg,
        .segment_count = 1,
        .timeout_ms = 100,         // Timeout 100ms
        .backend = SPI_BACKEND_POLL
    };
    spi_result_t result;

    while (1) {
        // Thực thi giao dịch
        spi_status_t status = spi_transfer(tiny1c_dev, &msg, &result);
        
        if (status == SPI_OK) {
            // Nhận gói VoSPI thành công -> Đẩy vào Shared DDR RAM cho Linux
        } else {
            // Lỗi giao dịch -> Kiểm tra result.transfer_status và result.cleanup_status
        }
        vTaskDelay(pdMS_TO_TICKS(40)); // Lặp lại theo tần số khung hình (25 fps)
    }
}
```

---

## 7. BẢNG MÃ LỖI VÀ Ý NGHĨA KỸ THUẬT (`spi_status_t`)

| Mã lỗi | Giá trị | Ý nghĩa và cách xử lý trong Product |
|---|:---:|---|
| `SPI_OK` | `0` | Giao dịch thành công hoàn hảo, CS đã nhả về mức cao an toàn. |
| `SPI_ERR_ARGUMENT` | `-1` | Con trỏ NULL, buffer overlap, độ dài = 0, hoặc cấu hình không hợp lệ. |
| `SPI_ERR_UNSUPPORTED` | `-2` | Chế độ chưa hỗ trợ (ví dụ LSB-first, hoặc backend DMA chưa cài đặt). |
| `SPI_ERR_NOT_READY` | `-3` | Bus đang ở trạng thái khóa an toàn do lỗi phần cứng trước đó chưa khắc phục. |
| `SPI_ERR_TIMEOUT` | `-4` | Hết ngân sách thời gian `timeout_ms` trước khi phần cứng truyền xong. |
| `SPI_ERR_IO` | `-5` | Lỗi phần cứng nghiêm trọng: Không kéo được CS, controller báo lỗi tràn RX, hoặc không thể shutdown phần cứng. |
| `SPI_ERR_BUSY` | `-6` | Bus đang được chiếm bởi một luồng khác và mutex bị timeout. |

---

## 8. KẾT LUẬN VÀ BÀN GIAO CHO GIAI ĐOẠN TIẾP THEO

Ngăn xếp driver từ **Tầng 0 đến Tầng 2** đã chính thức hoàn thành, đạt độ ổn định công nghiệp tuyệt đối:
1. Đã giải phóng hoàn toàn các vướng mắc phần cứng SoC SG2002 (D-PHY, PADCTRL, Pinmux).
2. Đã xác thực thực tế trên dây nối vật lý với STM32F103 Slave: **Tỷ lệ gói tin hợp lệ đạt 100% qua hàng trăm chu kỳ**.
3. Cơ chế Telemetry và truyền thông liên nhân Zero-Copy qua **DDR `0x8FF20000`** đã hoạt động trơn tru.

**Giai đoạn tiếp theo: Tích hợp Tầng 3 (Giao thức VoSPI đọc dữ liệu ảnh nhiệt và I2C cấu hình cho module camera Tiny1C)**.
