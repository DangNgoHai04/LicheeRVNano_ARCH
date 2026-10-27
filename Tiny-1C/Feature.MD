# TỔNG HỢP DANH MỤC TÍNH NĂNG TOÀN DIỆN CẢM BIẾN NHIỆT TINY-1C (ASIC-2121W)

> **Tài liệu tham chiếu chính**:
> 1. Tài liệu nhà sản xuất: `RTOS平台SPI接口软件开发资料 Interface development instructionsV1.0.docx`
> 2. Bảng mô tả hàm SDK: `Tiny1C模组SDK接口说明Interface Description1.2.xlsx`
> 3. Hướng dẫn hiệu chuẩn: `用户开发标定 User calibration instructions`
> 4. Toàn bộ mã nguồn C & Python SDK: `Tiny1C_STM32_SPI_SDK`, `RTOS_V1`, `test_v1`, `test_v2`, `test_v3`, `model_ml1`

---

## 1. TỔNG QUAN KIẾN TRÚC & HỆ THỐNG GIAO TIẾP

Module cảm biến nhiệt InfiRay **Tiny-1C** (trang bị chip xử lý ASIC **2121W / RS001 ISP**) cung cấp giải pháp đo nhiệt độ bức xạ hồng ngoại bước sóng dài (LWIR 8~14 µm) với độ phân giải thực **256 × 192 pixels**. Hệ thống hoạt động theo mô hình giao tiếp kép:
* **Kênh Dữ liệu (VOSPI - Video Over SPI)**: Băng thông cao, truyền luồng ảnh số YUV422 hoặc ma trận nhiệt RAW16 Radiometric theo thời gian thực (tối đa 25 FPS).
* **Kênh Điều khiển (I2C / CCI Protocol)**: Quản lý tập lệnh Vendor Command (VDCMD) để thiết lập thông số quang học, thuật toán FFC, đo đạc nhiệt độ điểm/đường/vùng, hiệu chuẩn và nâng cấp firmware.

```
+-----------------------------------------------------------------------------------+
|                        MODULE TINY-1C (ASIC-2121W / RS001)                        |
+-----------------------------------------------------------------------------------+
       │ Kênh Điều khiển: I2C (400 kHz, CCI)     │ Kênh Dữ liệu: VOSPI (Mode 3, 20~75MHz)
       ▼                                         ▼
+───────────────────────────────────────────────────────────────────────────────────+
|                           NỀN TẢNG HOST (RTOS / LINUX)                            |
|  - Tầng Driver: Synopsys DW I2C, SPI Master + Hardware/Software GPIO Chip Select   |
|  - Tầng VDCMD: Đóng gói Short / Standard / Long Commands                          |
|  - Tầng Streaming: Bắt tay VOSPI 0xAA/0xCC, bóc tách Frame Header 32-byte         |
|  - Tầng Nghiệp vụ: Đo nhiệt (Point/Line/Rect), Lưới Cell 8x8, Báo cháy NPU AI     |
|  - Tầng Căn chỉnh: Cân bằng NUC/FFC, Chống kẹt trập, Hiệu chuẩn môi trường        |
+───────────────────────────────────────────────────────────────────────────────────+
```

---

## 2. NHÓM TÍNH NĂNG 1: GIAO THỨC TRUYỀN DỮ LIỆU HÌNH ẢNH & NHIỆT ĐỘ (VOSPI)

### 2.1. Đặc tính vật lý bus VOSPI
* **Chuẩn truyền thông**: Motorola Serial Peripheral Interface (SPI) 4-wire (SCK, MOSI, MISO, CS/NSS).
* **Chế độ SPI (SPI Mode)**: **SPI Mode 3** (`CPOL = 1` - SCK ở mức cao khi nghỉ; `CPHA = 1` - chốt dữ liệu ở sườn thứ hai). Hỗ trợ truyền 8-bit MSB first.
* **Tần số hoạt động**: Tần số phần cứng ASIC hỗ trợ lên đến **75 MHz**. Khuyến nghị:
  * Chế độ Dual (256×384): $f_{\text{SCK}} \ge 39.32\,\text{MHz}$ ($256 \times 384 \times 2 \times 8 \times 25\,\text{bps}$).
  * Chế độ Single (256×192): $f_{\text{SCK}} \ge 19.66\,\text{MHz}$ ($256 \times 192 \times 2 \times 8 \times 25\,\text{bps}$).

