# Đánh giá Source và Trạng thái Hoàn thiện của Tầng 2 (RTOS SPI Driver Layer)

Cập nhật: **09-11-2026**. Nền tảng mục tiêu: **LicheeRV-Nano (SOPHGO SG2002 / CV181x — FreeRTOS trên nhân RISC-V C906L)**.

**Kết luận kiểm tra source:** Tầng 2 đã được chuẩn hóa và hoàn thiện đạt chuẩn chất lượng sản xuất (Production-ready). Các cơ chế điều phối RTOS (Static Mutex không cấp phát heap, giao dịch nguyên tử đa phân đoạn, điều khiển chính xác chân Chip-Select GPIOC17, Lazy Reconfiguration chống ghi đè thanh ghi, tự động chọn backend AUTO theo kích thước gói, và cơ chế tự phục hồi lỗi phần cứng) đã được triển khai đầy đủ và kiểm chứng sạch (`0 error, 0 warning`) trên cả trình biên dịch RISC-V 64-bit và bộ test host.

---

## 1. Phạm vi và Nguồn đối chiếu

- **Source Tầng 2**:
  - [`spi.c`](../../../freertos/cvitek/driver/spi/src/spi.c): Điều phối giao dịch đa phân đoạn, quản lý Mutex, chọn backend thực thi, điều khiển CS, snapshot và tự phục hồi sau sự cố.
  - [`spi.h`](../../../freertos/cvitek/driver/spi/include/spi.h): Khai báo các cấu trúc `spi_bus_t`, `spi_device_t`, `spi_segment_t`, `spi_bus_config_t`, `spi_device_config_t` và hệ thống Public API cho các driver cảm biến.
  - [`CMakeLists.txt`](../../../freertos/cvitek/driver/spi/CMakeLists.txt): Cấu hình biên dịch module driver SPI thành static library `libdriver.a`.
- **Source Tầng Dưới đối chiếu**:
  - [Tài liệu Kiến trúc T1](../T1_layer_arch/spi_t1_controller_architecture.md): Giao diện điều khiển IP Synopsys DW APB SSI (`hal_dw_spi.c`).
  - [Tài liệu Kiến trúc T0](../T0_layer_arch/spi_t0_platform_architecture.md): 12 Platform Hooks kết nối với phần cứng SoC.

---

## 2. Bảng Đối chiếu Các Hạng mục Kỹ thuật Đã Hoàn thiện ở Tầng 2

| Hạng mục | Cơ chế thực thi trong Source Code | Đánh giá & Trạng thái |
| :--- | :--- | :--- |
| **1. Static Mutex (Thread-Safety)** | Sử dụng `xSemaphoreCreateMutexStatic(&bus->mutex_storage)` trong `spi_bus_init()`. Vùng nhớ tĩnh nằm trọn trong `spi_bus_t`. | **HOÀN THÀNH:** 100% không dùng heap động, loại trừ nguy cơ phân mảnh bộ nhớ và lỗi cấp phát bộ nhớ trong môi trường RTOS nhúng. |
| **2. Giao dịch Đa phân đoạn (Multi-segment)** | `spi_transfer()` duyệt mảng `spi_segment_t`, thực thi liên tiếp các phân đoạn trong cùng một chu kỳ giữ chân CS ở mức LOW. | **HOÀN THÀNH:** Hỗ trợ đầy đủ các mô hình truyền phức tạp như Lệnh ghi + Delay + Đọc ảnh. |
| **3. Giữ & Nhả chân CS (Chip-Select)** | Kéo `CS = LOW` trước phân đoạn đầu tiên và chỉ kéo `CS = HIGH` sau khi phân đoạn cuối cùng hoàn tất và bus đã idle. Hỗ trợ `cs_change` toggle CS $2\ \mu\text{s}$. | **HOÀN THÀNH:** Đảm bảo toàn vẹn dạng sóng điện học, ngăn ngừa hiện tượng cắt cụt khung dữ liệu của slave. |
| **4. Lazy Reconfiguration** | So sánh cấu hình `hal_configs_equal()` giữa `bus->active_dev` và `dev`. Bỏ qua việc nạp lại thanh ghi nếu thiết bị và tham số không đổi. | **HOÀN THÀNH:** Tiết kiệm chu kỳ bus APB, tối ưu hóa thông lượng khi đọc streaming liên tục từ cùng một cảm biến. |
| **5. Điều phối Backend Thông minh (AUTO)** | `select_segment_backend()` tự động phân loại: `< 32` khung $\rightarrow$ `POLL`, `32..63` khung $\rightarrow$ `IRQ`, $\ge 64$ khung $\rightarrow$ `DMA`. | **HOÀN THÀNH:** Đạt độ trễ thấp cho gói nhỏ và thông lượng tối đa cho dữ liệu lớn mà không bắt sensor driver phải tự tính toán. |
| **6. Xác thực Quyền trước CS (Pre-CS Check)** | Duyệt toàn bộ phân đoạn và gọi `hal_spi_prepare_irq()` / `hal_spi_prepare_dma()` trước khi kéo CS xuống LOW. Nếu lỗi, từ chối ngay. | **HOÀN THÀNH:** Ngăn chặn việc làm nháy chân CS ra ngoài khi tài nguyên hệ thống chưa sẵn sàng. |
| **7. Quản lý Deadline & Non-blocking Polling** | `compute_poll_budget()` tính toán ngân sách vòng lặp dựa trên số khung và `timeout_ms`. Ngăn chặn vòng lặp quay vô tận. | **HOÀN THÀNH:** Bảo vệ hệ thống RTOS không bị treo cứng (hang) khi slave không phản hồi. |
| **8. Tự Phục Hồi Lỗi (Self-Healing)** | Khi gặp lỗi FIFO hoặc timeout: tự động chụp `hal_spi_snapshot()`, gọi `hal_spi_abort()`, kéo CS lên HIGH và nhả Mutex. | **HOÀN THÀNH:** Phục hồi bus về trạng thái sẵn sàng ngay lập tức cho các giao dịch sau mà không cần reboot board. |
| **9. Hệ thống Helper API Tiện ích** | Cung cấp `spi_transfer_one()`, `spi_write_then_read()`, `spi_write()`, `spi_read()`. | **HOÀN THÀNH:** Thân thiện, dễ dùng cho các kỹ sư phát triển Driver Cảm biến ở Tầng 3. |

---
