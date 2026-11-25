# Yêu cầu T3 Tiny1C trên SPI0 và Soft-I2C

Ngày kiểm tra: 2026-11-12. Phạm vi: tài liệu SPI/I2C, bảng API Tiny1C 1.2, bảng phụ thuộc firmware, bộ lệnh I2C gốc 1.2.0, tài liệu hiệu chuẩn, sample STM32 và source Driver_T3 hiện tại. Đây là đánh giá source/tài liệu; không phải kết quả thử camera thật hay xác nhận từng opcode của toàn bộ API.

## 1. Kết luận

T3 cần driver thiết bị gồm kênh điều khiển VDCMD qua I2C và kênh dữ liệu VoSPI qua SPI. Source hiện có đủ các khối chính nhưng chưa đủ điều kiện nghiệm thu. Không cần mang toàn bộ STM32 SDK sang SG2002. IPC Linux và task ứng dụng là lớp tích hợp, không phải điều kiện để một driver Tiny1C độc lập hoạt động.

## 2. Thành phần bắt buộc

| Thành phần | Trách nhiệm T3 | Hiện trạng |
|---|---|---|
| Transport I2C | Gọi soft_i2c T2, địa chỉ 7-bit 0x3C, register 16-bit MSB-first, truyền lỗi | Có adapter; cần bỏ retry mù đối với ghi kích hoạt lệnh |
| VDCMD | Short/Standard/Long read/write; HLD 0x9D00, RW 0x1D00, polling 0x0200; mutex toàn chuỗi | Có; timeout lệnh và lỗi cần hoàn thiện |
| API camera | Đóng gói đúng opcode, tham số, độ dài, endianness; giới hạn API theo model/FW | Hầu hết tên hàm có, chưa xác minh toàn bộ semantics |
| Lifecycle | Probe thông tin FW/PN/SN, start/stop, đổi mode, restore/reboot/reconnect | Có start/stop; chưa có quản lý đầy đủ trạng thái và khả năng firmware |
| VoSPI | New frame AA/CC, continued frame 55; CS, delay, parse header, payload | Có luồng thu; sai lựa chọn stream nhiệt đơn, thiếu kiểm tra frame |
| Metadata và dữ liệu | Valid bit, frame index, shutter/gain/freeze; YUYV hoặc nhiệt 16-bit LE | Có struct và helper; chưa bảo đảm frame hợp lệ trước khi công bố |
| Đồng bộ | Khóa thiết bị, điều phối đổi mode/FFC với capture; quyền sở hữu buffer | Có mutex VDCMD; chưa đủ cho vòng đời và buffer liên nhân |
| Lỗi và kiểm thử | Deadline, phân biệt lỗi bus/lỗi camera, thống kê frame lỗi, phục hồi | Có khung xử lý; host hiện chủ yếu là smoke test giả lập |

T0/T1/T2 tiếp tục sở hữu pinmux, MMIO, DMA, cache và bus lock. Board phải xác nhận tài nguyên SPI/DMA/GPIO trước khi bind vào T3; không dùng hằng true để thay cho việc thực sự dành tài nguyên.

## 3. Luồng stream cần hỗ trợ

| Mode | Cấu hình preview | Y16 | Yêu cầu frame |
|---|---|---|---|
| IMAGE | 256x192, mode 8 | Tắt khi chuyển từ TEMP | 0xAA |
| TEMP | 256x192, mode 8 | y16_preview_start(PATH0, TEMPERATURE) | 0xAA theo temp_frame_get của sample |
| IMAGE + TEMP | 256x384, mode 8 | Không mặc định giữ Y16 của mode TEMP | 0xAA cho ảnh, 0xCC cho nhiệt; theo dõi metadata từng stream |

Trình tự sample: CS LOW, trao đổi 512 byte khởi đầu, CS HIGH, chờ 1 ms, CS LOW, đọc 192 hàng x 512 byte, CS HIGH. Phải giữ CS xuyên suốt phần payload, kể cả khi DMA chia khối. Khi mất đồng bộ phải hủy frame dở và bắt đầu lại frame mới.

