# Đối chiếu mã nguồn T0 I2C — GPIO platform

Đối chiếu ngày **25-11-2026** cho LicheeRV-Nano (SG2002/CV181x). [Bản kiến trúc](i2c_t0_platform_architecture.md) · [README I2C HAL](../../../freertos/cvitek/hal/cv181x/i2c/README.md) · [Mã nguồn chính](../../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) · [Bộ test host](../../../freertos/cvitek/hal/cv181x/i2c/tests/run.sh)

## Hợp đồng đọc từ mã nguồn

| Điểm đối chiếu | Hành vi trong source |
| --- | --- |
| `bb_gpio_platform_init` | Kiểm tra con trỏ, tần số timebase, ownership và singleton. Bật clock `0x03002000` bit 29, nhả DDR/preload DR=0, bật `PAD_PU` trên `SPK_EN_IOBLK` (`0x03001908`) & `EMMC_DAT3_IOBLK` (`0x03001924`), đặt FMUX Func 3, readback kiểm tra và chờ `50 µs` kiểm tra mức idle HIGH qua `GPIOA_EXT` (`[T0-I2C] Idle line LOW`). |
| `drive` / `sample` | DDR đổi hướng để kéo thấp hoặc nhả. EXT đọc mức chân thực. Các thao tác RMW được bảo vệ cục bộ trên FreeRTOS. |
| `time_us` / `delay_us` | Đổi `rdtime` sang microgiây bằng timebase truyền vào. Delay bận chờ theo bộ đếm. |
| Giới hạn xác minh | Kiểm thử mock và cross-compile kiểm tra triển khai. Tình trạng đường dây và quyền ghi GPIOA liên nhân được xác nhận theo từng board. |

### Đường khởi tạo khi dùng Tiny1C

`THERMAL_START` xác nhận cờ tài nguyên trước khi `data_rw_i2c_hw_init()` gọi `bb_gpio_platform_init()`. I²C5 là ID logic trong driver camera. Hai dây vật lý là GPIOA15 (SPK_EN/SCL) và GPIOA27 (EMMC_DAT3/SDA), không đi qua IP DW I²C. T0 nhả DDR, preload DR bằng 0, bật pull-up pad, đổi mux rồi kiểm tra readback và idle HIGH sau 50 µs. `claimed` chỉ được đặt sau khi các kiểm tra này thành công.

Nếu START trả `-22` và chẩn đoán `BB_NOT_READY`, đọc log `[T0-I2C] Idle line LOW`, kiểm tra chân nào thấp, miền I/O, điện trở kéo và quyền ghi cùng bank GPIOA. Kiểm tra idle trước khi phân tích opcode VDCMD. Bring-up ngày 18/11 thu được dữ liệu ở cấu hình I²C 20 kHz với module và dây nối đã dùng hôm đó. [Nhật ký board](../../Tiny-1C/Implementation/09_board_bringup_2026-11-18.md)
