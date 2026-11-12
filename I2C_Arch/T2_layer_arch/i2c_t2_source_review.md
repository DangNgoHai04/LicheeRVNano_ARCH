# Đánh giá Source và Trạng thái Hoàn thiện của Tầng 2 (RTOS Soft-I²C Bus Driver Layer)

Cập nhật: **09-11-2026**. Nền tảng mục tiêu: **LicheeRV-Nano (SOPHGO SG2002 / CV181x — FreeRTOS trên nhân RISC-V C906L)**.

**Kết luận kiểm tra source:** Tầng 2 (`soft_i2c.c` và `soft_i2c.h`) đã được chuẩn hóa hoàn chỉnh, cung cấp đầy đủ cơ chế khóa tương tranh bằng `StaticSemaphore_t` (không dùng Heap), quản lý ngân sách thời gian thực kết hợp giữa chờ Mutex và truyền nhận trên dây, cùng bộ hàm `soft_i2c_read_reg16()` / `soft_i2c_write_reg16()` khớp tuyệt đối với giao thức CCI / VDCMD của cảm biến nhiệt Tiny1C. Mã nguồn đã vượt qua toàn bộ bài test trên Host và biên dịch sạch (`0 warning, 0 error`) với trình biên dịch RISC-V 64-bit.

---

## 1. Phạm vi và Nguồn đối chiếu

- **Source Tầng 2**:
  - [`soft_i2c.c`](../../../freertos/cvitek/driver/i2c/src/soft_i2c.c): Quản lý khóa FreeRTOS Static Mutex, quy đổi ngân sách thời gian, thống kê giao dịch và triển khai các hàm helper thanh ghi 16-bit.
  - [`soft_i2c.h`](../../../freertos/cvitek/driver/i2c/include/soft_i2c.h): Khai báo cấu trúc `soft_i2c_bus_t` và 5 hàm Public API.
  - [`CMakeLists.txt`](../../../freertos/cvitek/driver/i2c/CMakeLists.txt): Biên dịch tự động `soft_i2c.c` vào thư viện `libdriver.a` và cài đặt header vào `install/include/driver/i2c/`.
- **Source Tầng Dưới đối chiếu**:
  - [Tài liệu Kiến trúc T1](../T1_layer_arch/i2c_t1_controller_architecture.md): Bộ máy trạng thái I²C Bitbang (`hal_i2c_bitbang.c`).
  - [Tài liệu Kiến trúc T0](../T0_layer_arch/i2c_t0_platform_architecture.md): Lớp nền tảng GPIOA (`hal_i2c_gpio_platform.c`).

---

## 2. Bảng Đối chiếu Các Hạng mục Kỹ thuật Đã Hoàn thiện ở Tầng 2

| Hạng mục | Cơ chế thực thi trong Source Code | Đánh giá & Trạng thái |
| :--- | :--- | :--- |
| **1. Static Mutex Không Dùng Heap** | Sử dụng `xSemaphoreCreateMutexStatic(&b->lock_storage)` trong `soft_i2c_init()`. | **HOÀN THÀNH:** Loại trừ hoàn toàn phân mảnh bộ nhớ và nguy cơ hết Heap khi hệ thống chạy liên tục dài ngày. |
| **2. Ngân sách Thời gian Thực Thống nhất** | Đo `elapsed = now_us() - begin` sau khi `xSemaphoreTake()` thành công; chỉ gọi xuống T1 nếu `elapsed < budget` với thời gian còn lại `budget - elapsed`. | **HOÀN THÀNH:** Đảm bảo thời gian thực thi tổng cộng không vượt quá deadline dự kiến của Task gọi. |
| **3. Đọc Thanh ghi 16-bit Nguyên tử (`read_reg16`)** | Ghép 2 thông điệp (`Write 2-byte reg MSB-first` + `Read N-byte data`) trong một lệnh `soft_i2c_transfer(b, m, 2, ms)`. | **HOÀN THÀNH:** Giữ khóa Mutex xuyên suốt cả pha ghi địa chỉ thanh ghi và pha đọc dữ liệu qua `Repeated START`. |
| **4. Ghi Thanh ghi 16-bit Không Cấp phát Động (`write_reg16`)** | Dùng mảng cục bộ `uint8_t buffer[258]` trên Stack để ghép 2 byte địa chỉ thanh ghi và tối đa 256 byte dữ liệu trong 1 thông điệp duy nhất. | **HOÀN THÀNH:** Hỗ trợ đầy đủ cả lệnh ghi thường (`64B`) lẫn lệnh nạp bộ đệm (`256B`) của Tiny1C mà không dùng `malloc`. |
| **5. Tách biệt ABI với Driver I²C Cũ** | Giữ nguyên `i2c.h` / `i2c.c` cho các tác vụ sensor MIPI sẵn có của SDK; cung cấp header riêng `soft_i2c.h`. | **HOÀN THÀNH:** Tương thích ngược 100% với toàn bộ mã nguồn FreeRTOS hiện hành của Cvitek/Sophgo. |
| **6. Kiểm thử Tự động & Biên dịch RISC-V** | Kiểm tra đầy đủ qua `test_i2c.c` (`run.sh`) và `compile-riscv.sh` với cờ `-Wall -Wextra -Werror`. | **HOÀN THÀNH:** Đạt **100% PASS**, sẵn sàng phục vụ phát triển Tầng 3. |
