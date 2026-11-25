# Tích hợp Soft-I²C và SPI0 cho Tiny1C

Task camera dùng hai đường truyền. VDCMD đi qua `core.c → falcon_cmd.c → vdcmd.c → data_rw.c → soft_i2c.c` để probe, bật preview và FFC. Dữ liệu điểm ảnh đi qua `vospi.c → hal_spi.c → spi.c`. Xem điểm vào thực tế tại [thermal_service_task](../../freertos/cvitek/driver/sensor/tiny1C/rtos/thermal_service.c) và [Driver T3](../../freertos/cvitek/driver/sensor/tiny1C/README.md).

1. START xác nhận tài nguyên. `core_init` gọi GPIO platform và Soft-I²C, chuẩn hóa địa chỉ `0x78 → 0x3C` rồi probe. Một lần đọc thanh ghi truyền index MSB-first, repeated START và NACK cuối do master phát.
2. T3 khóa từng lệnh VDCMD từ submit đến response. T2 khóa mỗi giao dịch riêng. Đọc `0x0200` phân biệt ACK ở dây với kết quả thực thi của ASIC.
3. Service khởi tạo SPI0 rồi gọi `core_start`. Lệnh preview chọn format; `MODE_TEMP` bật Y16. Một CAPTURE thu header và payload trong hai phiên CS trên SPI0.
4. Task công bố gói THM1 qua DDR/mailbox. Bridge kiểm tra rồi gửi TCP. Viewer giải mã YUYV hoặc Y16.

Cấu hình mặc định I²C 100 kHz, còn phiên board 18/11 dùng 20 kHz và SPI0 polling ở SCK cấu hình 11,71875 MHz. Khi đối chiếu lỗi, đọc ownership và idle trước, wire ACK/NACK tiếp theo, rồi status ASIC và cuối cùng là header VoSPI. [Nhật ký board](../Tiny-1C/Implementation/09_board_bringup_2026-11-18.md) · [Lộ trình đọc](../Tiny-1C/LO_TRINH_DOC_T3_APP.md)