### 2.2. Các chế độ xuất luồng (Output Modes)
1. **Chế độ Ảnh đơn sắc/Màu giả (IMAGE_OUTPUT - 256×192)**:
   * Header đồng bộ: `0xAA`.
   * Định dạng dữ liệu: **YUV422 (YUYV)**, 2 bytes/pixel (tổng 98,304 bytes/frame).
   * Dùng để hiển thị hình ảnh trực quan trên màn hình LCD (chuyển đổi YUV422 $\rightarrow$ RGB565) hoặc nén video RTSP/H.264.
2. **Chế độ Ma trận Nhiệt độ bức xạ (TEMP_OUTPUT - 256×192)**:
   * Kích hoạt qua: `y16_preview_start(PREVIEW_PATH0, Y16_MODE_TEMPERATURE)`.
   * Header đồng bộ: `0xCC`.
   * Định dạng dữ liệu: **RAW16 (Y16 Radiometric)**, 2 bytes/pixel (14-bit có nghĩa, Little-Endian).
   * Công thức quy đổi ra độ C:
     $$T_{\text{Celsius}} = \frac{\text{Raw}_{16}}{64} - 273.15$$
3. **Chế độ Kép đồng thời (IMAGE_AND_TEMP_OUTPUT - 256×384)**:
   * Kích thước khung hình mở rộng $256 \times 384$ pixels (tổng 196,608 bytes/frame).
   * Nửa trên (192 dòng đầu): Dữ liệu hình ảnh YUV422.
   * Nửa dưới (192 dòng sau): Dữ liệu nhiệt độ RAW16.
   * Cho phép CPU/NPU vừa render hình ảnh vừa đo đạc nhiệt độ từng pixel trong cùng một frame.
4. **Chế độ Xuất xen kẽ (Interval Frame Output)**:
   * Dành cho MCU/Host có băng thông SPI hạn chế: Cứ $N$ frame ảnh (ví dụ 4 frame YUV) thì chèn 1 frame nhiệt RAW16, giúp giảm tải tần số SCK mà vẫn duy trì giám sát nhiệt.

### 2.3. Cấu trúc Khung hình VOSPI & Frame Header 32-Byte
Một frame VOSPI tiêu chuẩn bao gồm:
1. **Lệnh Master "Read a New Frame"**: Kéo CS = LOW, gửi chu kỳ mồi Dummy Cycles (tối thiểu 512 chu kỳ xung SCK) để ASIC truy xuất dữ liệu từ DDR nội.
2. **VOSPI Frame Header (32 Bytes)**:
   * **Byte 0**: `FrameValid` (Bit 0 = 1: Dữ liệu hợp lệ; Bit 0 = 0: Dữ liệu không hợp lệ).
   * **Byte 1**: `Reserved`.
   * **Byte 2-3**: `FrameIndex_L`, `FrameIndex_H` (Số thứ tự khung hình 16-bit).
   * **Byte 4**: `Shutter State` (0 = Đang đóng; 1 = Đang mở).
   * **Byte 5-6**: `Vtemp_L`, `Vtemp_H` (Giá trị nhiệt độ thô cảm biến đo được tại buồng bolometer).
   * **Byte 7-8**: `Shutter temp` (Nhiệt độ lá trập, trên Tiny-1C mặc định 0).
   * **Byte 9**: `Gain State` (1 = High Gain; 0 = Low Gain).
   * **Byte 10**: `Temp Area` (Luôn là 1 trên Tiny-1C).
   * **Byte 11**: `Overexposure State` (0 = Bình thường).
   * **Byte 12**: `Pixel Freeze State` (1 = Ảnh đang bị đóng băng; 0 = Luồng bình thường).
   * **Byte 13–31**: `Reserved`.
