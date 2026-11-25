# Đối chiếu mã nguồn T1 SPI — DW SSI controller HAL

Đối chiếu ngày **25-11-2026** cho LicheeRV-Nano (SG2002/CV181x). [Bản kiến trúc](spi_t1_controller_architecture.md) · [README SPI HAL](../../../freertos/cvitek/hal/cv181x/spi/README.md) · [Mã nguồn chính](../../../freertos/cvitek/hal/cv181x/spi/src/hal_dw_spi.c) · [Bộ test host](../../../freertos/cvitek/hal/cv181x/spi/tests/run.sh)

## Hợp đồng đọc từ mã nguồn

| Điểm đối chiếu | Hành vi trong source |
| --- | --- |
| `hal_spi_configure` | Cấu hình mode 0–3, word 4–16 bit, divider chẵn, trần SCK 46,875 MHz và độ trễ lấy mẫu. |
| `hal_spi_begin` / `hal_spi_poll` | Bắt đầu segment, drain RX và giới hạn số word đang bay theo độ sâu FIFO 8. |
| `hal_spi_prepare_irq` / `hal_spi_prepare_dma` | Backend được chuẩn bị độc lập trước CS từ T2. DMA dài chia block, LSB-first dùng xử lý bit phần mềm. |
| `hal_spi_finish` / `hal_spi_abort` | Kết thúc segment thành công hoặc dọn khi lỗi. T2 snapshot trước abort để giữ dấu vết. |

### Từ khung SSI đến gói VoSPI

`frame` trong HAL nghĩa là một SPI word. Tiny1C dùng word 8 bit, mode 3, MSB-first, `RX_SAMPLE_DLY=1`. Một gói header VoSPI 512 byte tương ứng 512 SPI word. Một lượt nhận payload 98.304 byte tương ứng 98.304 word và tạo clock bằng `fill_word=0x55`. T1 bảo vệ giới hạn FIFO của SSI và thực thi segment. T3 đọc `frame_valid`, metadata và kiểm tra nội dung.

Với ref clock khai báo 187,5 MHz, bring-up dùng `BAUDR=16` khi stream, tương đương SCK tính toán 11,71875 MHz. Trần 46,875 MHz của HAL là giới hạn cấu hình chung. Host test xác nhận các nhánh polling, IRQ và DMA qua mock. Nhật ký board xác nhận capture Tiny1C ở polling; chất lượng tín hiệu ở các backend/tốc độ khác cần phép đo riêng. [Driver T3](../../../freertos/cvitek/driver/sensor/tiny1C/README.md)