**Điều chỉnh nhận xét trước về MOSI:** sample phát mỗi hàng `55 00 ...`; T3 phát toàn `55`. Sơ đồ tài liệu thể hiện `55h` là lệnh continued frame ở đầu phiên CS, phần sau không chỉ rõ giá trị bắt buộc. Vì vậy đây là khác biệt cần xác minh, không đủ căn cứ kết luận chắc chắn sai. Bring-up nên dùng mẫu hãng trước, sau đó kiểm chứng cách tối ưu bằng capture thực.

Header: phần chữ và bảng định nghĩa nêu 32 byte; hình New Frame lại ghi 64 byte. Dummy cycles cấu hình được, tối thiểu 512 SCK = 64 byte. Vì vậy không coi offset 64 và điểm bắt đầu payload là phổ quát chỉ vì struct khớp bảng. Cần xác nhận cấu hình camera và dữ liệu thực; kiểm tra frame_valid bit 0, frame_index, shutter/gain/freeze, không phát hành frame giả hợp lệ.

Băng thông payload ở 25 fps:
- Một stream: 256 x 192 x 2 x 8 x 25 = 19.6608 Mbit/s.
- Hai stream mỗi frame: 39.3216 Mbit/s, chưa gồm header, khoảng chờ và overhead DMA/RTOS.
- SCK mặc định 23.4375 MHz chỉ có khả năng đáp ứng một stream về lý thuyết; không thể đáp ứng hai stream mỗi frame ở 25 fps. Cần đo throughput và chọn SCK phù hợp phần cứng, hoặc lấy nhiệt cách frame theo sample. Không đồng nhất FPS cấu hình camera với FPS host thu được.

## 4. API hãng và phạm vi tính năng

Đếm các tên API firmware tại cột C, dòng 4–38 của Interface Description1.2: có 38 tên; 37 có implementation mang tên tương ứng trong falcon_cmd.c hiện tại (spi_read đã đổi thành falcon_spi_read). Thiếu tpd_set_rect_temp_thd. Đây chỉ là kiểm kê sự hiện diện, không phải mức độ đúng 37/38.

Điểm cần lưu ý: tpd_set_rect_temp_thd cũng không được tìm thấy trong .c/.h của sample STM32 và trong nội dung CHM đã giải nén. Bảng API có tên nhưng mô tả set/get không nhất quán. Không tự đoán opcode; đánh dấu unsupported cho đến khi có đặc tả xác nhận.

Nhóm chức năng cần phân lớp:
- Cơ bản: get_device_info, preview_start/stop, y16_start/stop, FFC/ooc_b_update, lấy frame và metadata.
- Theo yêu cầu sản phẩm: palette, TNR/SNR/DDE/brightness/contrast/AGC, mirror/flip, zoom; TPD điểm/đường/vùng/min/max; auto-shutter/fall protection; lưu cấu hình.
- Hiệu chuẩn tùy chọn: DPC, lid correction, một/hai điểm. Cần cả quy trình điều phối chứ không chỉ wrapper gửi lệnh.
- Firmware update tùy chọn: tách module và trạng thái maintenance; tham khảo OTHER/firmware.c, không lẫn vào capture thường.

Tài liệu hãng yêu cầu chỉ dùng API/tham số được bảng Tiny1C cho phép; đặc biệt không coi mọi hàm erase/write Flash trong falcon_cmd.h là API thông thường của sản phẩm. Các API cập nhật firmware phải có phạm vi riêng. Trong bảng Tiny1C, palette không liệt kê MODE_2; TPD không liệt kê TPD_PROP_DISTANCE, dù generic header có enum này. T3 hiện cho phép MODE_2 và core_set_env_params luôn ghi DISTANCE: cần kiểm tra lại thay vì coi generic enum là xác nhận hỗ trợ.

## 5. Timeout, firmware và hiệu chuẩn