3. **Vùng Pixel Payload**: 192 dòng (mỗi dòng 512 bytes = 256 pixel × 2 bytes).
4. **Tail Dummy Bytes**: Dãy byte 0 kết thúc sau khi hoàn tất stream.

---

## 3. NHÓM TÍNH NĂNG 2: GIAO THỨC ĐIỀU KHIỂN LỆNH QUA I2C (VDCMD)

### 3.1. Đặc tính bus điều khiển I2C
* Địa chỉ I2C Slave: `0x78` (dạng 8-bit) tương đương `0x3C` (dạng 7-bit). Tuân thủ chuẩn CCI (Camera Control Interface) trong MIPI CSI-2.
* Địa chỉ thanh ghi: 16-bit Index (MSB first, Big-Endian).
* Cơ chế Multi-byte: Hỗ trợ gói 2-byte hoặc 4-byte có canh lề địa chỉ.
* Thanh ghi đồng bộ trạng thái: `0x0200` (`I2C_VD_BUFFER_STATUS`), cho phép Host polling cờ BUSY (`0x01`) và cờ RESULT PASS/FAIL (`0x02`) để biết thời điểm ASIC hoàn thành xử lý.

### 3.2. Cấu trúc 3 cấp lệnh Vendor Command (VDCMD)
* **Short Command (`short_cmd_read`, `short_cmd_write`)**: Dành cho các lệnh điều khiển nhanh hoặc dữ liệu $\le 4$ bytes (cấu trúc header `vdcmd_short_header_t`).
* **Standard Command (`standard_cmd_read`, `standard_cmd_write`)**: Dành cho truy xuất tham số theo khối bộ đệm thông thường (địa chỉ 32-bit `dwAddr`, độ dài `wLen`, chia nhỏ thành các gói $\le 256$ bytes qua `I2C_VD_BUFFER_RW` và `I2C_VD_BUFFER_HLD`).
* **Long Command (`long_cmd_read`, `long_cmd_write`)**: Dành cho cấu hình phức tạp yêu cầu 2 vùng địa chỉ 32-bit (`dwAddr1`, `dwAddr2`), ví dụ: tọa độ điểm/đường/vùng đo nhiệt độ hoặc nạp khối thuộc tính.

---

## 4. NHÓM TÍNH NĂNG 3: ĐIỀU KHIỂN CAMERA & MÀN TRẬP (SHUTTER / FFC)

