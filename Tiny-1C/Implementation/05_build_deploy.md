# 05 — Build, đấu nối và chạy camera

Mọi lệnh dưới đây trên PC bắt đầu ở root workspace. Các script chỉ build/package,
không tự flash, ghi thẻ SD, SSH hay thay file trên board.

## 1. Build và kiểm tra

```sh
bash freertos/cvitek/hal/cv181x/spi/tests/run.sh
bash freertos/cvitek/hal/cv181x/i2c/tests/run.sh
bash freertos/cvitek/hal/cv181x/i2c/tests/compile-riscv.sh
make -C freertos/cvitek/driver/sensor/tiny1C clean test riscv-check
make -C Doc_md/Tiny-1C/App all linux test
THERMAL_I2C_FREQ_HZ=20000 Doc_md/Tiny-1C/App/scripts/build_firmware.sh
Doc_md/Tiny-1C/App/scripts/build_mailbox.sh
python3 Doc_md/Tiny-1C/App/scripts/package_fip.py \
  --base install/soc_sg2002_licheervnano_sd/fip.bin
```

Script `build_firmware.sh` dùng mã nguồn độc lập trong `freertos/cvitek/driver/sensor/tiny1C`, config đã có `build/.config` và generated memory map trong
`build/output/sg2002_licheervnano_sd`. Target: sg2002_licheervnano_sd, cv181x,
riscv64, FAST_IMAGE_TYPE=0. Không chạy với SDK/board khác mà chưa chỉnh cấu hình.
Muốn tắt service trong build thường: cấu hình lại task với `-DTHERMAL_TINY1C=OFF`.

Sản phẩm ở `Doc_md/Tiny-1C/App/build/`:

| File | Dùng làm gì |
|---|---|
| cvirtos.elf, cvirtos.map, cvirtos.symbols | Debug, kiểm tra symbol và vùng RAM |
| cvirtos.bin | Payload FreeRTOS mới, không phải file boot Linux độc lập |
| fip-thermal.bin | FIP nền với payload RTOS thay thế |
| thermal_bridge | Chạy trên Linux RISC-V của board |
| soph_rtos_cmdqu.ko | Module mailbox đã sửa ioctl; kernel phải khớp |
| thermal_bridge_host | Mô phỏng trên PC |
| build-*.log | Log build từng phần |

`package_fip.py` dùng FIP nền chỉ định, không sửa file gốc. Script đọc lại kết quả,
so sánh payload BL2/DDR/monitor/loader và payload RTOS. FIP vừa build dựa vào FIP
workspace; chỉ dùng cho board tương ứng. Có thể cung cấp bản FIP đang boot tốt của
chính board qua --base. Tên file output có thể chọn bằng --output.

## 2. Đấu nối và dành tài nguyên

| Tín hiệu | Phía SG2002 trong T0 hiện tại | Phía camera |
|---|---|---|
| SCL | GPIOA15 / SPK_EN | SCL |
| SDA | GPIOA27 / EMMC_DAT3 | SDA |
| SCK | SPI0, MIPI_TXM2, FPC2 chân 15 | SPI clock |
| CS | GPIOC17, MIPI_TXP2, FPC2 chân 14 | SPI chip select |
| MOSI | SPI0 SDO, MIPI_TXM1, FPC2 chân 18 | SPI data input |
| MISO | SPI0 SDI, MIPI_TXP1, FPC2 chân 17 | SPI data output |
| GND | Ground chung | Ground |

Xác nhận hướng đánh số FPC và pinout của carrier camera trước khi nối. Điện áp
nguồn camera và mức I/O phải theo carrier/datasheet cụ thể; code không xác định
được điện áp nguồn của module đang cầm. FPC SPI SG2002 dùng miền 1.8V theo tài liệu
board trong repo. Soft-I2C cần pull-up và dịch mức phù hợp; không suy ra camera
chấp nhận 3.3V chỉ từ tên module.

Trước START:
- Không chạy SPI loopback/smoke-test hoặc tác vụ khác chiếm SPI0/DSI/GPIOC17.
- Linux không bind SPI0, I2C-gpio A15/A27 hoặc thay pinmux tương ứng. DTS workspace
  hiện đã disable spi0 và i2c5, nhưng cần kiểm tra DT đang chạy trên board.
- T0 Soft-I2C dùng RMW GPIOA DDR/DR: phải phối hợp với các consumer khác của bank
  GPIOA. DTS còn LED A14, key A30 và SPI-gpio A22..25; không được coi disable i2c5
  là bằng chứng toàn bank đã được cô lập. Dừng/disable consumer có thể đổi DDR/DR
  cạnh tranh, hoặc tích hợp khóa liên nhân trước khi xác nhận ownership.
