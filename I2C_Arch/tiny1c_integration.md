# Chuẩn bị T3 Tiny1C: phối hợp I²C và SPI

Đoạn dưới là mẫu tích hợp cho task khởi tạo board, không tự bật bus khi boot.
`timer_hz` phải lấy từ cấu hình timebase firmware; `gpioa_owned` chỉ true sau
khi giải quyết quyền truy cập bank như README.

```c
#include "hal_i2c_gpio_platform.h"
#include "soft_i2c.h"
static soft_i2c_bus_t camera_bus;

bb_status_t camera_control_init(uint32_t timer_hz, bool gpioa_owned)
{
    bb_io_t io;
    bb_status_t rc = bb_gpio_platform_init(&io, timer_hz, gpioa_owned);
    if (rc != BB_OK) return rc;
    return soft_i2c_init(&camera_bus, &io, 100000, 1000);
}

/* Đọc trạng thái VDCMD, không phải xác thực model/firmware camera. */
bb_status_t camera_command_status(uint8_t *status)
{
    return soft_i2c_read_reg16(&camera_bus, 0x3C, 0x0200,
                               status, 1, 20);
}
```

Adapter Tiny1C ở T3 sẽ gọi read/write_reg16 thay cho STM32 HAL. Địa chỉ slave
là 0x3C; T1 tự tạo 0x78/0x79 trên dây. Register index 16 bit gửi MSB-first.
Việc đóng gói payload và giải mã response tuân theo từng VDCMD; không áp dụng
endianness của pixel SPI cho dữ liệu I²C.

Trình tự triển khai T3:

1. Khởi tạo I²C, xác minh status/identity theo SDK trên module thật.
2. Gắn SPI0 bằng API SPI hiện hành; cấu hình Mode 3, buffer và timeout.
3. VDCMD preview_start qua I²C, chọn output; thêm y16_preview_start khi cần.
4. Task capture nhận frame qua SPI, kiểm tra header/độ dài, công bố sang tầng 4.
5. Task control tuần tự hóa VDCMD bằng device mutex. Đổi mode/FFC phối hợp với
   capture ở ranh giới frame; loại bỏ khung đang chuyển trạng thái.

Lỗi I²C cần phân biệt lỗi transport với lỗi thực thi camera: ACK không đồng
nghĩa VDCMD thành công. Lệnh dài có command deadline riêng; bus timeout chỉ áp
cho từng transfer. Khi FFC chạy, không giữ bus mutex suốt thời gian chờ.

Bring-up phần cứng cần đo idle HIGH, START/STOP, 0x78/0x79, ACK, repeated START,
NACK cuối, rise time và clock thực. Kiểm tra mất camera, SDA/SCL kẹt, rồi chạy
SPI streaming đồng thời camera Linux. Host tests và cross-compile không thay
thế các phép đo này. Tài liệu Tiny1C trong repo còn tham chiếu SDK nguồn không
có ở đường dẫn tương đối hiện tại; chưa coi các opcode/offset header video là
đã kiểm chứng trên camera thật.
