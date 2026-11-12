# Đánh giá Source và Trạng thái Hoàn thiện của Tầng 0 SPI0

Cập nhật: **30-10-2026**. Board mục tiêu: **LicheeRV-Nano, SPI0 qua cổng FPC2**.

**Kết luận kiểm tra source:** Tầng 0 đã được chuẩn hóa và tách thành hai mô-đun rõ ràng (`hal_spi_platform_io.c` cho Clock/Reset/D-PHY/Pinmux/CS và `hal_spi_platform.c` cho PLIC IRQ/System DMA/D-Cache). Toàn bộ các sai lệch địa chỉ thanh ghi Clock, Reset, D-PHY, Pinmux FPC2 và thứ tự khởi tạo GPIO CS từ đợt review trước đã được khắc phục trong mã nguồn và biên dịch sạch (`0 warning, 0 error`) với toolchain RISC-V 64-bit.

---

## 1. Phạm vi và Nguồn đối chiếu

- **Source Tầng 0**:
  - [`hal_spi_platform_io.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_io.c) (Clock tree readback, Clock gate, Soft-reset, MIPI DSI D-PHY, PADCTRL, FMUX FPC2, GPIO CS).
  - [`hal_spi_platform.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform.c) (PLIC IRQ + Semaphore, System DMA `DW_axi_dmac` + D-Cache 64B).
  - [`hal_spi_platform_limits.h`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_limits.h) và [`hal_dw_spi.h`](../../../freertos/cvitek/hal/cv181x/spi/include/hal_dw_spi.h).
- **Tài liệu phần cứng đối chiếu**:
  - [TRM SG2002 v1.02](../../HW/TRM/TRM_v102.md): Chương Clock (`0x03002000`), Reset (`0x03003000`), System Controller DMA Remap (`0x03000154`), DW SSI (`0x04180000`).
  - [SDK Pinmux Register](../../../freertos/cvitek/hal/cv181x/config/cv181x_reg_fmux_gpio.h) và [Function Select](../../../freertos/cvitek/hal/cv181x/config/cv181x_pinlist_swconfig.h).
  - [Schematic LicheeRV-Nano](../../HW/Schematic/LicheeRV_Nano_70415_Schematic_AI.md): §9.1 FPC2, bảng chân U1.

---

## 2. Bảng Đối chiếu Các Hạng mục Đã Khắc phục ở Tầng 0

| Hạng mục | Vấn đề ở bản cũ | Giải pháp đã chốt trong mã nguồn hiện tại | File thực thi |
| :--- | :--- | :--- | :--- |
| **1. Clock Gate SPI0** | Bật nhầm `0x03002004` bit 16 (`clk_uart1`), thiếu bật `clk_spi`. | Mở đồng thời `clk_apb_gpio` (`0x03002000` bit 29), `clk_apb_spi0` (`0x03002004` bit 9) và `clk_spi` (`0x0300200C` bit 6) kèm readback xác nhận. | [`hal_spi_platform_io.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_io.c) |
| **2. Đọc tần số `clk_spi`** | Trả cứng hằng số `187.5 MHz`, không đọc thanh ghi phần cứng. | Đọc `CLK_BYP_0` (`0x03002030` bit 30: nếu `1` trả `25 MHz`) và `DIV_CLK_SPI` (`0x03002100`: kiểm tra bit 0 reset, bit 3 chọn divider, tính `1500 MHz / div`). Trả `0` nếu bộ chia đang bị giữ reset hoặc hệ số chia bằng `0`. | [`hal_spi_platform_io.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_io.c) |
| **3. Soft-Reset SPI0** | Ghi nhầm `0x03003000` bit 16 (`reg_soft_reset_x_sd0`). | Phát xung reset tích cực mức thấp đúng tại `SOFT_RSTN_1` (`0x03003004` bit 8 — `reg_soft_reset_x_spi0`) và readback kiểm tra đã nhả reset. | [`hal_spi_platform_io.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_io.c) |
| **4. D-PHY & Pinmux FPC2** | Ghi nhầm CSI RX D-PHY (`0x0A0D0000`) và dùng offset mux của SD1/SPI2. | Mở khóa Digital GPO/GPI (`0x00001F1F`) trên **MIPI DSI D-PHY (`0x0A0D1000`)**, đặt `PADCTRL` (`0x03001C70..7C = 0x44`), ánh xạ đúng `FMUX` (`0x030011A4` = 6, `0x030011A8` = 3 `XGPIOC_17`, `0x030011AC` = 6, `0x030011B0` = 6) và readback toàn bộ. | [`hal_spi_platform_io.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_io.c) |
| **5. Thứ tự khởi tạo CS** | Gọi `pinmux_init()` trước `cs_init()`, có nguy cơ tạo xung chọn giả. | `hal_spi_init()` gọi `hal_spi_platform_cs_init()` (nạp `DR = 1` trước, `DDR = 1` sau) **trước khi** gọi `hal_spi_platform_pinmux_init()`. | [`hal_dw_spi.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_dw_spi.c) & [`hal_spi_platform_io.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_io.c) |
| **6. Trần tốc độ SCK** | Chưa áp trần tốc độ phần cứng của SoC. | Bổ sung `HAL_SPI_MAX_SCK_HZ = 46875000U` (`46.875 MHz` theo TRM §21.3.2) trong `hal_spi_platform_limits.h` và ép trần trong `hal_spi_configure()`. | [`hal_spi_platform_limits.h`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_limits.h) & [`hal_dw_spi.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_dw_spi.c) |
| **7. Khởi tạo Lazy IRQ/DMA & Quyền sở hữu DMAC** | `hal_spi_init()` tự động đăng ký PLIC và ghi đè bảng remap DMAC ngay lúc khởi tạo bus. | Tách thành `hal_spi_prepare_irq()` và `hal_spi_prepare_dma()`. DMA yêu cầu cờ `dma_resources_reserved = true` và kiểm tra toàn bộ kênh DMAC đang rảnh (`CHEN_L & 0xFF == 0`) trước khi ánh xạ kênh. | [`hal_spi_platform.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform.c) & [`spi.c`](../../../freertos/cvitek/driver/spi/src/spi.c) |
| **8. Vệ sinh Semaphore & D-Cache khi lỗi** | Chưa xả token cũ khi init lại IRQ; bỏ qua invalidate RX khi khối DMA lỗi giữa chừng. | Bổ sung vòng xả token cũ trong `hal_spi_platform_irq_init()`, kiểm tra tràn kích thước/căn lề 16-bit và luôn `platform_dcache_invalidate_rx()` cả khi khối DMA gặp lỗi. | [`hal_spi_platform.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform.c) |

---

## 3. Các Lưu ý Khi Tích hợp Hệ thống (Board Integration Notes)

1. **Quyền sở hữu độc quyền tài nguyên dùng chung giữa Linux và FreeRTOS**:
   - Đảm bảo node `&spi0` trong Device Tree của Linux được đặt `status = "disabled";` khi FreeRTOS sở hữu `SPI0`.
   - Nếu bật `enable_dma = true` và `dma_resources_reserved = true`, cấu hình DTS của Linux không được sử dụng đồng thời Kênh 0 và Kênh 1 của `sysdma` (`0x04330000`).
2. **Đo kiểm thực tế trên phần cứng (Hardware Bring-up)**:
   - Khi chạy với tần số cao (`clk_spi = 187.5 MHz`), cần đo kiểm dạng sóng thực tế trên cổng FPC2 và tinh chỉnh tham số `rx_sample_dly` (`0..8`) theo độ trễ đường truyền của mạch đích.