| Tên Hàm / Tính Năng | Mô Tả & Cơ Chế Hoạt Động | Ghi Chú Kỹ Thuật |
| :--- | :--- | :--- |
| **`preview_start`** | Khởi động luồng video của cảm biến với tham số: Path (0/1), Source, Width, Height, FPS (tối đa 25), Mode (8: SPI output). | Bắt buộc gọi khi khởi tạo hệ thống. |
| **`preview_stop`** | Dừng luồng phát video của cảm biến. | Giải phóng tài nguyên đường truyền. |
| **`y16_preview_start`** | Chuyển mạch đường ống nội bộ của ISP sang xuất ma trận nhiệt độ RAW16 (Y16). | Áp dụng cho độ phân giải 256×192. |
| **`y16_preview_stop`** | Dừng chế độ xuất ma trận nhiệt độ Y16, quay lại ảnh thông thường. | |
| **`ooc_b_update`** | Kích hoạt đóng/mở màn trập cơ học để thực hiện hiệu chuẩn đồng nhất mặt phẳng (NUC / FFC - Flat Field Correction / B-Update). | Loại bỏ hiện tượng trôi nhiệt và nhiễu lưới cảm biến. |
| **`shutter_manual_switch`** | Cưỡng bức mở hoặc đóng lá màn trập (`SHUTTER_OPEN` / `SHUTTER_CLOSE`). | Dùng cho mục đích chẩn đoán phần cứng. |
| **`shutter_sta_get`** | Đọc trạng thái hoạt động hiện tại của màn trập và cờ kích hoạt. | Kiểm tra lá trập đang che hay mở. |
| **`cur_vtemp_get`** | Đọc nhiệt độ thô hiện tại của cảm biến (`Vtemp`). | Đơn vị nội bộ chip ASIC. |
| **`set_prop_auto_shutter_params`** | Cấu hình tham số tự động đóng màn trập (Auto FFC):<br>• `SHUTTER_PROP_SWITCH`: Bật/tắt tự động.<br>• `SHUTTER_PROP_MIN_INTERVAL`: Thời gian tối thiểu giữa 2 lần đóng trập.<br>• `SHUTTER_PROP_MAX_INTERVAL`: Thời gian tối đa bắt buộc phải đóng trập.<br>• `SHUTTER_PROP_TEMP_THRESHOLD_B`: Ngưỡng biến thiên nhiệt $\Delta V_{\text{temp}}$ để tự kích hoạt FFC.<br>• `SHUTTER_PROP_ANY_INTERVAL`: Hạn chế chu kỳ đóng trập quá dày đặc tránh quá nhiệt cuộn hút. | Tham số lưu vào Flash qua lệnh `spi_config_save`. |
| **Tính năng Chống Kẹt / Rơi Trập (`Fall Protection`)** | Khi module bị rung lắc, va đập hoặc rơi tự do, lá màn trập có thể bị kẹt đóng ngoài ý muốn. Firmware tích hợp thuật toán phân tích phương sai hình ảnh; nếu độ phân tán pixel nhỏ bất thường (ảnh đen đồng nhất), chip tự phát xung GPIO đẩy mở lại màn trập. | Bật/tắt bằng `SHUTTER_PROP_PROTECT_SWITCH`, đặt ngưỡng độ nhạy qua `SHUTTER_PROTECT_THR_HIGH_GAIN` và `SHUTTER_PROTECT_THR_LOW_GAIN`. |

---

## 5. NHÓM TÍNH NĂNG 4: ĐO NHIỆT ĐỘ CHUYÊN NGHIỆP (TPD - TEMPERATURE MEASUREMENT)

### 5.1. Đo nhiệt độ cực trị toàn khung
* **`tpd_get_max_min_temp_info`**: Truy vấn đồng thời nhiệt độ cao nhất ($T_{\text{max}}$), nhiệt độ thấp nhất ($T_{\text{min}}$) toàn khung cùng với tọa độ pixel chính xác $(X_{\text{max}}, Y_{\text{max}})$ và $(X_{\text{min}}, Y_{\text{min}})$.
* **`tpd_get_max_temp` / `tpd_get_min_temp`**: Truy vấn nhanh giá trị cực đại/cực tiểu toàn khung (dạng số nguyên 16-bit).

### 5.2. Đo nhiệt độ Điểm / Đường / Vùng (Point, Line, Rectangle TPD)
* **Đo điểm (`tpd_get_point_temp_info`)**: Nhận tọa độ $(x, y)$, trả về nhiệt độ tức thời tại pixel đó.
* **Đo đoạn thẳng (`tpd_get_line_temp_info`)**: Nhận điểm đầu $(x_1, y_1)$ và điểm cuối $(x_2, y_2)$, tính toán:
  * Nhiệt độ trung bình trên đoạn ($T_{\text{avg}}$).
  * Nhiệt độ cực đại ($T_{\text{max}}$) và tọa độ điểm nóng nhất trên đoạn.
  * Nhiệt độ cực tiểu ($T_{\text{min}}$) và tọa độ điểm lạnh nhất trên đoạn.
* **Đo vùng chữ nhật (`tpd_get_rect_temp_info`)**: Nhận tọa độ góc $(x, y)$ và kích thước $(w, h)$, tính toán:
  * Nhiệt độ trung bình toàn vùng ($T_{\text{avg}}$).
  * Điểm nóng nhất trong vùng ($T_{\text{max}}$, $X_{\text{max}}, Y_{\text{max}}$).
  * Điểm lạnh nhất trong vùng ($T_{\text{min}}$, $X_{\text{min}}, Y_{\text{min}}$).
* **Cảnh báo vượt ngưỡng vùng (`tpd_set_rect_temp_thd`)**: Đặt ngưỡng cảnh báo quá nhiệt cho từng khung đo hình chữ nhật.

