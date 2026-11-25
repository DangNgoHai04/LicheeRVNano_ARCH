# 01 — Hoàn thiện driver T3

> **Vị trí mã nguồn chính thức (25-11-2026):** Toàn bộ mã nguồn T3 (`src/`, `include/`, `tests/`, `rtos/thermal_service.c`, `CMakeLists.txt`, `Makefile`, `README.md`) nằm độc lập trong [`freertos/cvitek/driver/sensor/tiny1C/`](../../../freertos/cvitek/driver/sensor/tiny1C/README.md). Các file cũ `legacy_reference/*.txt` được lưu trữ tham khảo tại `Doc_md/Tiny-1C/Driver_T3/legacy_reference/`.

## Công việc đã thực hiện

1. Chặn build target thiếu header thật. RISC-V không được định nghĩa THERMAL_HOST_TEST;
   thiếu FreeRTOS/Soft-I2C/SPI sẽ lỗi build thay vì trả dữ liệu giả.
2. GPIOA ownership mặc định false; DMA mặc định false. Polling là mặc định bring-up.
3. Probe đọc `DEV_INFO_CHIP_ID`, `PROJECT`, `FW_DATE`, `FW_VERSION`, `PN` và `SN`,
   ghi nhận bitmask thành công (`probe_ok_mask`), mã trạng thái thanh ghi `0x0200`
   (`vdcmd_status`) và `cmd_echo[8]` (`0x1D00..0x1D07`) để chẩn đoán. Lỗi transport ở PROJECT, PN hoặc SN trả `IR_NOT_GET_DEVICE_INFO`; khi gặp `IR_CHECK_DONE_FAIL` ở các mục này, driver ghi log cảnh báo và cho phép tiếp tục để hỗ trợ chẩn
   đoán trạng thái ASIC (ví dụ phân biệt ROM bootloader với Application firmware).
4. Giữ timeout lệnh 20 giây và không retry gói kích hoạt. Đo bằng rdtime 25 MHz;
   delay ngắn bảo đảm tối thiểu thời gian chờ, không dựa vào vTaskDelay(1) đơn thuần.
   Các định dạng `printf` dùng `%d`/`%x` (tránh `%u`/`%X`) để tương thích `vsnprintf`
   rút gọn của Cvitek RTOS.
5. Start kiểm tra mode/fps, stop mode trước, kiểm tra lỗi đổi SCK (`11.71875 MHz`
   mặc định cho ổn định dây nối thật) và bật Y16. Không tự gửi FFC hoặc thay tham
   số fall protection ở boot/start.
6. Kiểm tra `frame_valid`, tự quét offset header `64..480` trong gói 512 byte để
   thử offset 64 rồi lấy byte khác 0 đầu tiên khi bit valid tại 64 bằng 0, phù hợp chuỗi dummy quan sát trên module của Tiny1C (thực tế module trả 480 byte
   dummy + 32 byte header ở offset `480..511`), đồng thời từ chối header/payload
   toàn `0xFF`. `MODE_TEMP` dùng `0xAA`. `MODE_DUAL` giữ riêng metadata `0xAA`/`0xCC`
   và báo index không trùng qua app.
7. Tách task/IPC cũ khỏi build, lưu ở `Doc_md/Tiny-1C/Driver_T3/legacy_reference/*.txt`.
8. Thay bài demo PASS đơn thuần bằng 3 bộ test có assertion cho khởi tạo, lỗi I2C và tiêm lỗi VoSPI trong [`freertos/cvitek/driver/sensor/tiny1C/tests/`](../../../freertos/cvitek/driver/sensor/tiny1C/tests/test_driver.c).

## Cách chạy kiểm thử Tầng 3

Chạy toàn bộ host unit test và kiểm tra biên dịch chéo RISC-V (từ gốc repo):

```bash
make -C freertos/cvitek/driver/sensor/tiny1C clean test riscv-check
```