- DMA mặc định tắt, thiết bị SPI được đặt `SPI_BACKEND_POLL`. Chỉ thêm
  `--dma-reserved` trong lần chạy sau khi polling đã thu khung hình đúng và
  DMA0/1 cùng các tài nguyên nền T0 thực sự dành riêng cho RTOS.
- Nguồn/reset camera đã ổn định. Service không điều khiển nguồn carrier.

`--resources-owned` là cam kết của cấu hình board, không phải thao tác tự động
unbind/dành tài nguyên. Bản bring-up mặc định polling có thể chậm hơn DMA.

## 3. Triển khai firmware và module

1. Lưu bản FIP đang boot tốt và bản module mailbox đang dùng để có thể khôi phục.
2. Trên boot partition SD đúng board, thay file boot FIP bằng `fip-thermal.bin`,
   với tên `fip.bin` theo quy trình boot đang sử dụng. Không đổi tên cvirtos.bin
   thành fip.bin. Không ghi image này vào địa chỉ flash chưa xác định.
3. Chép `thermal_bridge` vào `/root/thermal_bridge` và cấp quyền thực thi.
4. Chép module mới vào vị trí module hệ thống dùng, thông thường
   `/mnt/system/ko/soph_rtos_cmdqu.ko`; xác nhận kernel trước bằng `uname -r` và
   `modinfo soph_rtos_cmdqu.ko`. Dùng reboot để load bộ module/FIP đồng bộ.
   Không rmmod mailbox khi audio/camera/system khác còn sử dụng nó.
5. Nếu cập nhật Linux Device Tree (ví dụ đã đặt `&spi0 { status = "disabled"; };`
   trong `sg2002_licheervnano_sd.dts`) bằng `build_kernel`: **không** dùng file
   `install/soc_sg2002_licheervnano_sd/boot.sd` (vì bị `raw2cimg.py` bọc thêm 128
   byte header `CIMG` khiến U-Boot `bootm` báo `Wrong Image Format`). Phải chép
   file FIT Image thuần `install/soc_sg2002_licheervnano_sd/rawimages/boot.sd`
   (bản sao `boot.itb`, magic `d0 0d fe ed`) vào `/boot/boot.sd` trên thẻ SD.
   Sau khi boot, xác nhận bằng `cat /proc/device-tree/*spi*4180000*/status` phải ra
   `disabled` (khi đó không cần chạy lệnh `unbind` nữa).
6. Boot, kiểm tra UART RTOS không có assert; service sẽ chỉ chờ mailbox.

Artifact module hiện build cho:
`5.10.260-tag-20260926-00002-g235afa875600 preempt mod_unload riscv`.
Nếu kernel board khác, rebuild module với chính kernel/config/Module.symvers đó.

## 4. Chạy thật

Trên board (nếu đã nạp `boot.sd` mới có `spi0 = "disabled"` thì bước `unbind` tự bỏ qua; nếu gặp `camera owner lock: Resource temporarily unavailable` hãy tắt bản cũ bằng `killall thermal_bridge` trước):

```sh
if [ -e /sys/bus/platform/devices/4180000.spi0/driver ]; then
  printf 4180000.spi0 > /sys/bus/platform/drivers/dw_spi_mmio/unbind
fi
killall thermal_bridge 2>/dev/null || true
chmod +x /root/thermal_bridge
/root/thermal_bridge --resources-owned --bind 0.0.0.0 --port 9000
```

Trên PC, ưu tiên kết nối qua **USB Ethernet** của board (ví dụ `10.250.50.1`, độ trễ < 1 ms, ổn định hơn Wi-Fi; kiểm tra IP bằng `ip -br a` / `ip route | grep enx` trên PC hoặc `ip -br a` trên UART board):

```sh
# 1. Xem trực tiếp qua Web Viewer (http://127.0.0.1:8080)
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py 10.250.50.1 --run stream --mode temp --fps 25

# 2. Hoặc chỉ nhận và lưu dữ liệu thô [64B THM1 + payload] không dựng ảnh:
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py 10.250.50.1 --run data --mode temp --fps 25 --count 500 --output capture.thm
```

*(Hoặc nếu vẫn bind `127.0.0.1`, mở SSH tunnel `ssh -L 9000:127.0.0.1:9000 root@BOARD_IP` rồi nối tới `127.0.0.1`. Tham số `--fps` là giới hạn nhịp yêu cầu từ PC, không phải tốc độ cảm biến thực tế.)*

Viewer mặc định yêu cầu nhiệt đơn. Kiểm tra project/PN/SN trong UART RTOS,
status=0 và frame hợp lệ; sau đó bấm Anh YUYV hoặc Anh + Nhiet. Nếu cần đo FPS,
xem FPS thực trên viewer hoặc thống kê `--run data`; không lấy giá trị cấu hình 25 fps làm kết quả đo.

