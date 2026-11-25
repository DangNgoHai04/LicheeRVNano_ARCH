# Đối chiếu mã nguồn T2 SPI — RTOS SPI bus/device

Đối chiếu ngày **25-11-2026** cho LicheeRV-Nano (SG2002/CV181x). [Bản kiến trúc](spi_t2_driver_architecture.md) · [README SPI T2](../../../freertos/cvitek/driver/spi/README.md) · [Driver T3](../../../freertos/cvitek/driver/sensor/tiny1C/README.md) · [Mã nguồn chính](../../../freertos/cvitek/driver/spi/src/spi.c) · [Bộ test host](../../../freertos/cvitek/hal/cv181x/spi/tests/run.sh)

## Hợp đồng đọc từ mã nguồn

| Điểm đối chiếu | Hành vi trong source |
| --- | --- |
| `spi_bus_init` / `spi_device_init` | Khởi tạo bus và device do caller cấp vùng nhớ. Yêu cầu quyền DMA khi bật DMA. |
| `select_segment_backend` | AUTO ưu tiên DMA rồi IRQ rồi polling, theo điều kiện enable và ngưỡng mặc định 64/32 word. |
| `spi_transfer` | Kiểm tra mọi segment, khóa bus, chuẩn bị engine, nạp cấu hình khi cần, giữ CS theo message với tùy chọn `cs_change`. |
| `spi_bus_recover` | Lấy khóa, đưa HAL về trạng thái phục hồi và xóa device đang active. Lỗi giao dịch không kích hoạt gửi lại message. |

### Hai phiên CS và giới hạn timeout ở Tiny1C

T3 gọi `spi_transfer_one()` riêng cho header 512 byte và payload 98.304 byte. T2 giữ mutex và CS active-low trong từng lời gọi. Khoảng đợi 1 ms giữa hai lần gọi diễn ra sau khi CS đã nhả. Dual lặp hai cặp giao dịch cho `0xAA` và `0xCC`. Task camera tuần tự hóa toàn bộ capture cùng START, STOP và FFC.

`timeout_ms` trong `spi_transfer()` dùng khi đợi mutex rồi được truyền nguyên giá trị cho từng engine của từng segment. Polling thực thi ngân sách theo số vòng `compute_poll_budget()`, IRQ/DMA có đường chờ riêng ở HAL. Tổng thời gian message còn phụ thuộc số segment, chuẩn bị backend và delay. Khi prepare IRQ/DMA thất bại, T2 trả lỗi trước khi kéo CS, không tự chọn lại polling. [Lộ trình T3 → App](../../Tiny-1C/LO_TRINH_DOC_T3_APP.md)