### 5.3. Xử lý phân tích Lưới 2 Chiều 8×8 (Grid 64 Cells - Realtime Edge AI)
* Hiện thực trong module `grid.c` / `grid.h` của `RTOS_V1`:
  * Chia ma trận $256 \times 192$ thành **64 ô Cell** ($8 \times 8$), mỗi ô quản lý khối $32 \times 24 = 768$ pixels.
  * Mỗi Cell tính toán độc lập: $T_{\text{avg}}$, $T_{\text{max}}$, $T_{\text{min}}$, tọa độ điểm nóng cục bộ, và **tốc độ biến thiên nhiệt $\Delta T$** so với khung hình trước đó.
  * Theo dõi nhãn thời gian thực `frame_time_t` (Giờ, Phút, Giây, Mili-giây, Số thứ tự Frame).
  * Tự động quét kiểm tra mảng 2 chiều và phát cờ báo động `alarm_active` khi có Cell vượt ngưỡng nhiệt định trước (mặc định $70.0^\circ\text{C}$).

---

## 6. NHÓM TÍNH NĂNG 5: XỬ LÝ HÌNH ẢNH & ISP (IMAGE ENHANCEMENT)

1. **Bảng màu giả ngoại vi (Pseudo-Color / Palette Selection)**:
   * Chức năng: `pseudo_color_set(PREVIEW_PATH0, color_type)` / `pseudo_color_get`.
   * Hỗ trợ **11 chế độ bảng màu giả lập** chuyên dụng cho cứu hỏa, quân sự và kiểm tra công nghiệp: White Hot, Black Hot, Ironbow, Rainbow, High Contrast Rainbow, Amber, v.v.
2. **Thu phóng kỹ thuật số (Digital Zoom)**:
   * **Zoom trung tâm**: `zoom_center_up` (Phóng to từ tâm), `zoom_center_down` (Thu nhỏ về tâm).
   * **Zoom theo tọa độ tùy chọn**: `zoom_position_up`, `zoom_position_down` (Phóng to/thu nhỏ tập trung vào một đối tượng quan tâm tại vị trí $X, Y$).
   * Tỉ lệ bước phóng tùy chỉnh theo các nấc bước (Scale steps).
3. **Cân chỉnh chất lượng hình ảnh (Image ISP Tuning)**:
   * Hàm: `set_prop_image_params` / `get_prop_image_params`.
   * Tinh chỉnh: Độ sáng (Brightness), Độ tương phản (Contrast), Tăng cường chi tiết (Detail Enhancement), Khử nhiễu không gian (Spatial Denoise).

---

## 7. NHÓM TÍNH NĂNG 6: CĂN CHỈNH & HIỆU CHUẨN THIẾT BỊ (CALIBRATION)

### 7.1. Hiệu chuẩn Điểm mù / Điểm chết (Dead Pixel Correction - DPC)
* Hiện tượng: Cảm biến vi nhiệt kế sau thời gian sử dụng, va đập hoặc nhiễm tĩnh điện ESD có thể xuất hiện các pixel hỏng (luôn quá sáng hoặc quá tối).
* Phương thức xử lý:
  1. **Hiệu chuẩn tự động (`dpc_auto_calibration`)**: Chip tự động thống kê và nội suy bù trừ các pixel bất thường. Thời gian cấu hình càng dài, độ chính xác càng cao.
  2. **Thêm/xóa thủ công (`dpc_add_point`, `dpc_remove_point`)**: Cho phép người dùng nhập trực tiếp tọa độ $(X, Y)$ của điểm lỗi vào bảng Dead Pixel Table trong Flash.

### 7.2. Hiệu chuẩn Nhiễu Nắp che / Quang sai vỏ (Lid Pattern Noise / Rim Cover - 锅盖标定)
* Mục đích: Khử nhiễu vân vòng tròn đồng tâm sinh ra do phản xạ nhiệt từ vỏ ống kính, vòng ren hoặc nắp che bảo vệ.
* Thao tác: Đặt tấm kim loại phẳng đen mờ (Anodized matte aluminum) cách ống kính $\approx 5\,\text{mm}$ ở nhiệt độ phòng.
* Lệnh thực hiện: Gọi `rmcover_auto_calc` (thời gian tính toán khoảng $5\,\text{s}$), bật/tắt hiệu ứng bằng `rmcover_sts_switch`.

