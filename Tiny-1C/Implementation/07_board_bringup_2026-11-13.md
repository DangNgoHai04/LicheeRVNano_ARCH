# Bước 7: Kết quả bring-up board 192.168.1.27 ngày 2026-11-13

## Nguồn dòng UART STM32 SPI0

Dòng `[STM32 SPI0] Boot test: FreeRTOS master / STM32 slave, Mode 3` nằm nguyên
văn trong `/etc/init.d/S99stm32test` trên Linux. `S99temptest` chỉ in dòng
`Starting temptest: OK` ngay trước đó. Script STM32 tự chạy lúc boot, kiểm tra
`/root/check_stm32.py` và có thể unbind Linux SPI0. Chuỗi này không có trong
`cvirtos.bin` của Tiny1C.

Đã đổi tên script trên board thành `/etc/init.d/stm32test.disabled`. `rcS` chỉ
chạy `/etc/init.d/S??*`, nên test STM32 không tự chạy ở lần boot tiếp theo.
Không có tiến trình `check_stm32.py` đang chạy lúc kiểm tra. Muốn khôi phục,
đổi tên file về `S99stm32test`. Sau khi bỏ script này, nếu Linux lại bind SPI0
thì cần unbind thủ công trước khi chạy bridge Tiny1C.

## Lỗi THERMAL_START=-5

Firmware chẩn đoán truyền mã BB, bước probe, thanh ghi và vị trí NACK về
`thermal_header_t.reserved[2]`. Kết quả trên board:

| Phép thử | Phản hồi |
|---|---|
| 100 kHz, START nhiệt | `status=-5, probe=PROJECT, reg=0x1d00, BB=NACK(-3)` |
| 20 kHz, START nhiệt | Cùng kết quả; NACK tại `msg0.address` |
| 20 kHz, address scan 0x08..0x77 | Không có địa chỉ nào ACK |

Address scan chỉ gửi byte địa chỉ rồi STOP, không ghi thanh ghi camera. Kết quả
cho thấy lỗi xảy ra trước VDCMD và trước SPI. Giảm tốc độ từ 100 xuống 20 kHz
không làm xuất hiện ACK; không thể kết luận firmware camera hay parser VoSPI lỗi.

Thanh ghi pad được người vận hành đọc sau START: `SPK_EN_IOBLK=0x44`,
`EMMC_DAT3_IOBLK=0x44`, GPIOA_EXT bit 15 và 27 đều HIGH khi rảnh. Điều này
không chứng minh biên độ và sườn xung ở phía đầu nối Tiny1C. Cần đo tại J4
(GPIOA15/SCL), J9 (GPIOA27/SDA) và ngay chân module, đối chiếu VDDIO, nguồn
camera, GND chung, chiều FPC và chân RESET/ENABLE của carrier. Không nối
pull-up vào 3.3 V trước khi xác nhận miền điện áp cả hai phía.

Người vận hành báo đồng hồ đọc SCL = 0 V tại cả J4 và module sau START.
Ở một lần đọc thanh ghi sau START trên cùng board qua SSH, `GPIOA_EXT` là
`0x7EBF9F80` (A15=1, A27=1), `GPIOA_DDR=0x04004000` (A15 input/nhả) và
pinmux SPK_EN là `3` (GPIO). Hai quan sát không khớp nếu đo cùng thời điểm,
cùng net và cùng GND. Cần đo lại sau khi START đã trả lỗi và bus đứng rảnh,
que đen đặt ở GND board J3; chụp rõ điểm đo J4 và đầu SCL module. Khi đã tắt
nguồn cả hai phía, đo thông mạch từ J4 đến SCL module và kiểm tra có chập GND.
Không sửa địa chỉ I2C hoặc ép GPIO lên HIGH chỉ dựa trên số đo đang mâu thuẫn.

