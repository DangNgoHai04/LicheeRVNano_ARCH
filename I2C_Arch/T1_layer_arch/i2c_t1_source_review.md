# Đánh giá Source và Trạng thái Hoàn thiện của Tầng 1 (Bitbang Controller HAL) — I²C

Cập nhật: **09-11-2026**. Board mục tiêu: **LicheeRV-Nano (SOPHGO SG2002 / CV181x — Engine I²C Bitbang Master)**.

**Kết luận kiểm tra source:** Tầng 1 (`hal_i2c_bitbang.c` và `hal_i2c_bitbang.h`) đã được chuẩn hóa hoàn chỉnh, tách biệt hoàn toàn khỏi FreeRTOS và thanh ghi SoC. Máy trạng thái xử lý đầy đủ mọi tình huống biên của giao thức I²C (Repeated START, Clock Stretching có giới hạn riêng, Arbitration Loss, tách biệt lỗi `cleanup` của điều kiện STOP và cứu hộ 9 xung clock) và vượt qua 100% bài kiểm thử đơn vị cũng như trình biên dịch RISC-V 64-bit (`-Wall -Wextra -Werror`).

---

## 1. Phạm vi và Nguồn đối chiếu

- **Source Tầng 1**:
  - [`hal_i2c_bitbang.c`](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_bitbang.c): Thực thi các hàm nguyên tử đường dây (`high`, `start`, `stop`, `send_bit`, `recv_bit`, `write_byte`, `read_byte`) và 3 API chính (`bb_init`, `bb_transfer`, `bb_recover`).
  - [`hal_i2c_bitbang.h`](../../../freertos/cvitek/hal/cv181x/i2c/include/hal_i2c_bitbang.h): Định nghĩa `bb_status_t`, `bb_io_t`, `bb_msg_t`, `bb_master_t`.
- **Tài liệu đối chiếu**:
  - Đặc tả giao thức I²C Standard-mode (100 kHz) và chuẩn CCI (Camera Control Interface) 16-bit Register của cảm biến nhiệt InfiRay Tiny1C (`0x3C`).
  - [Tài liệu Kiến trúc T0](../T0_layer_arch/i2c_t0_platform_architecture.md): 4 Platform Hooks (`drive`, `sample`, `now_us`, `delay_us`).

---

## 2. Bảng Đối chiếu Các Hạng mục Kỹ thuật Đã Hoàn thiện ở Tầng 1

| Hạng mục | Cơ chế thực thi trong Source Code | Đánh giá & Trạng thái |
| :--- | :--- | :--- |
| **1. Kiểm tra Tham số Trước Khi Chạm Dây** | `bb_transfer()` duyệt toàn bộ mảng `bb_msg_t` kiểm tra `addr <= 0x7F`, `len > 0` khi `read == true`, và `data != NULL` khi `len > 0` **trước khi** phát điều kiện `START`. | **HOÀN THÀNH:** Tránh tạo khung truyền dở dang trên đường dây khi caller truyền sai tham số. |
| **2. Giao dịch Đa thông điệp & Repeated START** | Giữa các phần tử `m[i]` liên tiếp trong mảng `bb_msg_t`, `start(b)` tự động nhả `SDA`, nâng `SCL` rồi kéo `SDA` xuống thấp để tạo `Repeated START` mà không phát `STOP` ở giữa. | **HOÀN THÀNH:** Đáp ứng chính xác chu trình đọc thanh ghi 16-bit của Tiny1C (`START -> 0x78 -> reg_hi -> reg_lo -> Sr -> 0x79 -> data -> STOP`). |
| **3. Hỗ trợ Clock Stretching Kép** | `high()` chờ `level(b, true) == true` với hai điều kiện bảo vệ: `now(b) - start < b->stretch_us` (mỗi xung) và `now(b) < b->deadline` (toàn phiên). Chỉ bắt đầu đếm `half_us` sau khi `SCL` đã thực sự lên mức cao. | **HOÀN THÀNH:** Đảm bảo độ rộng xung `HIGH` tối thiểu luôn đạt chuẩn ngay cả khi ASIC Tiny1C giữ `SCL` để xử lý nội bộ. |
| **4. Quy tắc ACK/NACK Khi Đọc** | `read_byte()` gọi `send_bit(b, last)` với `last = (j + 1 == m[i].len)`: phát `0` (ACK) cho các byte đầu và phát `1` (NACK) cho byte cuối cùng. | **HOÀN THÀNH:** Ngăn chặn lỗi Slave tiếp tục giữ `SDA` sau byte đọc cuối khiến Master không thể tạo điều kiện `STOP`. |
| **5. Phát hiện Mất Quyền Phân Xử (`BB_ARBITRATION`)** | Trong `send_bit()`, nếu Master nhả `SDA` lên `1` (`bit == true`) nhưng khi `SCL` ở mức cao lại đọc về `level(b, false) == 0`, hàm trả về `BB_ARBITRATION` và nhả cả hai dây, **không** phát `STOP`. | **HOÀN THÀNH:** Bảo vệ an toàn đường truyền khi có nhiễu hoặc xung đột trên bus. |
| **6. Tách biệt Lỗi Giao dịch và Lỗi Cleanup (`STOP`)** | Khi kết thúc bình thường (`BB_OK`) hoặc khi gặp `BB_NACK`, T1 gọi `b->cleanup = stop(b)`. Nếu lỗi gốc là `BB_NACK`, hàm vẫn trả về `BB_NACK` đồng thời lưu kết quả `STOP` vào `b->cleanup`. | **HOÀN THÀNH:** Không làm mất mã lỗi gốc của giao dịch trong khi vẫn cho phép chẩn đoán nếu đường dây bị kẹt lúc đóng phiên. |
| **7. Cứu hộ Bus Chủ động (`bb_recover`)** | Phát tối đa 9 xung `SCL` cho đến khi Slave nhả `SDA` lên mức cao rồi kết thúc bằng `stop(b)`. | **HOÀN THÀNH:** Cho phép khôi phục bus sạch sẽ sau khi xảy ra timeout hoặc reset nóng mà không cần cắt nguồn phần cứng. |

---

## 3. Trạng thái Kiểm chứng Kỹ thuật

- **Host Unit Test:** Đã chạy `bash freertos/cvitek/hal/cv181x/i2c/tests/run.sh` $\rightarrow$ **PASS 100%**.
- **Biên dịch chéo RISC-V 64-bit:** Đã chạy `bash freertos/cvitek/hal/cv181x/i2c/tests/compile-riscv.sh` $\rightarrow$ **PASS (`-Wall -Wextra -Werror`, 0 error, 0 warning)**.
- **Kết luận:** Tầng 1 đã đạt độ ổn định cao, sẵn sàng phục vụ Tầng 2 (`soft_i2c`) và tầng điều khiển lệnh VDCMD của Tiny1C (Tầng 3).