### 7.3. Hiệu chuẩn Đo nhiệt Thứ cấp (Secondary Radiometric Calibration - 二次标定)
* Mục đích: Bù trừ sai số nhiệt độ sau khi module được tích hợp vào cụm vỏ thiết bị thương mại hoàn chỉnh (có lắp thêm kính bảo vệ Germanium/Chalcogenide hoặc thay đổi phân bố tản nhiệt).
* Phương thức:
  1. **Hiệu chuẩn 1 điểm (`tpd_ktbt_recal_1point`)**: Đưa nguồn nhiệt chuẩn (Blackbody) vào tầm nhìn, gọi lệnh cân chỉnh một mức nhiệt. Thời gian xử lý khoảng $5\sim 10\,\text{s}$.
  2. **Hiệu chuẩn 2 điểm (`tpd_ktbt_recal_2point`)**: Dùng 2 nhiệt độ Blackbody chuẩn (nhiệt độ thấp $T_1$ và nhiệt độ cao $T_2$) để hiệu chỉnh toàn bộ đường cong độ dốc $K$ và độ lệch $B$. Thời gian xử lý khoảng $15\,\text{s}$. Dữ liệu ghi trực tiếp vào Flash.

### 7.4. Hiệu chỉnh Biến số Môi trường (Ambient Variable Correction)
* Các tham số môi trường ảnh hưởng đến kết quả đo nhiệt hồng ngoại:
  * Hệ số phát xạ bề mặt vật thể ($\varepsilon$ - Emissivity, từ 0.01 đến 1.00).
  * Khoảng cách đến mục tiêu ($d$ - Target Distance, mét).
  * Nhiệt độ phản xạ môi trường xung quanh ($T_{\text{refl}}$ / $T_u$ - Ambient Reflected Temp, Kelvin).
  * Nhiệt độ không khí quyển ($T_{\text{atm}}$ / $T_a$ - Atmospheric Temp, Kelvin).
  * Độ ẩm không khí tương đối/tuyệt đối (Humidity).
  * Hệ số truyền qua của khí quyển ($\tau$ - Atmospheric Transmissivity).
* Thư viện xử lý: Sử dụng `libirtemp` kết hợp bảng tra cứu khoảng cách (`tau.bin` / `1-transmittance.xlsx`) trên thẻ nhớ SD hoặc bộ nhớ nhúng để bù trừ nhiệt lượng khí quyển hấp thụ và phát xạ.

---

## 8. NHÓM TÍNH NĂNG 7: QUẢN TRỊ HỆ THỐNG & CẬP NHẬT FIRMWARE (SYSTEM & OTA)

1. **Nâng cấp Firmware từ xa qua I2C (`update_firmware`)**:
   * Hỗ trợ đọc file nhị phân firmware từ hệ thống tập tin (FATFS / SD Card) và nạp qua I2C.
   * Quy trình nâng cấp tự động:
     * Chuyển trạng thái ASIC sang chế độ Bootloader ROM (`sys_reset_to_rom`).
     * Kiểm tra nhà sản xuất Flash SPI ngoài (hỗ trợ Macronix `0xC2`, Winbond `0xEF`, GigaDevice `0xC8`).
     * Xóa các sector chứa mã lệnh trong Flash (`spi_erase_sector`, `spi_erase_fw`).
     * Từng khối 4096 bytes dữ liệu được ghi vào Flash và đọc ngược lại để kiểm tra tính toàn vẹn (Verify).
     * Ghi nhãn bộ đệm (`spi_write_tag`) và ra lệnh khởi động lại hệ thống (`sys_reset_to_update_fw`).