Sau khi người vận hành thay lại chân cấp I2C, thử `THERMAL_START` lần nữa trên
FIP 20 kHz vẫn cho `status=-5, probe=PROJECT, reg=0x1d00, BB=NACK(-3),
nack_at=msg0.address`; address scan tiếp tục không có địa chỉ ACK. Các thanh
ghi đọc sau lần thử mới: `GPIOA_EXT=0x7EBF9F00` (A15/A27 HIGH),
`GPIOA_DDR=0x04004000` (A15/A27 là input), hai IOBLK đều `0x44` (pull-up bật,
pull-down tắt). Gói Web Viewer có `payload_bytes=0` vì START thất bại trước
khi khởi động SPI. Cần điện áp đo thực tế và thông mạch ở đầu module để chốt
nguyên nhân; những dữ liệu này chưa có.

## Artifact và khả năng khôi phục

- FIP 20 kHz hiện boot trên board: SHA256
  `f76825ff3eeebabce30e90db34cb2946300cab6db83d44e04820811c83735bc6`.
- FIP trước thử nghiệm được lưu trên board tại
  `/boot/fip-before-thermal-debug.bin`, SHA256
  `cdb81d9356432d077c83055219999d55dac1648dfc9d284717fb455c62714fc8`.
- Bản 100 kHz mới ở `App/build/fip-thermal.bin`; bản 20 kHz ở
  `App/build/fip-thermal-20khz.bin`. Cả hai được đóng gói từ FIP đang boot tốt
  của board; script kiểm tra BL2, DDR, monitor và loader giữ nguyên.

Test host T0/T1, driver T3 và 6 test bridge/viewer đều đạt. Chưa thu được
khung ảnh/nhiệt từ module thật vì bus I2C chưa có ACK.

## Ảnh hồng trong Web Viewer sau khi sửa chân I2C

Ảnh chụp người vận hành cung cấp cho thấy `MODE_IMAGE`, `status=0`,
`payload_bytes=98304`, nhưng `index_image=index_temp=vtemp=65535`, cờ
`FROZEN|HIGH_GAIN`, 16 byte đầu payload đều `FF`, và pixel góc/trung tâm đều
`Y=U=V=255`. Màu hồng là kết quả chuyển YUYV toàn `FF` sang RGB; chưa phải
ảnh cảnh nhiệt từ camera. `status=0` chỉ chứng minh hàm nhận đủ byte SPI, không
chứng minh camera đã trả dữ liệu hợp lệ. Header toàn `FF` có thể vượt kiểm tra
bit `frame_valid` cũ. Cần đo CS, SCK, MISO và GND chung ở cả board và module;
kiểm tra pinmux, dây MISO, SPI mode và nguồn module. Chưa thể kết luận chính xác
nguyên nhân điện trên đường SPI chỉ từ ảnh chụp.

Tầng 3 đã bổ sung từ chối header 32 byte toàn `FF` và payload 96 KB toàn `FF`,
trả lỗi thay vì báo `status=0`/hiển thị ảnh hồng. Test host thêm hai ca này,
`make test` và `make riscv-check` đều đạt; firmware RISC-V đã build thành
`App/build/cvirtos.bin`.

## Nạp firmware kiểm tra MISO

FIP đang chạy trước khi nạp có SHA256
`21db6662cf2b4cb13ba68bec21561c8e1c97f1ff5863f05ae6c9902ac6e4c5d7`;
đã sao lưu trên board tại `/boot/fip-before-ff-guard.bin`. Đóng gói lại từ
FIP này, chỉ thay RTOS, giữ nguyên BL2/DDR/monitor/loader. FIP mới
`App/build/fip-thermal-ff-guard.bin` có SHA256
`ba85d11eb2d5ba5e3d53c4c20c9b2932978e6c0082f8abb67f6581a02c74bec3`;
đã nạp `/boot/fip.bin`, reboot và xác nhận cùng checksum sau boot.

Sau reboot đã unbind `4180000.spi0` khỏi Linux `dw_spi_mmio`, chạy bridge
`/root/thermal_bridge --resources-owned --bind 0.0.0.0 --port 9000`.
Viewer HTTP tại `127.0.0.1:8080` được khởi động lại và chọn MODE_IMAGE.
Kết quả lặp lại: `status=-2`, `seq=0`, `payload_bytes=0`: driver chặn header SPI
lỗi, không truyền payload giả. Kiểm tra thanh ghi khi rảnh: FMUX SCK/CS/MOSI/MISO
lần lượt là `6/3/6/6`, PADCTRL đều `0x44`, GPIOC17 (CS) là output HIGH,
SPI0 BAUDR=`16` (11,718,750 Hz) và cấu hình Mode 3.

