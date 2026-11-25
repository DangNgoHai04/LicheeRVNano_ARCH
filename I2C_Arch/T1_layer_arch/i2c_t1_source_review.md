# Đối chiếu mã nguồn T1 I2C — Bitbang controller HAL

Đối chiếu ngày **25-11-2026** cho LicheeRV-Nano (SG2002/CV181x). [Bản kiến trúc](i2c_t1_controller_architecture.md) · [README I2C HAL](../../../freertos/cvitek/hal/cv181x/i2c/README.md) · [Mã nguồn chính](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_bitbang.c) · [Bộ test host](../../../freertos/cvitek/hal/cv181x/i2c/tests/run.sh)

## Hợp đồng đọc từ mã nguồn

| Điểm đối chiếu | Hành vi trong source |
| --- | --- |
| `bb_init` / `bb_transfer` | Chấp nhận 1–100 kHz. Kiểm tra mảng message trước khi phát xung. Ghi rỗng hợp lệ cho address probe. Cập nhật `nack_phase` (`0x00` địa chỉ msg0, `1..` dữ liệu msg0, bit 7 `0x80` cho msg1, `0xFF` khi không NACK). |
| `high` / `start` / `stop` | Chờ SCL HIGH với giới hạn stretch và deadline. Lần START sau tạo repeated START giữa hai message. |
| `write_byte` / `read_byte` | Truyền MSB-first, theo dõi ACK của slave và phát NACK ở byte đọc cuối. |
| `bb_recover` | Tối đa 9 xung khi SDA thấp, rồi thử STOP nếu các xung thành công. T1 ghi `cleanup` và nhả hai dây. |

### Lần theo một giao dịch CCI

Đọc `0x0200` của Tiny1C tạo `S → 78 → 02 → 00 → Sr → 79 → data → NACK của master → P`. Các byte địa chỉ và byte ghi cần ACK của slave. NACK do master phát sau byte đọc cuối là một phần của giao dịch thành công. Khi slave NACK, `master.nack_phase` ghi vị trí và `master.cleanup` giữ kết quả STOP riêng. Ví dụ `0x00` là địa chỉ ghi, `0x01` là byte thanh ghi cao và `0x80` là địa chỉ đọc của thông điệp thứ hai. Trường này mã hóa gọn cho chẩn đoán CCI, vị trí rất dài có thể trùng nhau do trường chỉ rộng 8 bit.

`bb_init` nhận `1–100 kHz`. Yêu cầu 20 kHz cho `half_us=25`, còn 100 kHz cho `half_us=5`. Overhead GPIO, rise time, clock stretching và lịch RTOS ảnh hưởng chu kỳ đo trên dây. Khi timeout hoặc mất phân xử, T1 nhả hai dây. Recovery là lời gọi riêng từ tầng trên. [Driver T3](../../../freertos/cvitek/driver/sensor/tiny1C/README.md)