2. **Khôi phục Cài đặt Gốc (`restore_default_cfg`)**:
   * `DEF_CFG_ALL`: Khôi phục toàn bộ thông số hình ảnh, màn trập, môi trường về mặc định nhà máy (không làm mất dữ liệu hiệu chuẩn Blackbody).
   * `DEF_CFG_TPD`: Khôi phục thông số hiệu chuẩn đo nhiệt.
   * `DEF_CFG_DEAD_PIXEL`: Xóa trắng bảng điểm mù phụ nạp thêm, đưa về bảng điểm mù gốc.
3. **Lưu / Nạp Thông số Cấu hình (`spi_config_save` / `spi_config_load`)**:
   * Cho phép lưu các cài đặt Auto Shutter, Image Parameters, Palette vào phân vùng cấu hình Flash SPI để không bị mất khi ngắt nguồn.
4. **Truy vấn Thông tin Thiết bị (`get_device_info`)**:
   * Đọc mã định danh phần cứng (ASIC Version, Sensor ID).
   * Đọc phiên bản Firmware đang chạy (Firmware Version).
   * Đọc số Serial Number (SN) của module.

---

## 9. NHÓM TÍNH NĂNG 8: TÍCH HỢP HỆ THỐNG ĐA NHÂN & TRÍ TUỆ NHÂN TẠO (AI/ML & IPC PIPELINE)