- SDK STM32 đặt I2C_TRANSFER_WAIT_TIME_MS = 20000; T3 hiện đặt 2000. Không đủ cho một số lệnh hiệu chuẩn.
- Bảng Delay time: lid correction có thời gian chờ rộng 5 s; một điểm và điểm thứ hai của hiệu chuẩn hai điểm có thời gian chờ rộng 20 s. Tài liệu tổng quan có con số khác; ưu tiên deadline theo thao tác và kiểm chứng đúng FW. Không áp dụng bus timeout 1 s hay command timeout 2 s cho mọi thao tác.
- Dùng deadline thời gian thực và polling trạng thái, không chỉ trừ mỗi delay 5 ms trong biến đếm. Không khóa bus I2C suốt thời gian camera xử lý; vẫn khóa giao dịch cấp thiết bị.
- Không tự gửi lại ghi kích hoạt khi chưa biết lần trước đã thực thi hay chưa.
- Bảng dependency: FW 3.1/3.2 thiếu một số khả năng FFC/fall protection. FW 3.6–3.9 có hành vi restore gây restart, trả lỗi và dừng stream. T3 cần nhận diện FW, chờ khởi động lại và tái khởi tạo stream theo trạng thái; không chỉ chuyển tiếp mã lỗi.
- Tài liệu không thống nhất về shutter temperature: bảng dependency báo chưa hỗ trợ trong các bản nêu trên. Không hứa chức năng chỉ vì có symbol shutter_vtemp_get.
- Hiệu chuẩn cần dừng/điều phối stream, quản lý auto FFC, thứ tự thao tác, điều kiện cảnh và khôi phục cấu hình. Không tự chạy hiệu chuẩn khi boot.

## 6. Dữ liệu nhiệt và hiệu chỉnh môi trường

Tách rõ pixel VoSPI 16-bit little-endian (sample đổi Celsius bằng raw/64 - 273.15) với response TPD qua I2C và metadata Vtemp. Không áp dụng cùng endianness/scale cho mọi nguồn. Vtemp không tự động là nhiệt độ Celsius; cần hệ số riêng nếu muốn quy đổi.

Có hai cách hiệu chỉnh môi trường:
1. Firmware: gửi EMS/TAU/TA/TU bằng set_prop_tpd_params; EMS/TAU dạng 7-bit, 0..128. LUT tau 14-bit phải đổi scale trước khi ghi. Khoảng cách/độ ẩm/nhiệt độ có thể dùng để tra LUT tương đương theo tài liệu.
2. Host SDK: OTHER/temperature.c + libirtemp + LUT/NUC. libirtemp.lib đi kèm STM32 không được coi là thư viện RISC-V; tài liệu nói thuật toán đóng nguồn và hãng cần biên dịch cho target. Phần này là phụ thuộc riêng, không bắt buộc để nhận frame nhiệt hoặc dùng TPD của firmware.

core_set_env_params hiện chỉ ghi tham số firmware, chưa triển khai đầy đủ đường hiệu chỉnh host theo hãng.

## 7. Tích hợp SG2002 và tiêu chí hoàn thành

- Driver lõi xuất API init/probe/start/stop/read_frame/control, không phụ thuộc bắt buộc Linux IPC.
- Task/service gọi driver; IPC/mailbox chuyển frame cho Linux. Quy hoạch reserved DDR, cache, queue và double-buffer hoặc cơ chế acquire/release để không ghi đè frame Linux đang đọc.
- Nối source/library vào CMake firmware, gọi task thực sự; sửa include/FreeRTOS port và link. Kết quả kiểm tra trước: host build sạch với mock; cross-check theo Makefile T3 lỗi MTIME config; chưa có nối T3 trong firmware.
- Kiểm thử packet VDCMD theo bảng lệnh, long command/poll timeout/lỗi, lựa chọn AA/CC theo mode, valid/index và frame thiếu, chuyển mode, reset/reconnect, nhiều task.
- Trên board: probe đúng FW/PN/SN, thử IMAGE rồi TEMP rồi dual/interval, đo SCK/CS/MOSI/MISO, kiểm tra nhiệt điểm so với frame, đo FPS và frame drop. Chỉ nghiệm thu sau khi kiểm tra với camera thật.

