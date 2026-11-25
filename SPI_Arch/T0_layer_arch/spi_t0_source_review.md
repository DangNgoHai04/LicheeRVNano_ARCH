# Đối chiếu mã nguồn T0 SPI — Platform SPI0

Đối chiếu ngày **25-11-2026** cho LicheeRV-Nano (SG2002/CV181x). [Bản kiến trúc](spi_t0_platform_architecture.md) · [README SPI HAL](../../../freertos/cvitek/hal/cv181x/spi/README.md) · [Mã nguồn chính](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_io.c) · [Bộ test host](../../../freertos/cvitek/hal/cv181x/spi/tests/run.sh)

## Hợp đồng đọc từ mã nguồn

| Điểm đối chiếu | Hành vi trong source |
| --- | --- |
| Clock và reset | Khởi tạo SSI0 theo base hợp lệ, bật đường clock SPI/APB và reset IP. Ref clock mặc định 187,5 MHz theo cấu hình FPLL/8. |
| D-PHY và pinmux | `hal_spi_platform_pinmux_init` đưa FPC2 qua đường digital, kiểm tra cấu hình pad, mux và trạng thái GPIO CS. |
| GPIO CS | GPIOC17 active-low, được T2 điều khiển theo giao dịch. |
| PLIC và DMA | `hal_spi_platform.c` cung cấp hook cho backend được chuẩn bị khi cần. DMA có điều kiện ownership và thao tác cache. |

### Quyền sở hữu SPI0 trên image camera

FreeRTOS dùng SSI0 tại `0x04180000`, FPC2.14 (CS0/GPIOC17), FPC2.15 (SCK), FPC2.18 (MOSI) và FPC2.17 (MISO). Kiểm tra cả quyền dùng D-PHY, pad, clock/reset và GPIO. Cờ `--resources-owned` chuyển cam kết của người triển khai đến START. Nó không chỉnh Device Tree hay gỡ driver Linux.

Lần bring-up 18/11 đã xác nhận node SPI0 trên DT đang chạy là `disabled` và Linux không bind `dw_spi_mmio` vào `4180000.spi0`. Khi đổi DTS, bản FIT thuần tại `rawimages/boot.sd` mới là artifact được U-Boot trên thẻ SD này đọc trực tiếp. `fip.bin` chứa firmware RTOS. Hai artifact có vòng đời triển khai riêng. [Nhật ký board](../../Tiny-1C/Implementation/09_board_bringup_2026-11-18.md)