- **`build/test_driver`** ([`test_driver.c`](../../../freertos/cvitek/driver/sensor/tiny1C/tests/test_driver.c)): xác nhận `core_init(5, 0x78)` bị từ chối (`-22 / IR_I2C_DEVICE_OPEN_FAIL`) khi chưa xác nhận quyền sở hữu GPIOA; sau `data_rw_set_gpioa_ownership(true)`, kiểm tra `core_init`, probe thông tin thiết bị, `spi_stream_init(0, SPI0_DSI_CS_PIN_DEFAULT)`, `core_start(MODE_TEMP, 25)`, từ chối tham số sai (`mode=99`, `fps=0`, buffer `NULL`, palette `2` Reserved), thu khung đơn 96 KB, `core_start(MODE_DUAL, 25)` giữ SCK `THERMAL_SPI_DUAL_SCK_HZ` (`11,718,750 Hz`), thu khung kép 192 KB và `core_stop()`.
- **`build/test_i2c_failure`** ([`test_i2c_failure.c`](../../../freertos/cvitek/driver/sensor/tiny1C/tests/test_i2c_failure.c)): ghi đè weak `i2c_read`/`i2c_write` trả `-1`, xác nhận `core_init(5, 0x78)` trả `IR_NOT_GET_DEVICE_INFO`, `probed == false` và `core_start(MODE_TEMP, 25)` bị chặn.
- **`build/test_vospi`** ([`test_vospi.c`](../../../freertos/cvitek/driver/sensor/tiny1C/tests/test_vospi.c)): kiểm chứng 5 kịch bản tiêm lỗi (`inject=1` lỗi SPI header $\rightarrow$ `-2` + gọi `hal_spi_recover_bus()`; `inject=2` `frame_valid==0` $\rightarrow$ `-4` không đọc payload; `inject=3` lỗi SPI payload $\rightarrow$ `-3` + gọi `hal_spi_recover_bus()`; `inject=5` header toàn `0xFF` $\rightarrow$ `-2`; `inject=4` payload toàn `0xFF` $\rightarrow$ `-3`) và `inject=0` thu `MODE_DUAL` 192 KB qua 4 lần gọi `hal_spi_transfer_frame` (`0xAA` $\rightarrow$ 96 KB `0x55` $\rightarrow$ `0xCC` $\rightarrow$ 96 KB `0x55`) với thống kê `frames_invalid == 1`, `header_err == 2`, `payload_err == 2`, `recovered == 2`.
- **`riscv-check`**: biên dịch 6 tệp `src/*.c` và `rtos/thermal_service.c` bằng `riscv64-unknown-elf-gcc -std=gnu11 -O2 -Wall -Wextra -Werror`, kiểm tra ký hiệu `nm` cho `spi_transfer_one` (`hal_spi.o`), `soft_i2c_read_reg16` (`data_rw.o`), `xQueueSemaphoreTake` (`vdcmd.o`) và `T thermal_service_task` (`thermal_service.o`).

## Cách sử dụng

Service gọi lần lượt: xác nhận quyền tài nguyên → core_init → spi_stream_init →
core_start → spi_get_frame → core_stop. Một task điều phối toàn bộ lifecycle và
capture; không gọi core_start từ task khác trong lúc đang đọc frame.

T3 không tự cấp nguồn/reset GPIO cho module vì chưa có sơ đồ chân nguồn/reset
của carrier camera trong yêu cầu. Module cần được cấp nguồn và sẵn sàng trước START.

Các API hiệu chuẩn/Flash vẫn tồn tại nhưng không xuất qua viewer. Những thao tác
này cần quy trình riêng theo SDK và đúng firmware. tpd_set_rect_temp_thd trả
IR_UNSUPPORTED_CMD, không tự tạo opcode.

## Các điểm cần đối chiếu khi đọc mã

`core_init` chuẩn hóa địa chỉ và probe. `spi_stream_init` đi qua số chia 8 của adapter tương thích. `core_start` đặt SCK 11,71875 MHz cho cả mode đơn và kép. Build mặc định I²C 100 kHz, build board 18/11 dùng override 20 kHz. Mỗi cặp header/payload của VoSPI có hai phiên CS. Mutex thiết bị bao trọn chuỗi VDCMD; mutex Soft-I²C chỉ giữ từng transfer. [Driver T3 (freertos)](../../../freertos/cvitek/driver/sensor/tiny1C/README.md) · [Lộ trình đọc](../LO_TRINH_DOC_T3_APP.md)
