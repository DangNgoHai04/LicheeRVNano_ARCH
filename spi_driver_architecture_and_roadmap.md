# Kiến trúc driver SPI SG2002 và lộ trình tích hợp Tiny1C

**Phiên bản:** Bản cập nhật nghiệm thu thực tế phần cứng (Hardware Qualified), ngày 24-10-2026.  
**Board mục tiêu:** LicheeRV-Nano, SoC SG2002/CV181x, cấu hình `sg2002_licheervnano_sd`.  
**Trạng thái hiện tại:** **Milestone A0 & A1 ĐÃ HOÀN THÀNH 100% TRÊN PHẦN CỨNG THẬT** (Đã nghiệm thu vật lý qua STM32F103 Slave Mode 3, 500 kHz, > 600 packets, 0 bit error, 100% PASS).  
**Mục tiêu tiếp theo:** Milestone B1 (DMA Service / SPI DMA Backend) và Milestone C (Giao thức Tiny1C VoSPI / I2C Adapter).

---

## Mục lục

1. [Phạm vi, nguồn và mức bằng chứng](#1-phạm-vi-nguồn-và-mức-bằng-chứng)
2. [Trạng thái hiện tại](#2-trạng-thái-hiện-tại)
3. [Phần cứng và quyền sở hữu tài nguyên](#3-phần-cứng-và-quyền-sở-hữu-tài-nguyên)
4. [Kiến trúc và ranh giới từng tầng](#4-kiến-trúc-và-ranh-giới-từng-tầng)
5. [Tầng 0: platform, clock, reset và chân](#5-tầng-0-platform-clock-reset-và-chân)
6. [Tầng 1: controller HAL và polling](#6-tầng-1-controller-hal-và-polling)
7. [Tầng 2: hợp đồng API bus và thiết bị](#7-tầng-2-hợp-đồng-api-bus-và-thiết-bị)
8. [DMA service và backend SPI DMA](#8-dma-service-và-backend-spi-dma)
9. [Tầng 3: thiết bị kiểm thử STM32 (ĐÃ NGHIỆM THU)](#9-tầng-3-thiết-bị-kiểm-thử-stm32-đã-nghiệm-thu)
10. [Tầng 3: protocol Tiny1C](#10-tầng-3-protocol-tiny1c)
11. [Tầng 4: ứng dụng, bộ nhớ DDR và Linux (ABI v1)](#11-tầng-4-ứng-dụng-bộ-nhớ-ddr-và-linux-abi-v1)
12. [Build và kiểm chứng](#12-build-và-kiểm-chứng)
13. [Trạng thái các vấn đề kỹ thuật](#13-trạng-thái-các-vấn-đề-kỹ-thuật)
14. [Milestone và ma trận nghiệm thu](#14-milestone-và-ma-trận-nghiệm-thu)
15. [Bàn giao và cập nhật trạng thái](#15-bàn-giao-và-cập-nhật-trạng-thái)

---

## 1. Phạm vi, nguồn và mức bằng chứng

### 1.1. Phạm vi
Xây dựng driver SPI dùng chung trên FreeRTOS của lõi RISC-V C906L (Core 1), làm nền tảng kết nối thời gian thực cho cảm biến ngoại vi (mục tiêu chính: camera nhiệt Tiny1C). Linux chạy trên lõi chính (Core 0) tiếp nhận dữ liệu ứng dụng qua cơ chế vùng nhớ chia sẻ (Shared DDR RAM Zero-Copy).

SPI0 (trên cổng FPC2 của LicheeRV-Nano) là mục tiêu đã hoàn thiện tầng 0–2. Driver SPI độc lập với các giao thức tầng trên, không chứa mã lệnh camera, độ phân giải hay công thức tính nhiệt độ.

### 1.2. Nguồn tham chiếu
| Nguồn | Nội dung đối chiếu |
| --- | --- |
| [TRM local](TRM/TRM_v102.md) | §8.9.38 clock divider; §9.2.2 DMA remap; §21.3 SPI và registers |
| [Pinmux local](Pinmux/Pinmux.md) | Function select, miền điện áp, pad control |
| [Schematic FPC2](Schematic/LicheeRV_Nano_70415_Schematic_AI.md) | Net, chân FPC2, nguồn điện áp 1.8V |
| `freertos/cvitek/hal/cv181x/spi/` | Tầng 0 (Platform) & Tầng 1 (DesignWare SSI HAL) |
| `freertos/cvitek/driver/spi/` | Tầng 2 (Core Driver / Public API `spi.h`) |
| `freertos/cvitek/task/comm/` | Task kiểm thử phần cứng `spi_test_stm32.c` |
| [Báo cáo nghiệm thu Tầng 0–2](spi_layers_0_2_completion_report.md) | Báo cáo chi tiết và hướng dẫn đọc mã nguồn |
| [Báo cáo kiểm thử STM32](test_stm32.md) | Nhật ký đo kiểm thực tế trên phần cứng thật |

---

## 2. Trạng thái hiện tại

### 2.1. Ma trận thành phần đã triển khai
| Thành phần | Mã nguồn thực tế | Trạng thái nghiệm thu |
| --- | --- | --- |
| **Platform SPI0 (Tầng 0)** | `hal_spi_platform.c` | **BOARD QUALIFIED**: Đã xác thực mở khóa D-PHY (`0x1F1F`), PADCTRL (`0x44`), Pinmux Func 6, GPIO CS `XGPIOC17`. |
| **Controller HAL (Tầng 1)** | `hal_dw_spi.c`, `hal_dw_spi.h` | **BOARD QUALIFIED**: Đã xác thực Mode 3, 500 kHz, Full Duplex polling, FIFO invariant, Finish/Abort an toàn. |
| **Bus Manager (Tầng 2)** | `spi.c`, `spi.h` | **BOARD QUALIFIED**: Đã xác thực vòng đời Bus, Device Bind, Mutex tĩnh, truyền gói 16B..4096B quay vòng thành công 100%. |
| **STM32 Slave Test (Tầng 3)** | `spi_test_stm32.c`, PlatformIO STM32 | **BOARD QUALIFIED**: Giao thức Counter stream + Echo, 601/601 packets valid (100%), 0 bit error, 0 fail. |
| **Telemetry DDR RAM (Tầng 4)**| Phân vùng `.stm32_telemetry` (`0x8FF20000`)| **BOARD QUALIFIED**: ABI v1, 192 bytes, Seqlock, tách riêng cache line 64B, đọc Zero-Copy qua `/dev/mem`. |
| **DMA Service / Backend** | Chuẩn bị triển khai | Milestone B1 tiếp theo. |
| **Tiny1C VoSPI / I2C** | Đã có tài liệu kỹ thuật | Milestone C tiếp theo. |

---

## 3. Phần cứng và quyền sở hữu tài nguyên

### 3.1. Phân bổ chân FPC2 (SG2002 1.8V $\longleftrightarrow$ TXS0108E $\longleftrightarrow$ STM32 3.3V)
```
SG2002 FPC2 (1.8V)               TXS0108E Shifter            STM32 Blue Pill (3.3V)
------------------               ----------------            ----------------------
Pin 15 (PAD_MIPI_TXM2: SCK)  --->   A1 <---> B1   -------->   PA5 (SPI1_SCK)
Pin 14 (PAD_MIPI_TXP2: CS)   --->   A2 <---> B2   -------->   PA4 (SPI1_NSS)
Pin 18 (PAD_MIPI_TXM1: MOSI) --->   A3 <---> B3   -------->   PA7 (SPI1_MOSI)
Pin 17 (PAD_MIPI_TXP1: MISO) <---   A4 <---> B4   <--------   PA6 (SPI1_MISO)
Pin 26/29 (VCC 1.8V)         --->   VCCA & OE
                                    VCCB          <--------   3.3V Pin
Pin 13/16 (GND)              --->   GND           <--------   GND Pin
```

### 3.2. Quản lý quyền sở hữu liên nhân (Linux vs FreeRTOS Ownership)
- **Vấn đề tranh chấp SPI0:** Mặc định trong bản build Linux gốc (`boot.sd`), driver `dw_spi_mmio` chiếm quyền node `spi0@04180000` (`/dev/spidev0.0`).
- **Giải pháp đã kiểm chứng:**
  - Giải phóng tại Runtime: Chạy lệnh `echo 4180000.spi0 > /sys/bus/platform/drivers/dw_spi_mmio/unbind` trên Linux.
  - Giải phóng từ gốc: Đặt `status = "disabled"` cho `&spi0` trong Device Tree Linux (`sg2002_licheervnano_sd.dts`) và đóng gói vào `boot.sd`.

---

## 4. Kiến trúc và ranh giới từng tầng

```
+-----------------------------------------------------------------------------------+
|  TẦNG 4: LINUX APPLICATION & ZERO-COPY PIPELINE                                  |
|  - Ứng dụng đọc dữ liệu từ /dev/mem tại 0x8FF20000 (Seqlock, không tốn CPU copy)  |
+-----------------------------------------------------------------------------------+
                                         ^
                                         | (DDR Telemetry ABI v1 tại 0x8FF20000)
                                         v
+-----------------------------------------------------------------------------------+
|  TẦNG 3: SENSOR / TEST PROTOCOL (FreeRTOS Core 1)                                 |
|  - Task nghiệm thu STM32 (spi_test_stm32.c) -> Sau này là Tiny1C VoSPI Task      |
+-----------------------------------------------------------------------------------+
                                         | (Gọi Public API qua spi.h)
                                         v
+-----------------------------------------------------------------------------------+
|  TẦNG 2: CORE DRIVER / BUS & DEVICE MANAGER (driver/spi/src/spi.c)               |
|  - spi_bus_init, spi_bus_get, spi_device_bind, spi_transfer                       |
|  - Quản lý Mutex tĩnh, Deadline hữu hạn, CS setup/hold (10us/2us)                |
+-----------------------------------------------------------------------------------+
                                         | (Gọi HAL API qua hal_dw_spi.h)
                                         v
+-----------------------------------------------------------------------------------+
|  TẦNG 1: CONTROLLER HAL (hal/cv181x/spi/src/hal_dw_spi.c)                         |
|  - Synopsys DesignWare SSI driver thuần C11: FIFO invariant, BAUDR, Finish/Abort  |
+-----------------------------------------------------------------------------------+
                                         | (Gọi Platform Hook)
                                         v
+-----------------------------------------------------------------------------------+
|  TẦNG 0: PLATFORM HARDWARE HAL (hal/cv181x/spi/src/hal_spi_platform.c)           |
|  - Recipe mở khóa D-PHY (0x1F1F), PADCTRL (0x44), Pinmux Func 6, GPIO CS XGPIOC17 |
+-----------------------------------------------------------------------------------+
```

---

## 5. Tầng 0: Platform, Clock, Reset và Chân

- **File mã nguồn:** `hal/cv181x/spi/src/hal_spi_platform.c`
- **Nguồn Clock:** XTAL 25 MHz an toàn (bit 30 của `0x03002038`).
- **Mở khóa D-PHY (`0x0A0D1000`):** Áp dụng cấu hình 8 thanh ghi chuyển Lane 0..4 sang Digital GPO/GPI (`0x00001F1F`).
- **PADCTRL:** Thiết lập `0x44` (Pull-Up nội kích hoạt, Drive Strength mức 2) tại `0x03001C70..0x03001C7C`.
- **Chip Select (CS):** Sử dụng chân GPIO `XGPIOC17` (Active-Low) thay vì native hardware CS. Cơ chế này đảm bảo FreeRTOS kiểm soát chính xác thời gian trễ `cs_setup_us` (10 µs) và `cs_hold_us` (2 µs), loại bỏ hiện tượng co cụm xung nhịp làm hỏng đồng bộ Slave.

---

## 6. Tầng 1: Controller HAL và Polling

- **File mã nguồn:** `hal/cv181x/spi/src/hal_dw_spi.c`, `hal/cv181x/spi/include/hal_dw_spi.h`
- **Bảo toàn bất biến FIFO:** Thuật toán Polling luôn duy trì điều kiện `(sent - got) <= fifo_depth`, ngăn chặn triệt để lỗi tràn bộ đệm nhận (RX FIFO Overflow).
- **Phân tách trạng thái:** Tách biệt rõ ràng giữa việc chờ đường truyền vật lý kết thúc (`hal_spi_finish`), chụp ảnh lỗi thanh ghi (`hal_spi_snapshot`), và dọn dẹp nhả CS (`hal_spi_abort`).
- **Độ chính xác Baudrate:** Tính toán `DW_SPI_BAUDR` chẵn từ nguồn 25 MHz, đảm bảo tần số thực tế không vượt quá trần cấu hình.

---

## 7. Tầng 2: Hợp đồng API Bus và Thiết bị

- **File mã nguồn:** `driver/spi/src/spi.c`, `driver/spi/include/spi.h`
- **Giao diện công khai (Public API):**
  - `spi_bus_init()` / `spi_bus_get()`: Khởi tạo và truy xuất Bus 0.
  - `spi_device_bind()` / `spi_device_unbind()`: Gán cấu hình thiết bị từ pool tĩnh 4 slot (không dùng malloc).
  - `spi_transfer()`: Thực thi giao dịch nguyên tử (atomic transaction) gồm 1..32 segments dưới một lần hạ CS duy nhất.
- **Độc lập và bảo vệ lỗi:**
  - `result->transfer_status`: Lưu trữ mã lỗi truyền nhận ban đầu.
  - `result->cleanup_status`: Lưu trữ kết quả thu hồi phần cứng.
  - Nếu xảy ra lỗi nghiêm trọng không thể giải phóng CS, Bus tự động chuyển trạng thái `NOT_READY` để bảo vệ an toàn phần cứng.

---

## 8. DMA Service và Backend SPI DMA

- **Kế hoạch triển khai:** Milestone B1.
- **Mục tiêu:** Nâng cấp backend `SPI_BACKEND_DMA` cho Tầng 2 để phục vụ luồng video 25 fps của camera nhiệt Tiny1C (giải phóng tải CPU cho lõi FreeRTOS).
- **Yêu cầu kỹ thuật đã xác định:**
  - SoC DMA IP: `snps,dmac-bm` tại base `0x04330000`.
  - Request remap theo TRM §9.2.2: SPI0 RX = 16, TX = 17 (thanh ghi `sysdma_remap` tại `0x03000154`).
  - Quản lý Cache: `flush_dcache_range` cho TX buffer và `inv_dcache_range` cho RX buffer trước/sau khi truyền.

---

## 9. Tầng 3: Thiết bị kiểm thử STM32 (ĐÃ NGHIỆM THU)

### 9.1. Cấu hình phần cứng & Firmware Slave
- **Dự án STM32:** `/home/haidang/Documents/PlatformIO/Projects/STM32_SPI0_TEST`
- **Vi điều khiển:** STM32F103C8T6 (Blue Pill), xung nhịp 72 MHz (HSE 8MHz -> PLL 72MHz, fallback HSI).
- **Chế độ SPI:** SPI1 Slave Mode 3 (`CPOL = 1, CPHA = 1`), 8-bit MSB, Hardware NSS (PA4).
- **Cơ chế DMA:**
  - DMA1 Channel 2 (RX): Ring buffer 2048 bytes nhận tự động.
  - DMA1 Channel 3 (TX): Mảng đếm tuần tự `counter_tail` (`0x01, 0x02, ..., 0xFF, 0x00...`). Byte đầu `0x00` preload trong `DR`.
- **Cải tiến dập nhiễu sườn xung:** Bật Input Pull-Up trên chân PA4 (CS) và PA5 (SCK); hạ slew rate chân MISO (PA6) xuống 10 MHz (`0x9`) để triệt tiêu rung xung (ringing) qua mạch đệm TXS0108E.

### 9.2. Kết quả đo kiểm thực tế trên bo mạch
```text
========================================================================
             KẾT QUẢ NGHIỆM THU PHẦN CỨNG THỰC TẾ
========================================================================
- Tổng số chu kỳ FreeRTOS đã chạy  : > 450 chu kỳ liên tục
- Tổng số gói tin STM32 đã nhận     : 601 gói
- Tỷ lệ gói tin hợp lệ (Valid)      : 601 / 601 gói (100.0% SUCCESS)
- Lỗi giao thức (Protocol Errors)   : 0
- Lỗi phần cứng (OVR / DMA Errors)  : 0
- Lỗi rớt bit (Bit Errors)          : 0 (Khớp tuyệt đối từng bit)
- Kích thước payload kiểm tra       : Quay vòng 16B, 256B, 1024B, 2048B, 4096B
- Tần số xung nhịp đo được          : 500,000 Hz (500 kHz ổn định)
========================================================================
```

---

## 10. Tầng 3: Protocol Tiny1C

### 10.1. Thông số giao thức VoSPI (Đã đối chiếu)
- Chế độ SPI: **Mode 3 (`CPOL = 1, CPHA = 1`)**, 8-bit MSB.
- Tần số hoạt động: Khảo sát ở mức 500 kHz đến 1 MHz cho điều khiển; tăng lên 23.4 MHz – 31.25 MHz khi chuyển sang nhận luồng video DMA.
- Định dạng gói tin: Lệnh New-frame `0xAA`, gói header và payload ảnh nhiệt YUYV/Raw14.

### 10.2. Giao diện cấu hình I2C Adapter
- Địa chỉ thiết bị: 7-bit `0x3C` (hoặc byte ghi `0x78`).
- Giao diện thanh ghi 16-bit: Gửi tập lệnh CCI / VDCMD để cấu hình shutter tự động, dải đo nhiệt độ và chế độ xuất ảnh.

---

## 11. Tầng 4: Ứng dụng, Bộ nhớ DDR và Linux (ABI v1)

### 11.1. Cấu trúc Vùng nhớ Telemetry ABI v1 tại `0x8FF20000`
Vùng nhớ 192 bytes được cố định tại địa chỉ RAM DDR `0x8FF20000` (được bảo vệ bằng `ASSERT` trong linker script `cv181x_lscript.ld`):

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

### 11.2. Kiểm tra trực tiếp từ Linux qua `/dev/mem`
Lệnh Python chạy trên bo mạch (`root@192.168.1.27`):
```bash
python3 /tmp/read_stm32_telemetry.py
```
Kết quả trả về phản ánh chính xác trạng thái thời gian thực của Core FreeRTOS mà không tiêu tốn tài nguyên truyền thông hay can thiệp vào cổng serial của Linux.

---

## 12. Build và kiểm chứng

### 12.1. Quy trình Build Firmware hoàn chỉnh
```bash
# 1. Build thư viện và Task FreeRTOS
cd freertos/cvitek/build/task && ninja install && cd ../../../..

# 2. Build gói FIP hoàn chỉnh (FSBL + FreeRTOS + U-Boot)
source build/cvisetup.sh
defconfig sg2002_licheervnano_sd
make fip

# 3. Nạp sang bo mạch và khởi động lại
scp install/soc_sg2002_licheervnano_sd/fip.bin root@192.168.1.27:/boot/fip.bin
ssh root@192.168.1.27 "sync && reboot"
```

---

## 13. Trạng thái các vấn đề kỹ thuật

| Mã | Vấn đề | Trạng thái | Đánh giá nghiệm thu |
|:---:|---|:---:|---|
| **O1** | Clock contract XTAL 25 MHz | **ĐÃ ĐÓNG (CLOSED)** | Đã nghiệm thu tần số 500 kHz thực tế, chính xác tuyệt đối. |
| **O2** | Tranh chấp SPI0 với Linux | **ĐÃ ĐÓNG (CLOSED)** | Đã nghiệm thu unbind runtime và disable DTS. |
| **O3** | CS setup/hold & chống glitch | **ĐÃ ĐÓNG (CLOSED)** | Đã nghiệm thu qua GPIO `XGPIOC17`, trễ 10us/2us hoạt động hoàn hảo. |
| **O4** | Build & Link Public API | **ĐÃ ĐÓNG (CLOSED)** | Compile sạch, không warning, liên kết thư viện chuẩn. |
| **O5** | Nguồn PLL cao tốc | Đang mở | Phục vụ Milestone B0 khi cần đẩy SCK lên >20 MHz. |
| **O6** | DMA Service / Backend | Đang mở | Chuẩn bị triển khai cho Milestone B1. |
| **O7** | Layout Frame Buffer DDR | **ĐÃ ĐÓNG MỘT PHẦN** | Phân vùng `.stm32_telemetry` (`0x8FF20000`) đã hoạt động tốt. Cần mở rộng cho video buffer. |
| **O8** | Tiny1C VoSPI protocol | Đang mở | Chờ tích hợp tại Milestone C. |
| **O9** | I2C control adapter | Đang mở | Chờ tích hợp tại Milestone C. |
| **O10**| Nghiệm thu Physical Slave | **ĐÃ ĐÓNG (CLOSED)** | Nghiệm thu thành công 100% với STM32F103 Slave. |

---

## 14. Milestone và ma trận nghiệm thu

### 14.1. Milestone A0 — Chốt nền tảng phần mềm & Host Model
- **Trạng thái:** **HOÀN THÀNH (DONE)**. Vượt qua 26 kịch bản host model với UBSan.

### 14.2. Milestone A1 — Nghiệm thu Polling Tầng 0–2 trên Bo mạch Thực tế
- **Trạng thái:** **HOÀN THÀNH 100% (BOARD QUALIFIED)**.
- Giao tiếp thông suốt hai chiều SG2002 FreeRTOS $\longleftrightarrow$ STM32F103 Slave qua mạch đệm TXS0108E. Đạt 100% tỷ lệ gói tin hợp lệ.

### 14.3. Milestone B1 — Nâng cấp Backend DMA (Tiếp theo)
- Triển khai `SPI_BACKEND_DMA` trên Tầng 2.
- Remap kênh DMA SPI0 RX=16, TX=17 trên ngoại vi `snps,dmac-bm`.
- Kiểm chứng truyền nhận mảng lớn không tiêu tốn chu kỳ CPU.

### 14.4. Milestone C — Tích hợp Camera nhiệt Tiny1C (Mục tiêu cuối)
- Tích hợp driver I2C điều khiển tham số camera.
- Triển khai parser giao thức VoSPI (đồng bộ frame, kiểm tra header, giải mã ảnh nhiệt).
- Bàn giao luồng video Zero-Copy sang Linux xử lý hiển thị.

---

## 15. Bàn giao và cập nhật trạng thái

Ngăn xếp driver SPI từ **Tầng 0 (Platform HAL)**, **Tầng 1 (DesignWare SSI HAL)** đến **Tầng 2 (Core Driver / Device API)** đã chính thức đạt chứng chỉ kiểm chứng phần cứng thực tế (**Hardware Qualified**). Toàn bộ kiến trúc và mã nguồn đã sẵn sàng bàn giao cho giai đoạn phát triển camera Tiny1C.
