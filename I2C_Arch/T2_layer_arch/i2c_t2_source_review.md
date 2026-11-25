# Đối chiếu mã nguồn T2 I2C — RTOS Soft-I²C

Đối chiếu ngày **25-11-2026** cho LicheeRV-Nano (SG2002/CV181x). [Bản kiến trúc](i2c_t2_driver_architecture.md) · [README Soft-I2C](../../../freertos/cvitek/driver/i2c/README.md) · [Driver T3](../../../freertos/cvitek/driver/sensor/tiny1C/README.md) · [Mã nguồn chính](../../../freertos/cvitek/driver/i2c/src/soft_i2c.c) · [Bộ test host](../../../freertos/cvitek/hal/cv181x/i2c/tests/run.sh)

## Hợp đồng đọc từ mã nguồn

| Điểm đối chiếu | Hành vi trong source |
| --- | --- |
| `soft_i2c_init` | Tạo mutex tĩnh trong bus và giữ bản sao bảng hook qua T1. |
| `run` | Tính ngân sách trước khi chờ khóa. Truyền microgiây còn lại cho `bb_transfer` hoặc `bb_recover`. |
| `soft_i2c_read_reg16` | Hai message liên tiếp, ghi hai byte register MSB-first và đọc qua repeated START. |
| `soft_i2c_write_reg16` | Một message, buffer stack 258 byte, payload 1–256 byte. T3 còn có đường `soft_i2c_transfer` trực tiếp cho trường hợp chỉ gửi index. |

### Ranh giới timeout và khóa khi phục vụ Tiny1C

T2 nhận `timeout_ms` từ 1 đến 60.000. Chờ khóa hết thời gian trả `BB_BUSY`. Lấy được khóa nhưng đã cạn ngân sách trả `BB_TIMEOUT` trước khi chạm dây. Bộ đếm `transfers/errors` cập nhật sau khi lấy khóa, kể cả lời gọi recovery. Cả repeated START lẫn STOP thuộc cùng lời gọi T2 và dùng phần ngân sách còn lại.

T3 gọi T2 nhiều lần cho một lệnh. Mutex VDCMD ở `vdcmd.c` giữ thứ tự submit → poll → đọc đáp ứng; mutex bus nhả sau mỗi transfer. Trên RISC-V, `soft_i2c.c` được biên dịch vào `libthermal_tiny1c.a` qua [`tiny1C/CMakeLists.txt`](../../../freertos/cvitek/driver/sensor/tiny1C/CMakeLists.txt) khi bật `-DTHERMAL_TINY1C=ON`. Cấu hình mặc định T3 là 100 kHz, stretch 20 ms và timeout 1.000 ms cho mỗi giao dịch. Firmware trong nhật ký 18/11 override tần số xuống 20 kHz. Deadline poll VDCMD mặc định 20 s được kiểm tra giữa các lần poll và tách khỏi timeout T2. [Lộ trình T3 → App](../../Tiny-1C/LO_TRINH_DOC_T3_APP.md)