Ưu tiên: (1) transport/command deadline + probe; (2) stream TEMP AA + parser/frame validation; (3) lifecycle và cấu hình băng thông; (4) build/task/buffer; (5) API mở rộng/hiệu chuẩn theo nhu cầu.

## 8. Nguồn đã dùng

- [RTOS╞╜╠¿SPI╜╙┐┌╚φ╝■┐¬╖ó╫╩┴╧ Interface development instructionsV1.0.docx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/RTOS╞╜╠¿SPI╜╙┐┌╚φ╝■┐¬╖ó╫╩┴╧ Interface development instructionsV1.0.docx>)
- [Tiny1C─ú╫ΘSDK╜╙┐┌╦╡├≈Interface Description1.2.xlsx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/Tiny1C─ú╫ΘSDK╜╙┐┌╦╡├≈Interface Description1.2.xlsx>)
- [SPI╥└└╡╣╪╧╡ Interface dependency.xlsx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/SPI╥└└╡╣╪╧╡ Interface dependency.xlsx>)
- [╘¡╩╝I2C╓╕┴ε╝» Original I2C instruction set/Vender_command_description1.2.0.xlsx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/╘¡╩╝I2C╓╕┴ε╝» Original I2C instruction set/Vender_command_description1.2.0.xlsx>)
- [╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/▓Γ╬┬╙δ╣°╕╟▒Ω╢¿Secondary calibration& Lid pattern noise correction/╢■┤╬▒Ω╢¿╤╙╩▒╩▒╝ΣDelay time.xlsx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/▓Γ╬┬╙δ╣°╕╟▒Ω╢¿Secondary calibration& Lid pattern noise correction/╢■┤╬▒Ω╢¿╤╙╩▒╩▒╝ΣDelay time.xlsx>)
- [Tiny1C_STM32_SPI_SDK/USER/main.c](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/Tiny1C_STM32_SPI_SDK/USER/main.c>)
- [Tiny1C_STM32_SPI_SDK/VDCMD/all_config.h](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/Tiny1C_STM32_SPI_SDK/VDCMD/all_config.h>)
- [Tiny1C_STM32_SPI_SDK/OTHER/temperature.c](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/Tiny1C_STM32_SPI_SDK/OTHER/temperature.c>)
- [Tiny1C_STM32_SPI_SDK/OTHER/firmware.c](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/Tiny1C_STM32_SPI_SDK/OTHER/firmware.c>)
- [libirsdk.chm](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/libirsdk.chm>)
- [release_note.txt](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/release_note.txt>)
- [╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/╗╖╛│▒Σ┴┐╨▐╒²Ambient variable correction/README(3).docx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/╗╖╛│▒Σ┴┐╨▐╒²Ambient variable correction/README(3).docx>)
- [╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/▓Γ╬┬╙δ╣°╕╟▒Ω╢¿Secondary calibration& Lid pattern noise correction/README.docx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/▓Γ╬┬╙δ╣°╕╟▒Ω╢¿Secondary calibration& Lid pattern noise correction/README.docx>)
- [╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/├ñ╘¬▒Ω╢¿Dead pixel correction/README(Instruction Table).docx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/├ñ╘¬▒Ω╢¿Dead pixel correction/README(Instruction Table).docx>)
- [╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/├ñ╘¬▒Ω╢¿Dead pixel correction/README.docx](</home/haidang/Documents/Tiny1C-SDK/Tiny1C-SDK/SDK/RTOS/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/ASIC_2121W_RTOS_SDK_V1.0.0_release/SPI/╙├╗º┐¬╖ó▒Ω╢¿ User calibration instructions/├ñ╘¬▒Ω╢¿Dead pixel correction/README.docx>)

Source đang đánh giá: [Driver_T3](Driver_T3/README.md). Không sửa source driver trong lần kiểm tra này.
