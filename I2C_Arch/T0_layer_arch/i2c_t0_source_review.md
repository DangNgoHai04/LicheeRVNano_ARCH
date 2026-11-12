# Đánh giá Source và Trạng thái Hoàn thiện của Tầng 0 (GPIO Platform) — I²C Bitbang

Cập nhật: **09-11-2026**. Board mục tiêu: **LicheeRV-Nano (SOPHGO SG2002 / CV181x — `GPIOA15 = SCL`, `GPIOA27 = SDA`)**.

**Kết luận kiểm tra source:** Tầng 0 của I²C Bitbang (`hal_i2c_gpio_platform.c` và `hal_i2c_gpio_platform.h`) đã được triển khai chuẩn xác, gọn nhẹ và tuân thủ nghiêm ngặt quy tắc an toàn điện học Open-Drain. Toàn bộ địa chỉ thanh ghi Clock, Pinmux (`SPK_EN` / `EMMC_DAT3`), thanh ghi điều khiển `GPIOA` (`DR`, `DDR`, `EXT`) và bộ đếm thời gian thực `rdtime` đã được đối chiếu khớp 100% với TRM SG2002 và biên dịch sạch (`0 warning, 0 error`) với cờ `-Wall -Wextra -Werror` trên trình biên dịch RISC-V 64-bit.

---

## 1. Phạm vi và Nguồn đối chiếu

- **Source Tầng 0**:
  - [`hal_i2c_gpio_platform.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c): Thực thi khởi tạo GPIOA Open-Drain, Pinmux, kiểm tra readback và cung cấp 4 hàm hook (`drive`, `sample`, `time_us`, `delay_us`).
  - [`hal_i2c_gpio_platform.h`](../../../freertos/cvitek/hal/cv181x/i2c/include/hal_i2c_gpio_platform.h): Khai báo giao diện `bb_gpio_platform_init()`.
- **Tài liệu phần cứng đối chiếu**:
  - [TRM SG2002 v1.02](../../HW/TRM/TRM_v102.md): Chương Clock (`0x03002000` bit 29) và §21.4 Synopsys DW APB GPIO (`0x03020000`).
  - [SDK Pinmux Register](../../../freertos/cvitek/hal/cv181x/config/cv181x_reg_fmux_gpio.h) và [Function Select](../../../freertos/cvitek/hal/cv181x/config/cv181x_pinlist_swconfig.h).
  - [Pinout & Best Practice](../../../best-practice.md) và [Device Tree RISC-V](../../../build/boards/sg200x/sg2002_licheervnano_sd/dts_riscv/sg2002_licheervnano_sd.dts).

---

## 2. Bảng Đối chiếu Các Hạng mục Kỹ thuật Đã Hoàn thiện ở Tầng 0

| Hạng mục | Cơ chế thực thi trong Source Code | Đánh giá & Trạng thái | File thực thi |
| :--- | :--- | :--- | :--- |
| **1. Cấp xung nhịp GPIO (`CLK_EN_0`)** | Bật bit `29` (`1U << 29`) tại thanh ghi `0x03002000` trong vùng critical section. | **HOÀN THÀNH:** Đảm bảo khối APB GPIO luôn được cấp clock trước khi truy cập `GPIOA`. | [`hal_i2c_gpio_platform.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) |
| **2. Giả lập Open-Drain Không Xung Cao** | Giữ cố định `GPIOA_DR` (`0x03020000`) ở mức `0` cho bit 15 và 27. Hàm `drive()` chỉ thay đổi `GPIOA_DDR` (`0x03020004`): `1` để kéo xuống `0V`, `0` để nhả Hi-Z. | **HOÀN THÀNH:** Loại bỏ 100% nguy cơ phát mức `HIGH` push-pull gây đoản mạch khi Slave kéo SDA/SCL xuống thấp. | [`hal_i2c_gpio_platform.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) |
| **3. Thứ tự Khởi tạo Glitch-Free** | Nhả `GPIOA_DDR &= ~MASK` và xóa `GPIOA_DR &= ~MASK` **trước khi** ghi `0x0300103C = 3` (`A15`) và `0x03001058 = 3` (`A27`). | **HOÀN THÀNH:** Ngăn chặn xung giả (spurious START/STOP glitch) xuất hiện trên đường dây lúc chuyển mạch Pinmux. | [`hal_i2c_gpio_platform.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) |
| **4. Đọc Kiểm chứng (`Readback`)** | Kiểm tra `(mmio_read_32(0x0300103C) & 7U) == 3U`, `(mmio_read_32(0x03001058) & 7U) == 3U` và `(mmio_read_32(GPIOA_DDR) & MASK) == 0`. | **HOÀN THÀNH:** Phát hiện ngay nếu Pinmux bị khóa hoặc thanh ghi GPIO không phản hồi đúng trạng thái nhả. | [`hal_i2c_gpio_platform.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) |
| **5. Đọc mức dây thật (`GPIOA_EXT`)** | Hàm `sample()` đọc thanh ghi `0x03020050` (`GPIOA_EXT`) thay vì đọc lại thanh ghi latch `GPIOA_DR`. | **HOÀN THÀNH:** Phản ánh trung thực trạng thái điện áp vật lý trên chân để T1 kiểm tra Clock Stretching, ACK/NACK và Arbitration. | [`hal_i2c_gpio_platform.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) |
| **6. Timebase `rdtime` 64-bit** | Đọc counter phần cứng `rdtime`, tính `time_us()` bằng phép chia tách phần nguyên và phần dư chống tràn số 64-bit. | **HOÀN THÀNH:** Không phụ thuộc tần số xung nhịp CPU hay vòng lặp NOP cảm tính; đo đúng thời gian thực kể cả khi bị ngắt. | [`hal_i2c_gpio_platform.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) |
| **7. Bảo vệ Singleton & Ownership** | Kiểm tra cờ `ownership_confirmed` và biến tĩnh `claimed` trước mọi thao tác MMIO. | **HOÀN THÀNH:** Ngăn ngừa việc tái khởi tạo đè lên bus đang chạy hoặc chạy khi chưa xác nhận quyền sở hữu `GPIOA`. | [`hal_i2c_gpio_platform.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) |

---

## 3. Các Lưu ý Khi Tích hợp Hệ thống (Board Integration Notes)

1. **Giải phóng chân trong Linux Device Tree**:
   - Node `i2c5` (chân `porta 15` và `porta 27`) trong [`sg2002_licheervnano_sd.dts`](../../../build/boards/sg200x/sg2002_licheervnano_sd/dts_riscv/sg2002_licheervnano_sd.dts) đã được chuyển sang `status = "disabled";`. Cần biên dịch lại DTB/FIP khi nạp lên thẻ nhớ để thay đổi có hiệu lực.
2. **Đồng bộ truy cập Bank `GPIOA` giữa Linux và FreeRTOS**:
   - Đảm bảo không có driver Linux nào liên tục thay đổi hướng chân (`GPIOA_DDR`) trên cùng Port A trong lúc FreeRTOS đang thực hiện giao dịch I²C Bitbang, hoặc hoàn tất cấu hình các chân GPIOA của Linux trước khi khởi chạy tác vụ điều khiển camera trên FreeRTOS.
