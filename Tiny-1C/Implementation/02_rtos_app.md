# 02 — App FreeRTOS sở hữu camera

Mã nguồn chính thức nằm trong repo `freertos` tại [thermal_service.c](../../../freertos/cvitek/driver/sensor/tiny1C/rtos/thermal_service.c) và [thermal_protocol.h](../../../freertos/cvitek/driver/sensor/tiny1C/include/thermal_protocol.h) (giữ bản đối chiếu tại `Doc_md/Tiny-1C/App/` cho tuyến build Linux bridge/PC).

## Công việc

1. Thêm CMake option `THERMAL_TINY1C` trong [`freertos/cvitek/task/CMakeLists.txt`](../../../freertos/cvitek/task/CMakeLists.txt#L27-L34); chỉ dùng `CHIP=cv181x` và `CONFIG_FAST_IMAGE_TYPE=0`, thêm thư mục `${TOP_DIR}/driver/sensor/tiny1C` thành target `thermal_tiny1c`, đồng thời nối vào `CVI_TASK_LIBS` tại [`freertos/cvitek/task/main/CMakeLists.txt`](../../../freertos/cvitek/task/main/CMakeLists.txt#L36-L38).
2. [`freertos/cvitek/driver/sensor/tiny1C/CMakeLists.txt`](../../../freertos/cvitek/driver/sensor/tiny1C/CMakeLists.txt) biên dịch 6 tệp T3 (`src/*.c`), `rtos/thermal_service.c` và 3 tệp Soft-I²C5 T0–T2 (`soft_i2c.c`, `hal_i2c_bitbang.c`, `hal_i2c_gpio_platform.c`) thành `libthermal_tiny1c.a`; kiểm tra `THERMAL_I2C_FREQ_HZ` hợp lệ trong `1000..100000`.
3. [`freertos/cvitek/task/comm/src/riscv64/comm_main.c`](../../../freertos/cvitek/task/comm/src/riscv64/comm_main.c#L81-L91) dưới `#ifdef THERMAL_TINY1C` gắn `thermal_service_task` vào `gTaskCtx[E_QUEUE_CAMERA]` (`stack_size = configMINIMAL_STACK_SIZE * 4`, `priority = tskIDLE_PRIORITY + 3`, `queLength = 1`) và chạy `comm_poll_mailbox()` trong `prvCmdQuRunTask`.
4. Task chờ yêu cầu; không truy cập GPIO/SPI hoặc gửi lệnh camera khi boot.
5. START xác nhận quyền tài nguyên từ bridge (`THERMAL_GPIO_OWNED` và tùy chọn `THERMAL_DMA_OWNED`), gọi `core_init(5, I2C_SLAVE_ID)`, `spi_stream_init(0, SPI0_DSI_CS_PIN_DEFAULT)`, `core_start(mode, 25)` rồi chờ `100 ms`.
6. CAPTURE thu một frame (`98,304 B` hoặc `196,608 B`); trả header + payload hoặc status lỗi và payload rỗng.
7. FFC chỉ thực hiện khi người dùng bấm; STOP dừng preview.
8. Gửi response qua `E_QUEUE_CMDQU` để dùng spinlock/mailbox sẵn có, không tự ghi mailbox bằng đường fallback thiếu khóa.

## Cách build & kiểm tra App RTOS

```bash
# 1. Kiểm tra cross-compile T3 + thermal_service.c bằng RISC-V GCC (-Wall -Wextra -Werror)
make -C freertos/cvitek/driver/sensor/tiny1C riscv-check

# 2. Build đầy đủ firmware FreeRTOS (cấu hình I2C 20 kHz dùng trên board 18/11)
THERMAL_I2C_FREQ_HZ=20000 ./Doc_md/Tiny-1C/App/scripts/build_firmware.sh

# 3. Xác nhận build độc lập trong freertos (không tham chiếu Doc_md) và đủ symbol
! grep -q 'Doc_md' freertos/cvitek/build/task/build.ninja
grep -q ' T thermal_service_task$' Doc_md/Tiny-1C/App/build/cvirtos.symbols
```

Để kiểm tra cấu hình mặc định không bật camera nhiệt, cấu hình `freertos/cvitek/task` với `-DTHERMAL_TINY1C=OFF` (với `PROJECT_FULLNAME=sg2002_licheervnano_sd`, `DDR_64MB_SIZE=n` và toolchain `riscv64-unknown-elf-gcc` trong `PATH`).

## Bộ nhớ và quyền sở hữu

Buffer gồm header 64 byte + payload tối đa 196608 byte, căn 64 byte, nằm trong
BSS của firmware. Linker đặt nó trong reserved FreeRTOS DDR (hiện tại
0x8fe00000..0x8fffffff). Không dùng địa chỉ cố định 0x8fe80000 có thể đè BSS/heap.
RTOS clean cache + fence trước response. Linux map đọc và copy toàn frame sang
buffer riêng. RTOS tái sử dụng reply ở yêu cầu kế tiếp. Bridge copy xong rồi mới gửi yêu cầu mới. Một bridge giữ
flock độc quyền; không chạy ứng dụng khác gửi cùng IP/command IDs.

Đây là cơ chế thu theo yêu cầu, không quảng bá 25 fps liên tục. Khi không có
người xem, service không thu thêm frame. Khi client TCP ngắt, bridge gửi STOP.
Nếu bridge bị kill -9 thì camera có thể còn preview nhưng buffer không bị ghi
liên tục; khởi động bridge mới và START/STOP sẽ điều phối lại.

## ABI

- IP_CAMERA=7; START=0x40, CAPTURE=0x41, STOP=0x42, FFC=0x43, INFO=0x44.
- START param: mode 0/1/2 ở byte thấp; bit16 xác nhận GPIO/SPI ownership;
  bit17 xác nhận kênh DMA 0/1 nếu bật DMA.
- Response giữ command ID, block=1, param_ptr là địa chỉ vật lý header.
- Header THM1, version1, 64 byte, little-endian; không có pointer trong TCP.
- Các cờ: 1 mô phỏng, 2 shutter closed, 4 freeze, 8 high gain, 16 dual index mismatch.
- Hai stream không được tuyên bố đồng thời nếu index không khớp; viewer hiển thị cờ.

## Đọc chẩn đoán từ header

- CAPTURE thành công tăng `seq` và công bố kích thước thật. DUAL gắn cờ khi hai index lệch. Một plane lỗi làm payload của cả lần CAPTURE bằng 0.
- START lỗi dùng `index_image/index_temp` cho 8 byte echo, `flags` cho probe mask và `vtemp` cho status ASIC. `reserved[0]` là `BB_*` có dấu. `reserved[1]` chứa stage, pha NACK và register.
- Riêng CAPTURE `-4`, hai trường index chứa RX bytes 64..71. `vtemp` ghép offset byte khác 0 đầu tiên ở 16 bit cao, giá trị byte ở 8 bit tiếp theo và RX[0] ở 8 bit thấp.
- INFO trả mẫu metadata thiết bị khi đã init. Nếu chưa init, service chỉ có thể quét khi adapter I²C đã sẵn sàng.

[Lộ trình đọc T3 → App](../LO_TRINH_DOC_T3_APP.md)