Thứ tự xác nhận trên phần cứng:

1. Chạy lệnh trên **không có** `--dma-reserved`: xác nhận probe I2C, nhận
   header VoSPI và lưu một khung nhiệt Y16; tiếp tục kiểm tra ảnh YUYV và chế độ kép.
   Đây là phép thử polling thật; các bài test host chỉ dùng dữ liệu mô phỏng.
2. Ghi nhận lỗi và FPS của polling. Nếu khung 96 KB bị timeout, kiểm tra tốc độ
   SPI thực tế, thời hạn giao dịch và CS trước khi kết luận camera lỗi.
3. Sau khi đã dành riêng DMA0/1 và xác nhận polling hoạt động, dừng bridge,
   khởi động lại RTOS/board để cấu hình bus từ đầu, rồi chạy bridge với
   `--resources-owned --dma-reserved`. So sánh nội dung khung, lỗi và FPS.

Chọn backend khi `spi_bus_init()` tạo bus. Thêm `--dma-reserved` vào một bridge
đang chạy hoặc gửi START mới trên bus đã khởi tạo không tự chuyển polling sang DMA.

## 5. Kiểm tra lỗi theo thứ tự

- SCL đứng 0 V lúc bus rảnh: pad SPK_EN/A15 có pull-down mặc định theo tài liệu
  SoC. Firmware mới chuyển pad sang pull-up nội, nhả DDR và kiểm tra mức cả SCL/SDA
  trước giao dịch. Nếu log `[T0-I2C] Idle line LOW`, đo ở A15 phía board và đầu
  SCL camera, kiểm tra VDDIO, pull-up ngoài về đúng mức, GND chung và pinmux.
  Tháo camera khi đã tắt nguồn rồi đo lại để phân biệt board với module/cáp.
  Pull-up nội chỉ giúp xác nhận mức rảnh; cần xem cạnh lên bằng oscilloscope ở
  100 kHz, có thể vẫn cần pull-up ngoài phù hợp. Không kéo 3.3 V vào pad/module
  trước khi xác nhận mức I/O cho phép.
- Nếu SCL/SDA đều HIGH khi rảnh nhưng START vẫn trả `-5`, xem UART RTOS:
  `Probe PROJECT/PN/SN failed` cho biết bước bị lỗi; `READ/WRITE ... bb_status`
  cho biết lỗi bus gốc (`-3` NACK, `-2` TIMEOUT, `-4` BUSY, `-5` ARBITRATION).
  Ghi lại log ở 100 kHz trước. Nếu nghi cạnh lên chậm, build thử riêng ở 20 kHz:
  `THERMAL_I2C_FREQ_HZ=20000 Doc_md/Tiny-1C/App/scripts/build_firmware.sh`,
  sau đó đóng gói FIP và thử lại. Log khởi tạo sẽ in tần số thực được cấu hình.
  Hai bản FIP tham chiếu trong `App/build/`: `fip-thermal.bin` (bản 18/11 cấu hình I2C `20 kHz` + quét offset VoSPI `64..480` đã thu đủ 3 chế độ trên board) và `fip-thermal-20khz.bin` (bản thử nghiệm 13/11 trước khi chỉnh chân và quét offset).
  Với firmware chẩn đoán, phản hồi `THERMAL_START` cũng chứa mã BB trong
  `reserved[0]` và bước probe/vị trí NACK/thanh ghi cuối trong `reserved[1]`;
  chạy viewer `--cli` sẽ in `probe=..., reg=..., BB=..., nack_at=...`.
  Sau START lỗi, dùng `python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py
  192.168.1.27 --port 9000 --scan` để quét địa chỉ chỉ bằng byte địa chỉ.
- START lỗi -5: probe không thành công; kiểm tra nguồn, địa chỉ 0x3C, SCL/SDA,
  pull-up, pinmux, ACK/repeated START và firmware.
- Capture lỗi -4: frame header invalid; kiểm tra offset/dummy cycles và waveform.
- Header/payload SPI lỗi: kiểm tra SCK/CS/MOSI/MISO, mức điện áp, tần số thực,
  tài nguyên IRQ/DMA. Thử single mode trước dual.
- ioctl timeout: xác nhận FIP mới đang chạy, queue camera được tạo và module
  mailbox đúng phiên bản; bridge không retry tự động để tránh response trễ.
- mmap/permission lỗi: cần quyền root, /dev/mem và reserved RAM đúng memory map.
- Viewer thiếu tkinter: cài Tk trên PC; có thể chạy test_pipeline không có GUI.
- Màu/nhiệt không đúng: phân biệt YUYV với Y16; kiểm tra endian, scale và cờ
  shutter/freeze/gain; không dùng frame trong lúc shutter đóng để đánh giá cảnh.
