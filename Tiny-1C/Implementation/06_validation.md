# Bước 6: Nhật ký kiểm chứng build, test và dữ liệu mô phỏng

## Đối chiếu lại ngày 25-11-2026

Toàn bộ phần T0–T3 và `thermal_service.c` đã nằm độc lập trong `freertos/` và vượt qua toàn bộ kiểm thử tự động sau khi đồng bộ:
- Host test SPI T0–T2 (`run.sh`, 9 ca trong `test_spi.c`), host test I²C T0–T2 (`run.sh`), cross-compile RISC-V I²C (`compile-riscv.sh` với `-Wall -Wextra -Werror`).
- Ba bộ host test T3 (`test_driver`, `test_i2c_failure`, `test_vospi`) và `riscv-check` tại [`freertos/cvitek/driver/sensor/tiny1C/`](../../../freertos/cvitek/driver/sensor/tiny1C/README.md).
- Build đầy đủ firmware `cvirtos.bin` (cấu hình I²C 20 kHz với `-DTHERMAL_TINY1C=ON` và kiểm chứng build mặc định `-DTHERMAL_TINY1C=OFF`).
- Build `thermal_bridge_host`, `thermal_bridge` (static RISC-V musl) và chạy đủ **11 bài test** trong `Doc_md/Tiny-1C/App/tests/test_pipeline.py`.

