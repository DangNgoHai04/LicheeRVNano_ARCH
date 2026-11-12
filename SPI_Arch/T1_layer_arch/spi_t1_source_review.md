# Đánh giá Source và Trạng thái Hoàn thiện của Tầng 1 (Controller HAL) SPI0

Cập nhật: **09-11-2026**. Board mục tiêu: **LicheeRV-Nano (SOPHGO SG2002 / CV181x — Khối IP DW APB SSI0)**.

**Kết luận kiểm tra source:** Tầng 1 đã được chuẩn hóa và hoàn thiện đạt chuẩn sản phẩm (Production-ready). Các cơ chế an toàn phần cứng (trần tốc độ SCK 46.875 MHz, bất biến chống tràn FIFO 8 khung, chuẩn bị lazy backend độc lập, bounce-buffer LSB-first cho DMA và tự phục hồi cấu hình trong abort) đã được triển khai đầy đủ và kiểm chứng sạch (`0 error, 0 warning`) trên cả trình biên dịch RISC-V 64-bit và bộ test host.

---

## 1. Phạm vi và Nguồn đối chiếu

- **Source Tầng 1**:
  - [`hal_dw_spi.c`](../../../freertos/cvitek/hal/cv181x/spi/src/hal_dw_spi.c): Quản lý máy trạng thái DW APB SSI, tính toán BAUDR, 3 backend engines (`POLL`, `IRQ`, `DMA`), snapshot và abort.
  - [`hal_dw_spi.h`](../../../freertos/cvitek/hal/cv181x/spi/include/hal_dw_spi.h): Định nghĩa `hal_spi_t`, `hal_spi_config_t`, `hal_spi_snapshot_t` và 12 nguyên mẫu hàm T1.
  - [`dw_spi_regs.h`](../../../freertos/cvitek/hal/cv181x/spi/src/dw_spi_regs.h): Offset thanh ghi và bitfield chuẩn Synopsys DW APB SSI.
- **Tài liệu phần cứng đối chiếu**:
  - [TRM SG2002 v1.02](../../HW/TRM/TRM_v102.md): §21.3 Synopsys DW APB SSI (Giới hạn SCK 46.875 MHz, cấu trúc FIFO 8x16-bit, thanh ghi điều khiển DMA và ngắt).
  - [Tài liệu Kiến trúc T0](../T0_layer_arch/spi_t0_platform_architecture.md): 12 Platform Hooks kết nối với SoC.

---

## 2. Bảng Đối chiếu Các Hạng mục Kỹ thuật Đã Hoàn thiện ở Tầng 1

| Hạng mục | Cơ chế thực thi trong Source Code | Đánh giá & Trạng thái |
| :--- | :--- | :--- |
| **1. Trần tốc độ SCK (46.875 MHz)** | `compute_even_divider()` kiểm tra `requested_sck_hz > HAL_SPI_MAX_SCK_HZ (46875000U)` và từ chối ngay với `HAL_SPI_ERR_INVALID`. Làm tròn số chia lên số chẵn gần nhất (`div & ~1`). | **HOÀN THÀNH:** Triệt để tuân thủ giới hạn vật lý của TRM §21.3.2, bảo vệ đường truyền trên FPC2. |
| **2. Bất biến Chống tràn FIFO (`POLL`)** | `fill_tx_fifo()` chỉ nạp khi `(tx_done - rx_done) < HAL_SPI_FIFO_DEPTH (8)`. Luôn `drain_rx_fifo()` trước khi bơm tiếp TX. | **HOÀN THÀNH:** Ngăn chặn tuyệt đối lỗi tràn RX FIFO (`RXOI`) khi CPU C906L bị Task khác chiếm quyền (preempt). |
| **3. Ngưỡng ngắt động (`IRQ`)** | `update_irq_thresholds_and_mask()` tự động hạ `RXFTLR = rem_rx - 1` khi số khung còn lại $\le 4$. | **HOÀN THÀNH:** Đảm bảo khung dữ liệu cuối cùng rơi vào RX FIFO luôn kích hoạt được ngắt `RXFI` để gọi `irq_notify()`, không bị treo chờ ngắt. |
| **4. Khởi tạo Trễ (Lazy Prepare)** | Cung cấp `hal_spi_prepare_irq()` và `hal_spi_prepare_dma()`. Chỉ gọi khi gói tin thực sự cần và chỉ kích hoạt khi đã kiểm tra `resources_reserved`. | **HOÀN THÀNH:** Tránh chiếm dụng tài nguyên ngắt và System DMA của SoC khi chỉ chạy Polling. |
| **5. Chuẩn LSB-First qua DMA** | Sử dụng Stack bounce buffer 128 byte (`uint16_t tx_bounce[64]`) để đảo bit theo từng cụm $\le 64$ khung, đảo bit RX sau completion. | **HOÀN THÀNH:** Bảo toàn tính bất biến cho bộ đệm hằng (`const void *tx`) của người dùng, không gây side-effect bộ nhớ. |
| **6. Độ trễ lấy mẫu (`rx_sample_dly`)** | Nhận giá trị cấu hình `0..8` chu kỳ xung nhịp và nạp vào thanh ghi `DW_SPI_RX_SAMPLE_DLY (0xF0)`. | **HOÀN THÀNH:** Hỗ trợ bù trễ đường dây cho các cảm biến tốc độ cao trên cổng FPC2. |
| **7. Thu hồi & Tự khôi phục (`hal_spi_abort`)** | Dừng DMA nếu `dma_ready`, phát xung Soft-Reset T0, kéo CS lên HIGH, xóa sạch state và tự động gọi lại `hal_spi_configure()` với cấu hình đã lưu. | **HOÀN THÀNH:** Đưa controller về trạng thái sẵn sàng ngay lập tức cho các giao dịch tiếp theo mà không cần khởi tạo lại bus từ đầu. |
| **8. Chẩn đoán (`hal_spi_snapshot`)** | Chụp lại đồng thời `CTRLR0`, `BAUDR`, `SR`, `RISR`, `TXFLR`, `RXFLR`, `IMR`, `DMACR`, `RX_SAMPLE_DLY` mà không xóa cờ lỗi. | **HOÀN THÀNH:** Cung cấp đầy đủ thông tin cho T2 chụp lại trạng thái lỗi trước khi cleanup. |

---

## 3. Trạng thái Kiểm chứng Kỹ thuật

- **Host Unit Test:** Đã chạy `bash freertos/cvitek/hal/cv181x/spi/tests/run.sh` $\rightarrow$ **PASS 100%**.
- **Biên dịch chéo RISC-V 64-bit:** Đã chạy `cmake --build cvitek/build/hal --target install` $\rightarrow$ **0 error, 0 warning**.
- **Kết luận:** Tầng 1 đã hoàn thiện đầy đủ cả về mã nguồn lẫn tài liệu kiến trúc, sẵn sàng làm nền tảng vững chắc cho Tầng 2 và các driver cảm biến phía trên.
