# Driver T3 camera nhiệt Tiny1C

> **Lưu ý đồng bộ (25/11/2026):** Mã nguồn chính thức của Driver T3 cùng dịch vụ RTOS (`thermal_service.c`, `thermal_protocol.h`) được biên dịch trực tiếp vào firmware FreeRTOS tại [`freertos/cvitek/driver/sensor/tiny1C/`](../../../freertos/cvitek/driver/sensor/tiny1C/README.md). Thư mục `Doc_md/Tiny-1C/Driver_T3/` giữ bản đối chiếu tham khảo.

T3 điều khiển ASIC Tiny1C bằng VDCMD qua Soft-I²C và thu mặt phẳng ảnh hoặc nhiệt bằng VoSPI qua SPI0. [Task RTOS (freertos)](../../../freertos/cvitek/driver/sensor/tiny1C/rtos/thermal_service.c) là caller chính, điều phối vòng đời và cung cấp buffer nhận ảnh. [Lộ trình đọc T3 → App](../LO_TRINH_DOC_T3_APP.md) lần theo một yêu cầu từ viewer đến cảm biến và đường trả dữ liệu.

## Mã nguồn theo thứ tự đọc

| Tệp | Trách nhiệm | Hàm nên tìm |
| --- | --- | --- |
| [core.c](src/core.c) | Probe, lifecycle, preview, FFC, TPD/ISP và hiệu chuẩn | `core_init`, `core_probe_device_info`, `core_start`, `core_stop`, `trigger_shutter` |
| [falcon_cmd.c](src/falcon_cmd.c) | Đóng gói từng lệnh ASIC, chọn nhóm/lệnh con và tham số | `get_device_info`, `preview_start`, `y16_preview_start`, `ooc_b_update` |
| [vdcmd.c](src/vdcmd.c) | Header short/standard/long 4/8/16 byte, xử lý vùng hold/RW và mutex lệnh tĩnh | `standard_cmd_read`, `standard_cmd_write`, `long_cmd_read` |
| [data_rw.c](src/data_rw.c) | Adapter Soft-I²C, chuẩn hóa địa chỉ 7 bit, polling status và chẩn đoán | `data_rw_i2c_hw_init`, `i2c_data_write`, `i2c_check_access_done`, `delay_ms` |
| [hal_spi.c](src/hal_spi.c) | Bind bus/device và chuyển truyền nhận xuống SPI T2 | `spi0_dsi_hw_init`, `hal_spi_set_freq_hz`, `hal_spi_transfer_frame` |
| [vospi.c](src/vospi.c) | Header VoSPI, kiểm tra frame, capture và thống kê | `spi_stream_init`, `spi_get_frame`, `vospi_read_single_frame` |

## Điều khiển qua I²C

Task xác nhận ownership rồi gọi `core_init(5, I2C_SLAVE_ID)`. ID 5 là bus logic trên GPIOA15/GPIOA27. Driver chuẩn hóa `0x78` thành `0x3C`. `core_init()` khởi tạo đường điều khiển, đăng ký VDCMD và probe thông tin. `probe_ok_mask` cho biết mục nào trả thành công. Nhánh probe cho phép tiếp tục với một số lỗi status ASIC, vì vậy cần đọc mask và log để biết mục còn thiếu.

Mutex thiết bị bảo vệ submit → poll → response, còn mutex Soft-I²C bảo vệ từng giao dịch. Ghi vùng hold `0x9D00` nạp dữ liệu. Đường kích hoạt dùng vùng `0x1D00` và poll `0x0200`. Deadline poll mặc định là 20 s, timeout mỗi giao dịch 1 s và giới hạn stretch 20 ms. Commit có thể đã tới camera khi đường truyền lỗi, do đó driver tránh phát lại commit. Tần số build mặc định 100 kHz. Bring-up 18/11 dùng override `THERMAL_I2C_FREQ_HZ=20000`. [Cấu hình](include/all_config.h) · [I²C T0–T2](../../I2C_Arch/README.md)

## Thu ảnh qua SPI0

`spi_stream_init()` chuẩn bị transport. Adapter dùng mode 3, word 8 bit, MSB-first, CS GPIOC17 và `rx_sample_dly=1`. Đường cấu hình divider tương thích lúc init có thể log 23,4375 MHz. `core_start()` đặt SCK cho cả mode đơn và kép về 11,71875 MHz theo cấu hình hiện tại. Backend mặc định polling. Task khởi động preview rồi gọi `spi_get_frame()` theo từng yêu cầu CAPTURE.

Mỗi mặt phẳng dùng hai phiên SPI. Phiên đầu trao đổi 512 byte, tìm header 32 byte tại offset 64 hoặc theo heuristic đến 480 và kiểm tra bit valid. Sau khi nhả CS, driver chờ 1 ms rồi phát byte điền `0x55` cho phiên nhận 98.304 byte. Gói header hoặc payload toàn `0xFF` được xem là lỗi. Ở mode đơn, `0xAA` lấy YUYV hoặc Y16 tùy cấu hình preview. DUAL ghép YUYV `0xAA` và Y16 `0xCC`; service gắn cờ nếu hai index khác nhau. [SPI T0–T2](../../SPI_Arch/spi_driver_architecture_and_roadmap.md) · [Bring-up thực tế](../Implementation/09_board_bringup_2026-11-18.md)

```bash
make -C freertos/cvitek/driver/sensor/tiny1C test riscv-check
```

Lần đối chiếu 25/11/2026, ba bài test host và compile RISC-V (bao gồm `thermal_service.c`) từ nguồn độc lập trong `freertos` đều đạt. `riscv-check` kiểm tra compile cùng các symbol T2 và mutex RTOS. Build firmware đầy đủ theo [hướng dẫn build và deploy](../Implementation/05_build_deploy.md).