Kết quả capture trên module thật lấy từ [nhật ký 18/11](09_board_bringup_2026-11-18.md). [Lộ trình tái hiện trên PC](../LO_TRINH_DOC_T3_APP.md#6-chạy-từ-mô-phỏng-đến-kiểm-tra-board)

---

## 0. Quy trình chạy toàn bộ Test (T0 → T3 → Firmware → App)

Chạy từ thư mục gốc workspace (`/home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test`):

```bash
# 1. Test SPI0 (T0-T2): POLL, IRQ, DMA, AUTO, LSB-first, Helpers, Fault Recovery (9 test cases)
bash freertos/cvitek/hal/cv181x/spi/tests/run.sh

# 2. Test I2C5 Bitbang (T0-T2) & Cross-compile RISC-V với header FreeRTOS thật
bash freertos/cvitek/hal/cv181x/i2c/tests/run.sh
bash freertos/cvitek/hal/cv181x/i2c/tests/compile-riscv.sh

# 3. Test Driver T3 Tiny1C & Cross-compile RISC-V (gồm rtos/thermal_service.c)
make -C freertos/cvitek/driver/sensor/tiny1C clean test riscv-check

# 4. Build Bridge (Host + RISC-V Linux static) & 11 bài test toàn tuyến App
make -C Doc_md/Tiny-1C/App all linux test

# 5. Build firmware FreeRTOS (cấu hình 20 kHz bring-up) & xác nhận độc lập khỏi Doc_md
rm -rf freertos/cvitek/build/task
THERMAL_I2C_FREQ_HZ=20000 ./Doc_md/Tiny-1C/App/scripts/build_firmware.sh
! grep -q 'Doc_md' freertos/cvitek/build/task/build.ninja
grep -q ' T thermal_service_task$' Doc_md/Tiny-1C/App/build/cvirtos.symbols
```

---

## 1. Kiểm thử Đơn vị & Tiêm lỗi Driver Tầng 3 (`freertos/cvitek/driver/sensor/tiny1C`)

Lệnh thực thi:
```bash
make -C freertos/cvitek/driver/sensor/tiny1C clean test riscv-check
```

Kết quả đạt 100% (`exit code 0`):
- **`build/test_driver`**:
  - Xác nhận khóa an toàn tài nguyên: `core_init(5, 0x78)` bị từ chối (`-22 / IR_I2C_DEVICE_OPEN_FAIL`) khi chưa gọi `data_rw_set_gpioa_ownership(true)`.
  - Sau khi xác nhận quyền sở hữu GPIOA, `core_init` khởi tạo thành công I2C5 (`0x78 -> 7-bit 0x3C`) và đọc đầy đủ thông tin thiết bị (`TINY1C-ASIC2121W`, `FW-V3.9.0-RELEASE`, `PN-TINY1C-256192`, `SN-SG2002-000188`).
  - Xác nhận kiểm tra tham số (`mode=99`, `fps=0`, buffer `NULL`, palette `2` Reserved), `spi_stream_init()` cấu hình SPI0 (`23,437,500 Hz` lúc init qua adapter tương thích), sau đó `core_start()` đặt SCK về `11,718,750 Hz` (`THERMAL_SPI_DEFAULT_SCK_HZ` / `THERMAL_SPI_DUAL_SCK_HZ`) cho cả `MODE_TEMP` và `MODE_DUAL`, thu thành công khung 96 KB và 192 KB rồi `core_stop()`.
- **`build/test_i2c_failure`**:
  - Mô phỏng đứt cáp I2C (`i2c_read`/`i2c_write` trả về `-1`): `core_init` trả về `IR_NOT_GET_DEVICE_INFO`, `probed == false`, và `core_start` từ chối khởi chạy luồng.
- **`build/test_vospi`**:
  - Kiểm chứng 5 kịch bản tiêm lỗi VoSPI + 1 ca thu kép thành công:
    - `inject=1` (lỗi SPI đọc Header 512B): trả `-2`, tăng `header_err`, gọi `hal_spi_recover_bus()`.
    - `inject=2` (Header có `frame_valid == 0`): trả `-4`, tăng `frames_invalid`, không đọc Payload 96 KB.
    - `inject=3` (lỗi SPI đọc Payload 96KB): trả `-3`, tăng `payload_err`, gọi `hal_spi_recover_bus()`.
    - `inject=5` (Header toàn `0xFF`): trả `-2`, tăng `header_err`.
    - `inject=4` (Payload toàn `0xFF`): trả `-3`, tăng `payload_err`.
    - `inject=0` (`MODE_DUAL` 192 KB): 4 giao dịch SPI (`0xAA` $\rightarrow$ 96KB ảnh $\rightarrow$ `0xCC` $\rightarrow$ 96KB nhiệt, `dummy_fill = 0x55`); thống kê cuối: `frames_invalid == 1`, `header_err == 2`, `payload_err == 2`, `recovered == 2`.
- **`riscv-check` (Biên dịch với Toolchain RISC-V thực `riscv64-unknown-elf-gcc`)**:
  - Biên dịch sạch toàn bộ 6 tệp `src/*.c` và `rtos/thermal_service.c` với `-std=gnu11 -O2 -Wall -Wextra -Werror`.
  - Xác nhận bảng ký hiệu (`nm`) liên kết tới `spi_transfer_one` (`hal_spi.o`), `soft_i2c_read_reg16` (`data_rw.o`), `xQueueSemaphoreTake` (`vdcmd.o`) và xuất `T thermal_service_task` (`thermal_service.o`).

---

## 2. Kiểm thử Firmware RTOS, Module Mailbox Linux & Đóng gói FIP

Các tệp nhị phân hiện có trong [`Doc_md/Tiny-1C/App/build/`](../App/build):

| Tệp đích | Kích thước | Thông tin kiểm chứng |
| :--- | :---: | :--- |
| [`cvirtos.bin`](../App/build/cvirtos.bin) | `98,000 B` | Build 25/11 (`I2C 20 kHz`, `SPI0 11.71875 MHz`) từ nguồn độc lập trong `freertos/`; SHA256 `69b63b255828eab86b47cd26e87e9ed7aed053120b0be1076a6c9b5308e17cb2`. Chứa `thermal_service_task`, `core_start`, `spi_get_frame`, `spi_transfer_one`, `soft_i2c_read_reg16`, `bb_gpio_platform_init`. |
| [`fip-thermal.bin`](../App/build/fip-thermal.bin) | `582,144 B` | Bản FIP bring-up 18/11 (`I2C 20 kHz` + quét offset VoSPI `64..480` đã chạy thành công trên board); SHA256 `425592b4b6cf50ed13226278145b258af14e7bee36829af7b915c023b1dfd790`. *(Chú ý: cần chạy `package_fip.py` lại nếu muốn đóng gói `cvirtos.bin` mới build vào FIP.)* |
| [`fip-thermal-20khz.bin`](../App/build/fip-thermal-20khz.bin) | `581,632 B` | Bản lưu trữ thử nghiệm 13/11 (`20 kHz`, trước đợt sửa chân và quét offset VoSPI). |
| [`soph_rtos_cmdqu.ko`](../App/build/soph_rtos_cmdqu.ko) | `62,776 B` | Module mailbox Linux đã rebuild cho kernel `5.10.260-tag-20260926-00002-g235afa875600`. |
| [`thermal_bridge`](../App/build/thermal_bridge) | `412,776 B` | Binary RISC-V Linux (`riscv64-unknown-linux-musl-gcc -static`), SHA256 `071e089dc77acb55c810b14e9424f47a68e36af56574e0c032879aab668855c4`. |
| [`thermal_bridge_host`](../App/build/thermal_bridge_host) | `21,632 B` | Binary Host PC hỗ trợ `--simulate`, SHA256 `3662a9e2c6b8a5ec4bf8757fd30cbc2f5b6aa490310a99f780041e141700b546`. |

---

## 3. Kiểm thử Tự động Toàn tuyến (`test_pipeline.py`)

Lệnh thực thi:
```bash
make -C Doc_md/Tiny-1C/App all linux test
```

Kết quả thực thi (11 bài test thuộc 3 lớp `FastPaths`, `Pipeline`, `RuntimeModes`):
```text
python3 -m unittest discover -s tests -v
test_bmp_pixels_and_padding (test_pipeline.FastPaths) ... ok
test_grayscale_matches_color_formula (test_pipeline.FastPaths) ... ok
test_stats_without_render_and_latest_frame (test_pipeline.FastPaths) ... ok
test_cli_inspector_and_bmp_export (test_pipeline.Pipeline) ... ok
test_hardware_requires_resource_confirmation (test_pipeline.Pipeline) ... ok
test_i2c_diag_decoding (test_pipeline.Pipeline) ... ok
test_invalid_packet (test_pipeline.Pipeline) ... ok
test_modes_and_conversion (test_pipeline.Pipeline) ... ok
test_temperature_endian_and_scale (test_pipeline.Pipeline) ... ok
test_data_recording_all_modes (test_pipeline.RuntimeModes) ... ok
test_web_stream_commands_and_status (test_pipeline.RuntimeModes) ... ok

----------------------------------------------------------------------
Ran 11 tests in 0.559s

OK
```

---

## 4. Log dữ liệu mô phỏng qua TCP (`thermal_viewer.py --cli`)

Lệnh thực thi khi `thermal_bridge_host --simulate` đang chạy để quan sát 3 chế độ dữ liệu (`MODE_TEMP`, `MODE_IMAGE`, `MODE_DUAL`) và lưu các tệp mẫu ra [`Doc_md/Tiny-1C/App/build/sample_frames/`](../App/build/sample_frames):

```bash
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py 127.0.0.1 --port 19090 --cli --mode all \
    --no-ansi --dump-dir Doc_md/Tiny-1C/App/build/sample_frames
```

### 4.1. Log Gói #1 — Chế độ Nhiệt Độ (`MODE_TEMP`, `98,304` byte Y16)

```text
========================================================================================
[GÓI DỮ LIỆU CAMERA NHIỆT TINY1C] seq=1 | MODE_TEMP  (1 - RAW Y16 256x192, 98304 bytes)
========================================================================================
1. HEADER 64-BYTE (thermal_header_t - Little-Endian ABI):
   Hex (00..31): 54 48 4d 31 01 00 00 00 40 00 00 00 00 00 00 00 01 00 00 00 01 00 00 00 00 01 00 00 c0 00 00 00
   Hex (32..63): 00 80 01 00 92 3d d6 63 01 00 00 00 01 00 00 00 09 00 00 00 90 0b 00 00 00 00 00 00 00 00 00 00
   - magic / version / hdr : 0x314D4854 ('THM1') / v1 / 64 bytes
   - status / seq          : 0 / 1
   - width x height        : 256 x 192 pixels (payload_bytes = 98304 B)
   - index_image / temp    : 1 / 1 (timestamp = 1674984850 ms)
   - vtemp / flags         : 2960 / 0x09 (SIMULATED | HIGH_GAIN)

2. MẶT PHẲNG NHIỆT ĐỘ BỨC XẠ (RAW Y16 Little-Endian - 256x192x2 = 98,304 bytes):
   - Phạm vi byte Payload  : offset 0 .. 98303
   - 16 byte đầu (Hex)     : c9 49 c9 49 c9 49 c9 49 c9 49 c9 49 c9 49 c9 49
   - Công thức giải mã     : Celsius = (raw_u16 / 64.0) - 273.15
   - Thống kê toàn khung   : Min = 21.99°C tại (0, 0) | Max = 37.99°C tại (58, 67) | Tâm (128,96) = 23.49°C
   - Bảng đối chiếu byte thô -> nhiệt độ tại các tọa độ đặc trưng:
     + Góc trên trái (0, 0)     : (x=  0, y=  0) | offset=     0 | LE=[c9 49] -> raw=18889 -> 295.14 K ->  21.99 °C
     + Tâm khung hình (128, 96) : (x=128, y= 96) | offset= 49408 | LE=[29 4a] -> raw=18985 -> 296.64 K ->  23.49 °C
     + Điểm lạnh nhất Min       : (x=  0, y=  0) | offset=     0 | LE=[c9 49] -> raw=18889 -> 295.14 K ->  21.99 °C
     + Điểm nóng nhất Max       : (x= 58, y= 67) | offset= 34420 | LE=[c9 4d] -> raw=19913 -> 311.14 K ->  37.99 °C

   - Bản đồ nhiệt trực quan (256x192 thu gọn 64x16 trên Terminal):
  y=000 |                                                                |  22.0°C
  y=012 |                                                                |  22.2°C
  y=024 |                                                                |  22.4°C
  y=036 |                                                                |  22.6°C
  y=048 |                                                                |  22.7°C
  y=060 |                                                                |  22.9°C
  y=072 |            @@@@@@@@@                                           |  23.1°C
  y=084 |          @@@@@@@@@@@@@@                                        |  23.3°C
  y=096 |         @@@@@@@@@@@@@@@                                        |  23.5°C
  y=108 |          @@@@@@@@@@@@@@                                        |  23.7°C
  y=120 |............@@@@@@@@@...........................................|  23.9°C
  y=132 |................................................................|  24.1°C
  y=144 |................................................................|  24.2°C
  y=156 |................................................................|  24.4°C
  y=168 |................................................................|  24.6°C
  y=180 |................................................................|  24.8°C
========================================================================================
```

### 4.2. Log Gói #2 — Chế độ Hình Ảnh (`MODE_IMAGE`, `98,304` byte YUYV422)

```text
========================================================================================
[GÓI DỮ LIỆU CAMERA NHIỆT TINY1C] seq=2 | MODE_IMAGE (0 - YUYV422 256x192, 98304 bytes)
========================================================================================
1. HEADER 64-BYTE (thermal_header_t - Little-Endian ABI):
   Hex (00..31): 54 48 4d 31 01 00 00 00 40 00 00 00 00 00 00 00 02 00 00 00 00 00 00 00 00 01 00 00 c0 00 00 00
   Hex (32..63): 00 80 01 00 fd 3d d6 63 02 00 00 00 02 00 00 00 09 00 00 00 90 0b 00 00 00 00 00 00 00 00 00 00
   - magic / version / hdr : 0x314D4854 ('THM1') / v1 / 64 bytes
   - status / seq          : 0 / 2
   - width x height        : 256 x 192 pixels (payload_bytes = 98304 B)
   - index_image / temp    : 2 / 2 (timestamp = 1674984957 ms)
   - vtemp / flags         : 2960 / 0x09 (SIMULATED | HIGH_GAIN)

2. MẶT PHẲNG ẢNH QUANG HỌC (YUYV422 - 256x192x2 = 98,304 bytes):
   - Phạm vi byte Payload  : offset 0 .. 98303
   - 16 byte đầu (Hex)     : 22 80 22 80 23 80 23 80 24 80 24 80 25 80 25 80
   - Cụm Macropixel (0,0)  : Y0=34, U=128, Y1=34, V=128
   - Cụm Tâm (128,96)      : Y0=146, U=128, Y1=146, V=128
========================================================================================
```

### 4.3. Log Gói #3 — Chế độ Kép (`MODE_DUAL`, `196,608` byte: 96KB YUYV + 96KB Y16)

```text
========================================================================================
[GÓI DỮ LIỆU CAMERA NHIỆT TINY1C] seq=3 | MODE_DUAL  (2 - YUYV + Y16 256x384, 196608 bytes)
========================================================================================
1. HEADER 64-BYTE (thermal_header_t - Little-Endian ABI):
   Hex (00..31): 54 48 4d 31 01 00 00 00 40 00 00 00 00 00 00 00 03 00 00 00 02 00 00 00 00 01 00 00 80 01 00 00
   Hex (32..63): 00 00 03 00 36 3e d6 63 03 00 00 00 03 00 00 00 09 00 00 00 90 0b 00 00 00 00 00 00 00 00 00 00
   - magic / version / hdr : 0x314D4854 ('THM1') / v1 / 64 bytes
   - status / seq          : 0 / 3
   - width x height        : 256 x 384 pixels (payload_bytes = 196608 B)
   - index_image / temp    : 3 / 3 (timestamp = 1674985014 ms)
   - vtemp / flags         : 2960 / 0x09 (SIMULATED | HIGH_GAIN)

2. MẶT PHẲNG ẢNH QUANG HỌC (YUYV422 - 256x192x2 = 98,304 bytes):
   - Phạm vi byte Payload  : offset 0 .. 98303
   - 16 byte đầu (Hex)     : 23 80 23 80 24 80 24 80 25 80 25 80 26 80 26 80
   - Cụm Macropixel (0,0)  : Y0=35, U=128, Y1=35, V=128
   - Cụm Tâm (128,96)      : Y0=147, U=128, Y1=147, V=128

3. MẶT PHẲNG NHIỆT ĐỘ BỨC XẠ (RAW Y16 Little-Endian - 256x192x2 = 98,304 bytes):
   - Phạm vi byte Payload  : offset 98304 .. 196607
   - 16 byte đầu (Hex)     : c9 49 c9 49 c9 49 c9 49 c9 49 c9 49 c9 49 c9 49
   - Công thức giải mã     : Celsius = (raw_u16 / 64.0) - 273.15
   - Thống kê toàn khung   : Min = 21.99°C tại (0, 0) | Max = 37.99°C tại (60, 67) | Tâm (128,96) = 23.49°C
   - Bảng đối chiếu byte thô -> nhiệt độ tại các tọa độ đặc trưng:
     + Góc trên trái (0, 0)     : (x=  0, y=  0) | offset= 98304 | LE=[c9 49] -> raw=18889 -> 295.14 K ->  21.99 °C
     + Tâm khung hình (128, 96) : (x=128, y= 96) | offset=147712 | LE=[29 4a] -> raw=18985 -> 296.64 K ->  23.49 °C
     + Điểm lạnh nhất Min       : (x=  0, y=  0) | offset= 98304 | LE=[c9 49] -> raw=18889 -> 295.14 K ->  21.99 °C
     + Điểm nóng nhất Max       : (x= 60, y= 67) | offset=132728 | LE=[c9 4d] -> raw=19913 -> 311.14 K ->  37.99 °C
========================================================================================
```

### 4.4. Danh sách tệp mẫu đã xuất trong `Doc_md/Tiny-1C/App/build/sample_frames/`

- Chế độ Nhiệt (`MODE_TEMP`):
  - [`frame_temp_seq0001.bin`](../App/build/sample_frames/frame_temp_seq0001.bin) (`98,304` byte dữ liệu Y16 gốc)
  - [`frame_temp_seq0001.json`](../App/build/sample_frames/frame_temp_seq0001.json) (Header + tọa độ nhiệt độ Min/Max/Tâm)
  - [`frame_temp_seq0001.ppm`](../App/build/sample_frames/frame_temp_seq0001.ppm), [`frame_temp_seq0001.bmp`](../App/build/sample_frames/frame_temp_seq0001.bmp) (Ảnh bản đồ nhiệt `256×192`)
- Chế độ Ảnh (`MODE_IMAGE`):
  - [`frame_image_seq0002.bin`](../App/build/sample_frames/frame_image_seq0002.bin), [`frame_image_seq0002.json`](../App/build/sample_frames/frame_image_seq0002.json), [`frame_image_seq0002.ppm`](../App/build/sample_frames/frame_image_seq0002.ppm), [`frame_image_seq0002.bmp`](../App/build/sample_frames/frame_image_seq0002.bmp)
- Chế độ Kép (`MODE_DUAL`):
  - [`frame_dual_seq0003.bin`](../App/build/sample_frames/frame_dual_seq0003.bin) (`196,608` byte), [`frame_dual_seq0003.json`](../App/build/sample_frames/frame_dual_seq0003.json), [`frame_dual_seq0003.ppm`](../App/build/sample_frames/frame_dual_seq0003.ppm), [`frame_dual_seq0003.bmp`](../App/build/sample_frames/frame_dual_seq0003.bmp) (Ảnh ghép `256×384`)


---

## 5. Nhật ký chạy test từng bước từ T0 đến Board thực tế (2026-11-13)

### Bước 1: Unit Test T0–T2 Soft-I2C

Lệnh thực thi:
```bash
bash freertos/cvitek/hal/cv181x/i2c/tests/run.sh
```

Kết quả quan sát:
```text
I2C: combined read, final NACK, address NACK, stretching timeout, arbitration, busy, mutex, recovery, validation PASS
```

### Bước 2: Unit Test T0–T2 SPI0

Lệnh thực thi:
```bash
bash freertos/cvitek/hal/cv181x/spi/tests/run.sh
```

Kết quả quan sát:
```text
All SPI T0-T2 unit tests (POLL, IRQ, DMA, AUTO, LSB-first, Helpers, Recovery) passed!
```

### Bước 3: Unit Test Tầng 3 (`Driver_T3`) & Kiểm tra biên dịch chéo RISC-V

Lệnh thực thi:
```bash
make -C Doc_md/Tiny-1C/Driver_T3 test riscv-check
```

Kết quả quan sát (`exit code 0`):
- `build/test_driver`: `driver success/validation tests PASS` (kiểm tra từ chối khi chưa xác nhận quyền sở hữu `GPIOA` trả về `-22`, khởi tạo I2C5 `0x78 -> 7-bit 0x3c`, probe thông tin thiết bị, chuyển xung nhịp SPI0 `23,437,500 Hz` / `11,718,750 Hz`).
- `build/test_i2c_failure`: `disconnected camera rejected PASS` (`ir_status=-24`).
- `build/test_vospi`: `VoSPI AA/CC + invalid/header/payload error tests PASS`.
- `riscv-check`: biên dịch sạch 6 tệp `Driver_T3/src/*.c` + `App/rtos/thermal_service.c` bằng `riscv64-unknown-elf-gcc` với `-Wall -Wextra -Werror`, xác nhận liên kết tới `spi_transfer_one`, `soft_i2c_read_reg16`, `xQueueSemaphoreTake`.

### Bước 4: Build & Unit Test Tầng App (`thermal_bridge`, `thermal_viewer.py`)

Lệnh thực thi:
```bash
make -C Doc_md/Tiny-1C/App all linux test
```

Kết quả quan sát (`exit code 0`):
- Biên dịch sạch `build/thermal_bridge` bằng `riscv64-unknown-linux-musl-gcc -static`.
- `python3 -m unittest discover -s tests -v`: `Ran 6 tests in 0.293s` — `OK` (`test_cli_inspector_and_bmp_export`, `test_hardware_requires_resource_confirmation`, `test_i2c_diag_decoding`, `test_invalid_packet`, `test_modes_and_conversion`, `test_temperature_endian_and_scale`).

### Bước 5: Build Firmware RTOS & Đóng gói FIP

Lệnh thực thi:
```bash
THERMAL_I2C_FREQ_HZ=20000 ./Doc_md/Tiny-1C/App/scripts/build_firmware.sh
python3 Doc_md/Tiny-1C/App/scripts/package_fip.py --base Doc_md/Tiny-1C/App/build/fip-thermal-board-debug.bin --output Doc_md/Tiny-1C/App/build/fip-thermal.bin
```

Kết quả quan sát (`exit code 0`):
- `cvirtos.bin` (`I2C 20000 Hz`, `SPI0 11718750 Hz`), `fip-thermal.bin` SHA256: `21db6662cf2b4cb13ba68bec21561c8e1c97f1ff5863f05ae6c9902ac6e4c5d7`, giữ nguyên `BL2`, `DDR_PARAM`, `MONITOR`, `LOADER_2ND`.

### Bước 6: Kiểm thử trên Board thực tế (`192.168.1.27`)

| Hạng mục kiểm tra | Trạng thái | Kết quả quan sát thực tế |
| :--- | :---: | :--- |
| Mailbox Linux ↔ RTOS (`soph_rtos_cmdqu.ko` + `thermal_bridge`) | **Đạt** | `THERMAL_INFO` và `THERMAL_START` phản hồi tức thì, không còn timeout/treo `is_exception`. |
| Kênh I2C5 (`GPIOA15` SCL, `GPIOA27` SDA, slave `0x3C`) | **Đạt** | `0x03001908=0x44`, `0x03001924=0x44`; giao dịch sạch `BB=OK(0)`; `core_start()` đạt `status=0` (`seq=1` ở `MODE_TEMP` và `seq=170` ở `MODE_DUAL`). |
| Kênh SPI0 VoSPI (`FPC2.14/15/17/18`, `11.71875 MHz`, Mode 3 `0xC7`) | **Chưa đạt** | `SPI0 CTRLR0=0xC7`, `DSI_PHY` bypass `0x0A0D10C4=0x1F1F`, pinmux `0x030011A4..11B0` đã đúng (`6/3/6/6`), nhưng đường `MISO` (`PAD_MIPI_TXP1`) đọc về toàn `0xFF` (`index_image=65535`, `vtemp=65535`, payload `ff ff ...`). Cần kiểm tra dây/tiếp xúc `FPC2` và tương thích mức áp `1.8V` của SPI0 với module Tiny1C. |


---

## 6. Cập nhật kiểm chứng trên Board thực tế sau khi nạp DTS `boot.sd` & sửa VoSPI (2026-11-18)

Chi tiết quá trình debug xem thêm tại [09_board_bringup_2026-11-18.md](09_board_bringup_2026-11-18.md).

### 6.1. Firmware & Linux `boot.sd` đã kiểm chứng
- **Linux `boot.sd`** (từ `install/soc_sg2002_licheervnano_sd/rawimages/boot.sd`, FIT Image thuần `12,098,680` B, SHA256 `6cc88e796a68958d4e86be8554f4fe92a853b6f878127c023521ce7e72c56b31`): xác nhận `/proc/device-tree/*spi*4180000*/status` = `disabled`, không còn driver `dw_spi_mmio` bind `4180000.spi0`.
- **RTOS `fip-thermal.bin`** (`I2C 20 kHz`, `SPI0 11.71875 MHz`, có quét offset header VoSPI `64..480` trong [vospi.c](../../../freertos/cvitek/driver/sensor/tiny1C/src/vospi.c#L153-L163), SHA256 `425592b4b6cf50ed13226278145b258af14e7bee36829af7b915c023b1dfd790`): test driver T3 và test App đều đạt.

### 6.2. Bảng kết quả kiểm chứng toàn tuyến trên phần cứng thật

| Hạng mục kiểm tra | Trạng thái | Kết quả quan sát thực tế (2026-11-18) |
| :--- | :---: | :--- |
| Linux DTS tắt `spi0` (`rawimages/boot.sd`) | **Đạt** | `/proc/device-tree/*spi*4180000*/status` = `disabled`; không cần `unbind` thủ công. |
| Probe I2C5 (`0x3C`) & `core_start()` | **Đạt** | `probe_ok_mask = 0xF3` (đủ `CHIP_ID`, `FW_DATE`, `PROJECT`, `FW_VER`, `PN`, `SN`), `core_start()` trả `status = 0`. |
| Kênh SPI0 VoSPI (`MODE_TEMP`, `MODE_IMAGE`, `MODE_DUAL`) | **Đạt (truyền dẫn)** | Nhận đủ 98,304 B / 196,608 B, `status = 0`, `vtemp = 9341..9344`, `YUYV` giữ đúng `U=128, V=128` toàn bộ 192 hàng (không lỗi bit/tràn FIFO). `MODE_DUAL seq=3`: `Min=17.04°C`, Tâm `40.54°C`, `Max=43.73°C`. |
| Kênh mạng USB (`10.250.50.1`) | **Đạt** | Ping `0.571 ms`, cổng `22` và `9000` mở, ổn định hơn Wi-Fi (`192.168.1.30`). |
| Chất lượng hình ảnh cảm biến khi chạy lâu | **Cần kiểm tra nguồn/FFC** | Khi stream kéo dài (`seq=228`, `seq=370`), SPI vẫn sạch `U=V=0x80` nhưng cảm biến xuất sọc ngang (`Y0=102, Y1=14`), trôi nền `~81°C` và outlier `-98.15°C` → cần kiểm tra nguồn cấp đủ dòng (≥ 500 mA), tụ lọc và hiệu chuẩn FFC cho module Tiny1C. |