### 9.1. Giao tiếp Đa nhân Heterogeneous Dual-Core (RTOS Core $\leftrightarrow$ Linux Core)
* Được tối ưu hóa chuyên biệt trong thư mục [RTOS_V1](file:///home/duong/Workspace/Back-up/Tiny1C-SDK-Backup/Tiny1C-SDK/ASIC_2121W_RTOS_SDK_V1.0.0_release/RTOS_V1):
  * **Bộ nhớ chia sẻ Shared DDR RAM (`0x8FE80000`)**: Cấu trúc dữ liệu chuẩn `shared_data_t` chứa Magic `0x54494E59`, địa chỉ khung hình RAW, chế độ hoạt động, ma trận kết quả 64 cell và thời gian thực.
  * **Đồng bộ bộ nhớ đệm (Cache Coherency)**: Tự động thực hiện Flush D-Cache C906L (`flush_dcache_range` căn chỉnh 64-byte) và rào cản bộ nhớ `fence rw, rw` trước khi phát ngắt.
  * **Giao tiếp Mailbox IPC**:
    * Chiều RTOS $\rightarrow$ Linux: Bắn ngắt Mailbox (`CMD_THERMAL_GRID_READY`, `CMD_THERMAL_ALARM_ALERT`) qua `cvi_rtos_send_cmdqu` và hàng đợi `E_QUEUE_CMDQU`.
    * Chiều Linux $\rightarrow$ RTOS: Tiếp nhận lệnh qua `E_QUEUE_CAMERA` (`CMD_THERMAL_SHUTTER_REQ` đóng trập NUC, `CMD_THERMAL_SET_MODE` đổi chế độ ảnh).

### 9.2. Chuỗi Xử lý Phát hiện Cháy & Phân tích Nhiệt 2 Giai đoạn (NPU AI Pipelines)
* Triển khai trong các module `test_v1`, `test_v2`, `test_v3`, `model_ml1`:
  * **Giai đoạn 1 (Spatial Feature Extractor - 1 Hz)**:
    * Chạy mô hình lượng tử hóa BF16 trên NPU Sophgo TPU (`thermal_feature_extractor_8x8_cv181x_bf16.cvimodel`).
    * Trích xuất ma trận tensor $[1, 3, 8, 8]$: Kênh 0 = $T_{\text{avg}}$, Kênh 1 = $T_{\text{max}}$, Kênh 2 = $\Delta T$.
    * Phân tích 64 ô lưới Cell kết hợp với 1 chỉ số bức xạ toàn cảnh (Đánh giá 64+1).
  * **Giai đoạn 2 (Temporal Aggregator & Risk Classifier - 0.2 Hz)**:
    * Gom cụm 5 frame liên tiếp ($5\,\text{s}$) để tính toán động học ngọn lửa:
      * Tốc độ gia nhiệt bậc một: $\frac{dT}{dt}$ ($^\circ\text{C/s}$).
      * Gia tốc gia nhiệt bậc hai: $\frac{d^2T}{dt^2}$.
      * Gradient không gian: $|\nabla T|$ và phương sai nhiệt độ toàn cảnh $\sigma^2$.
    * Đưa ra phân loại 3 mức rủi ro: **SAFE** (An toàn), **SUSPECT** (Nghi ngờ), **FIRE ALARM** (Báo cháy khẩn cấp).
  * **Mô phỏng & Replay Kịch bản Cháy (`replay_fire.py`, `generate_dataset.py`)**:
    * Cung cấp công cụ mô phỏng dữ liệu nhiệt độ không cần phần cứng camera thật để phục vụ kiểm thử đơn vị và benchmark trên máy trạm x86 hoặc trực tiếp trên bo mạch.

---

## 10. BẢNG TỔNG HỢP ÁNH XẠ CHỨC NĂNG & TẬP TIN MÃ NGUỒN

| Hạng Mục Tính Năng | Tập Tin Mã Nguồn / Tài Liệu | API / Hàm Chức Năng Tiêu Biểu |
| :--- | :--- | :--- |
| **Giao thức VOSPI** | `stream_spi.h`, `stream_spi.c`, `hal_spi0_dsi.c` | `spi_stream_init`, `spi_get_frame`, `spi0_dsi_transfer_burst` |
| **Giao thức I2C / VDCMD** | `data_rw.h`, `data_rw.c`, `vdcmd.h`, `vdcmd.c` | `data_rw_i2c_hw_init`, `short_cmd_write`, `standard_cmd_read`, `long_cmd_read` |
| **Luồng Video & Màn trập** | `falcon_cmd.h`, `falcon_cmd.c`, `core.c` | `preview_start`, `y16_preview_start`, `ooc_b_update`, `set_prop_auto_shutter_params` |
| **Chống kẹt màn trập** | `falcon_cmd.h`, `falcon_cmd.c` | `SHUTTER_PROP_PROTECT_SWITCH`, `SHUTTER_PROTECT_THR_HIGH_GAIN` |
| **Đo nhiệt độ (TPD)** | `falcon_cmd.h`, `falcon_cmd.c` | `tpd_get_max_min_temp_info`, `tpd_get_point_temp_info`, `tpd_get_rect_temp_info` |
| **Lưới Cell 8x8 & Báo động** | `grid.h`, `grid.c`, `main.c` | `grid_process_2d`, `grid_check_2d`, `thermal_camera_task` |
| **Bảng màu & Zoom ảnh** | `falcon_cmd.h`, `falcon_cmd.c` | `pseudo_color_set`, `zoom_center_up`, `zoom_position_up` |
| **Hiệu chuẩn Điểm mù (DPC)** | `falcon_cmd.h`, `Dead pixel correction/` | `dpc_auto_calibration`, `dpc_add_point`, `dpc_remove_point` |
| **Hiệu chuẩn Nhiễu nắp che** | `falcon_cmd.h`, `Lid pattern noise/` | `rmcover_auto_calc`, `rmcover_sts_switch` |
| **Hiệu chuẩn Thứ cấp** | `falcon_cmd.h`, `Secondary calibration/` | `tpd_ktbt_recal_1point`, `tpd_ktbt_recal_2point` |
| **Bù sai số Môi trường** | `temperature.h`, `temperature.c`, `libirtemp.h` | `calculate_org_env_cali_parameter`, `calculate_new_env_cali_parameter` |
| **Nâng cấp Firmware** | `firmware.h`, `firmware.c` | `update_firmware`, `sys_reset_to_rom`, `spi_erase_sector` |
| **Liên kết Đa nhân IPC** | `ipc.h`, `ipc.c` | `ipc_init`, `ipc_notify_grid_ready`, `ipc_notify_alarm`, `ipc_process_linux_cmds` |
| **Mô hình NPU & AI Pipeline** | `test_v1/`, `test_v2/`, `test_v3/` | `integrated_pipeline.py`, `ml1_pipeline.py`, `test_pipeline.py` |