Đổi riêng PADCTRL MISO (FPC2.17 / `PAD_MIPI_TXP1`, `0x03001C7C`) từ pull-up
`0x44` sang pull-down `0x48` trong lúc viewer lấy khung: GPIOC input của MISO
chuyển từ HIGH xuống LOW; lỗi VoSPI chuyển từ `-2` (header `FF` bị từ chối)
sang `-4` (`frame_valid` bit 0 bằng 0). Đã khôi phục `0x44` sau phép thử.
Đây là bằng chứng mạnh rằng Tiny1C chưa lái MISO đến chân FPC2.17 trong lúc
chụp. Cần kiểm tra thông mạch MISO module → FPC2.17, CS module ← FPC2.14,
SCK ← FPC2.15, MOSI ← FPC2.18, GND chung, VDDIO và chiều cáp; quan sát CS/SCK/MISO
bằng oscilloscope khi gửi CAPTURE để phân biệt dây sai, CS không đến module,
hoặc module không phát dữ liệu. Chưa thể khẳng định lỗi phần cứng cụ thể nào
chỉ từ các thanh ghi board.

Viewer đã sửa để hiển thị lỗi `VoSPI header/đường SPI lỗi` và xoá bitmap cũ
khi khung mới lỗi. Giao diện ở cổng 8080 dùng HTTP polling; bridge ở cổng 9000
dùng TCP socket, không phải WebSocket.

## Đính chính chân FPC2 theo sơ đồ board

Người vận hành cung cấp bảng chân FPC2: CS=14 (`MIPI_TX2_P`), SCK=15
(`MIPI_TX2_N`), MISO=17 (`MIPI_TX1_P`), MOSI=18 (`MIPI_TX1_N`), nguồn
`VDD1V8_SYS` tại 26/29. Các giá trị này khớp với FMUX và T0 đang chạy.
`BRINGUP_GUIDE.md` trước đây ghi sai SCK là `GPIOC13/PAD_MIPI_TXP0`;
đã sửa thành **GPIOC16/PAD_MIPI_TXM2/FPC2.15**. Nếu cáp SCK được đấu theo
chỉ dẫn cũ, Tiny1C sẽ không có xung SPI, phù hợp với MISO không được lái.
Chưa có ảnh dây nối hoặc phép đo thông mạch để xác nhận đó là nguyên nhân
trên bộ phần cứng hiện tại. MISO FPC2.17 là input của SG2002 dù bảng chân
người vận hành ghi chung `Push-Pull Out`; phía Tiny1C mới là bên xuất MISO.

Ảnh footprint và BOM do người vận hành cung cấp xác nhận FPC2 là HDGC
`0.3K-QX-31PWB` (`C2919512`). Datasheet của hãng ghi **31 tiếp điểm**,
**bottom contact** và đánh dấu `PIN 1 Point` ở bên trái hình PCB layout khi
nhìn theo chiều mũi tên cắm cáp. Trong footprint của board, pad tín hiệu 1
ở trái, 31 ở phải, pad hông 32/33 là chân cơ khí. Ảnh đầu nối trực diện mới
cho thấy ký hiệu `A` tại đầu trái và `31` tại đầu phải: chân 1 được đếm từ
đầu `A` sang phải. Suy luận trước đó rằng chân 1 nằm ở đầu trên từ ảnh 3D
đã xoay là **sai**, không được dùng để kiểm tra dây. Nếu cáp/adapter được
nhìn từ mặt đối diện thì hình có thể bị lật; đo thông mạch trên cáp thật khi
tắt nguồn để xác nhận từng dây 14/15/17/18. Chưa có bằng chứng dây đang đảo.
Danh sách người vận hành ghi `32=VDD3V3_SYS` không khớp đầu nối 31 tiếp điểm;
không đưa 3,3 V vào pad 32/33 trước khi xác minh, vì sơ đồ board trích xuất
ghi hai pad này là GND.
