# Bước 9: Bring-up Linux DTS (`boot.sd`), sửa offset VoSPI & kết quả thu khung thật (2026-11-14 → 2026-11-18)

Tài liệu này tổng hợp các sửa đổi và kết quả kiểm chứng thực tế trên board LicheeRV-Nano (SG2002) từ ngày `2026-11-14` đến `2026-11-18`, hoàn tất toàn tuyến truyền dẫn I2C5 + SPI0 VoSPI + Mailbox + Bridge + Web Viewer cho module Tiny1C.

---

## 1. Tắt `spi0` trong Linux Device Tree & xử lý lỗi `boot.sd` (`raw2cimg.py`)

### 1.1. Nguyên nhân board vẫn bind `4180000.spi0` trước ngày 18/11
- Trong mã nguồn Device Tree ([sg2002_licheervnano_sd.dts](../../../build/boards/sg200x/sg2002_licheervnano_sd/dts_riscv/sg2002_licheervnano_sd.dts#L139-L142) và [cv181x_asic_qfn.dtsi](../../../build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi#L19-L20)), `&spi0` đã đặt `status = "disabled";`.
- Tuy nhiên, `package_fip.py` chỉ cập nhật `/boot/fip.bin` (Bootloader + FreeRTOS), còn `.dtb` của Linux nằm trong `/boot/boot.sd`. Vì thẻ SD vẫn giữ `boot.sd` cũ nên Linux khi boot vẫn gắn `dw_spi_mmio` vào `4180000.spi0`.

### 1.2. Sự cố `Wrong Image Format for bootm command` sau khi chạy `build_kernel`
- Khi chạy `build_kernel` ([cvisetup.sh](../../../build/cvisetup.sh#L190-L201) → `pack_boot` trong [common_functions.sh](../../../build/common_functions.sh#L84-L106)):
  1. `mkimage` tạo FIT Image `boot.itb` (`12,098,684` B) và copy sang `install/soc_sg2002_licheervnano_sd/rawimages/boot.sd`.
  2. Sau đó [raw2cimg.py](../../../build/tools/common/image_tool/raw2cimg.py#L51-L141) chèn thêm **128 byte header `CIMG`** (64B File Header + 64B Chunk Header) để xuất ra `install/soc_sg2002_licheervnano_sd/boot.sd` (`12,098,812` B).
- U-Boot trên thẻ SD dùng `fatload` + `bootm` trực tiếp lên `/boot/boot.sd`, chỉ chấp nhận chuẩn FIT Image (`d0 0d fe ed`). Gặp 128 byte `"CIMG"` ở đầu file nên U-Boot dừng tại `soph#` với lỗi `Wrong Image Format for bootm command`.

### 1.3. Giải pháp & kiểm chứng trên board
- Chép trực tiếp bản FIT Image thuần [`install/soc_sg2002_licheervnano_sd/rawimages/boot.sd`](../../../install/soc_sg2002_licheervnano_sd/rawimages/boot.sd) (`12,098,680` B, magic `d0 0d fe ed`, SHA256 `6cc88e796a68958d4e86be8554f4fe92a853b6f878127c023521ce7e72c56b31`) vào `/boot/boot.sd` trên thẻ SD (giữ nguyên `fip.bin` và `fip.bin.bak`).
- Kết quả kiểm tra trên board sau khi boot (`192.168.1.30`):
  - `cat /proc/device-tree/*spi*4180000*/status` → **`disabled`**.
  - `/sys/bus/platform/drivers/dw_spi_mmio/4180000.spi0` không còn tồn tại → Linux đã nhả hoàn toàn SPI0 cho FreeRTOS, **không cần chạy lệnh `unbind` thủ công nữa**.

---

## 2. Chẩn đoán & sửa lỗi VoSPI `status = -4` (Offset Header 480 thay vì 64)

### 2.1. Chẩn đoán
- Sau khi `spi0` được tắt sạch ở Linux và đường dây SPI0 hoạt động (không còn kẹt `0xFF`), toàn bộ 6 lệnh probe I2C (`CHIP_ID`, `FW_DATE`, `PROJECT`, `FW_VER`, `PN`, `SN` → `probe_ok_mask = 0xF3`) và `core_start()` đều trả về `status = 0`, nhưng `THERMAL_CAPTURE` trả về `status = -4` (`frame_valid & 0x01 == 0`).
- Bổ sung hàm `spi_get_last_rx_dummy()` trong [vospi.c](../Driver_T3/src/vospi.c) và đọc trực tiếp mảng `s_rx_dummy[0..511]` khi `status == -4`:
  ```text
  CAP#0: status=-4, rx[64..71]=00 00 00 00 00 00 00 00, nz_off=480, nz_val=0x01, rx0=0x00
  ```
- Kết luận: Theo đặc tả VoSPI (`Configurable Dummy Cycles`), module Tiny1C trả về **480 byte dummy `0x00`** trước rồi mới xuất **32 byte `vospi_frame_header_t` tại offset `480..511`** (`480 + 32 = 512` byte). Code cũ chỉ đọc cố định tại `&s_rx_dummy[64]` nên luôn thấy `0x00`.

### 2.2. Sửa lỗi & kiểm thử
- Sửa [vospi.c](../Driver_T3/src/vospi.c#L153-L163): nếu `(s_rx_dummy[64] & 0x01) == 0`, tự động quét từ offset `64` đến `VOSPI_TX_DUMMY_LEN - sizeof(vospi_frame_header_t)` (`480`) để tìm byte đầu khác `0x00` của header 32 byte (vẫn giữ nguyên bảo vệ chống MISO treo `0xFF` và tương thích 100% các test host tại offset `64`).
- Tần số cấu hình trong firmware: I2C `20 kHz` (`THERMAL_I2C_FREQ_HZ=20000`), SPI0 `11.71875 MHz` (`187.5 MHz / 16` trong [all_config.h](../Driver_T3/include/all_config.h#L84-L90)).
- Kiểm thử tự động đều đạt:
  - `make -C Doc_md/Tiny-1C/Driver_T3 test riscv-check` → PASS (`test_driver`, `test_i2c_failure`, `test_vospi`, biên dịch chéo RISC-V sạch).
  - `make -C Doc_md/Tiny-1C/App test` → `Ran 6 tests in 0.275s — OK`.
- FIP mới [`Doc_md/Tiny-1C/App/build/fip-thermal.bin`](../App/build/fip-thermal.bin): SHA256 `425592b4b6cf50ed13226278145b258af14e7bee36829af7b915c023b1dfd790`, đã nạp vào `/boot/fip.bin` trên board.

---

## 3. Kết quả thu khung hình thực tế cả 3 chế độ (2026-11-18)

Thử nghiệm trực tiếp trên board qua [`thermal_viewer.py`](../App/pc/thermal_viewer.py) với `/root/thermal_bridge --resources-owned --bind 0.0.0.0 --port 9000`:

| Chế độ | `seq` / `status` | Kích thước | Metadata VoSPI | Dữ liệu quan sát thực tế |
| :--- | :---: | :---: | :--- | :--- |
| `MODE_TEMP` (`Y16`) | `1` / `0` | `98,304 B` | `vtemp=9342`, `flags=0x08 (HIGH_GAIN)` | 16B đầu: `48 4c 44 4c 34 4c 58 4c ...`; Góc `(0,0)=31.98°C`, Tâm `(128,96)=46.23°C`, `Min=-37.40°C`, `Max=82.48°C`. |
| `MODE_IMAGE` (`YUYV422`) | `2` / `0` | `98,304 B` | `vtemp=9341`, `flags=0x08 (HIGH_GAIN)` | 16B đầu: `3c 80 35 80 31 80 36 80 ...` (`Y0=60, U=128, Y1=53, V=128`; Tâm `Y0=111, Y1=109`). |
| `MODE_DUAL` (`YUYV + Y16`) | `3` / `0` | `196,608 B` | `vtemp=9344`, `index=0/1`, `flags=0x18 (HIGH_GAIN \| DUAL_INDEX_MISMATCH)` | YUYV: `30 80 2f 80 2b 80 2f 80 ...`; Y16: `d8 4a dc 4a d4 4a e4 4a ...` (`Min=17.04°C`, Tâm `40.54°C`, `Max=43.73°C`). |

---

## 4. Kênh mạng USB (`10.250.50.1`) & vấn đề phần cứng cảm biến còn theo dõi

### 4.1. Chuyển kết nối từ Wi-Fi sang USB Ethernet
- Đường Wi-Fi (`192.168.1.30`, độ trễ ~200 ms) dễ rớt kết nối khi stream (`Destination Host Unreachable` / `[Errno 113] No route to host`).
- Board có sẵn giao diện mạng USB trực tiếp tại **`10.250.50.1`** (trên PC hiện card `enx...` nhận IP `10.250.50.100/24`, ping `0.571 ms`, cả cổng `22` và `9000` đều mở).
- Lệnh khuyến nghị trên PC:
  ```bash
  python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py 10.250.50.1 --port 9000 --fps 8 --web 8080
  ```
- Nếu chạy lại `thermal_bridge` trên board gặp `camera owner lock: Resource temporarily unavailable` (do tiến trình cũ đang chạy nền), dừng bằng `killall thermal_bridge` rồi chạy lại với `--bind 0.0.0.0`.

### 4.2. Hiện tượng sọc ngang trên ảnh cảm biến khi stream kéo dài (`seq=228`, `seq=370`)
- Quan sát trên Web Viewer (`seq=228`, `MODE_DUAL`):
  - Toàn bộ byte `U` và `V` trong `image_plane` (`66 80 0e 80 64 80 0e 80 ...`) đều giữ `0x80` (`128`) xuyên suốt 192 hàng trong mẫu quan sát. Đây là dấu hiệu định dạng YUYV vẫn thẳng hàng. Cần kiểm tra riêng để xác định mất byte, FIFO hoặc lỗi bit.
  - Khung nhận được có hiện tượng lệch sáng chẵn/lẻ từng pixel (`Y0=102, Y1=14`), sọc ngang toàn khung, nền nhiệt trôi lên `~81°C` (`seq=370`) kèm outlier mép ảnh `-98.15°C` tại `(249, 116)` và `+118.23°C` tại `(253, 132)`.
- Hạng mục cần kiểm tra tiếp ở phần cứng module Tiny1C:
  1. Đo điện áp trực tiếp tại chân nguồn module Tiny1C **trong lúc đang stream**; cấp nguồn sạch đủ dòng (≥ 500 mA), dây nguồn/GND ngắn và có tụ lọc gần module để loại trừ nhiễu/sụt áp lên mạch analog ROIC.
  2. Thực hiện hiệu chuẩn màn trập (`FFC`) khi nguồn đã ổn định, kiểm tra tiếng đóng/mở màn trập cơ và tháo nắp ống kính.
  3. Khi xem `MODE_TEMP` bằng GUI Tkinter, có thể nhập dải màu cố định 20–45 °C để tránh vài pixel outlier ở mép làm nén dải tương phản màu.
