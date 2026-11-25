# Kiến trúc SPI0 trên SG2002 và thứ tự đọc

Đối chiếu ngày **25-11-2026**. Driver chung phục vụ SPI0 master trên FreeRTOS C906L, đường FPC2 của LicheeRV-Nano. Đọc từ hợp đồng T2 xuống HAL và platform khi truy vết một lời gọi.

| Tầng | Trách nhiệm | Tài liệu |
| --- | --- | --- |
| T0 | Clock, reset, D-PHY digital bypass, pad/mux, GPIOC17 CS, PLIC, DMAC và cache | [Kiến trúc](T0_layer_arch/spi_t0_platform_architecture.md) · [Đối chiếu source](T0_layer_arch/spi_t0_source_review.md) · [README freertos](../../freertos/cvitek/hal/cv181x/spi/README.md) |
| T1 | Cấu hình DW SSI, divider, FIFO, polling/IRQ/DMA, finish và abort | [Kiến trúc](T1_layer_arch/spi_t1_controller_architecture.md) · [Đối chiếu source](T1_layer_arch/spi_t1_source_review.md) · [README freertos](../../freertos/cvitek/hal/cv181x/spi/README.md) |
| T2 | API bus/device, khóa task, segment, CS, chính sách backend và snapshot | [Kiến trúc](T2_layer_arch/spi_t2_driver_architecture.md) · [Đối chiếu source](T2_layer_arch/spi_t2_source_review.md) · [README freertos](../../freertos/cvitek/driver/spi/README.md) |
| T3 | Adapter VoSPI & tích hợp camera nhiệt Tiny1C | [Driver Tiny1C (freertos)](../../freertos/cvitek/driver/sensor/tiny1C/README.md) |

## Luồng từ API tới bộ điều khiển

Caller cấp `spi_bus_t` và cấu hình rồi gọi `spi_bus_init(bus, cfg)`. T2 tạo mutex tĩnh và gọi `hal_spi_init`, hàm này dùng hook T0 chuẩn bị SSI0. Caller gắn `spi_device_t` bằng `spi_device_init`; public API ở [spi.h](../../freertos/cvitek/driver/spi/include/spi.h).

Một `spi_transfer` kiểm tra toàn bộ mảng segment, lấy mutex, chuẩn bị backend cần dùng trước CS và nạp cấu hình thiết bị khi cần. Mỗi segment thực thi bằng polling, IRQ hoặc DMA. AUTO ưu tiên DMA, rồi IRQ, rồi polling theo quyền tài nguyên và ngưỡng đã cấu hình. CS thường giữ giữa các segment và có thể đổi bằng `cs_change`. TX trống phát `fill_word`; RX trống vẫn phải giải phóng dữ liệu từ FIFO. Trên lỗi T2 snapshot, gọi HAL abort và trả mã lỗi. [Khởi tạo backend](spi_backend_initialization.md)

SPI0 có một CS GPIO active-low trong driver này. Mode 0–3, word 4–16 bit và LSB-first nằm trong hợp đồng chung. Bộ đệm dùng phần tử 8 bit cho word ≤8 bit và 16 bit cho word >8 bit. Với `ref_clock_hz=0`, HAL lấy ref clock từ platform. Platform đọc mux bypass và divider hiện có. XTAL là 25 MHz, còn FPLL/8 cho 187,5 MHz theo FPLL khai báo 1,5 GHz. T1 làm tròn BAUDR lên số chẵn và giới hạn SCK 46,875 MHz. SCK tính từ divider cần được đối chiếu với phép đo nếu dùng làm tiêu chí timing.

## Đường Tiny1C trên board

Đường VoSPI dùng mode 3, word 8 bit, RX sample delay 1 và polling mặc định. Header 512 byte và payload 98.304 byte dùng hai `spi_transfer_one` riêng. Khi preview chạy, T3 yêu cầu 11,71875 MHz; con số 23,4375 MHz có thể xuất hiện trong log khởi tạo. Linux DT cần nhường SSI0, FPC2/D-PHY và các tài nguyên liên quan cho FreeRTOS trước START. Khi cập nhật ownership qua DTS, xác minh DT của bản FIT đang chạy. [Bring-up SPI0 và VoSPI](../Tiny-1C/Implementation/09_board_bringup_2026-11-18.md)

## Xác minh

```bash
bash freertos/cvitek/hal/cv181x/spi/tests/run.sh
```

Bộ mock host ngày 20/11 đạt các nhánh POLL, IRQ, DMA, AUTO, LSB-first, helper và recovery. Nhật ký board xác nhận tuyến Tiny1C chạy polling cho cả ba mode. Kiểm chứng IRQ/DMA trên board cần xác nhận PLIC, request remap, kênh DMA, cache và tải thực. Để tiếp tục từ device adapter đến viewer, đọc [lộ trình T3 → App](../Tiny-1C/LO_TRINH_DOC_T3_APP.md).
