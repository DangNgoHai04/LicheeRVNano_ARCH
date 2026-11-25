# Bước 4: Trình Xem & Kiểm Tra Dữ Liệu Camera Nhiệt (`pc/thermal_viewer.py`)

## Mục tiêu

Xây dựng công cụ phía máy trạm tại [`Doc_md/Tiny-1C/App/pc/thermal_viewer.py`](../App/pc/thermal_viewer.py) chỉ dùng thư viện chuẩn của Python (Zero External Dependencies) để người phát triển có thể **nhìn thấy tận mắt dữ liệu Camera nhiệt truyền sang như thế nào** ở mọi môi trường (kể cả trên máy không cài `python3-tk`).

---

## 1. Giải mã toán học & kiểm tra gói tin

1. **Kiểm tra nghiêm ngặt Header 64-byte (`read_packet`)**:
   - Đọc đúng 64 byte `HEADER = struct.Struct('<IIIi12I')`.
   - Kiểm tra `magic == 0x314D4854` (`'THM1'`), `version == 1`, `header_bytes == 64`, `mode in (0, 1, 2)`, `width == 256`, `height == (384 nếu dual, 192 cho mode đơn)`, và payload thành công khác rỗng phải khớp chính xác `98,304` byte (`MODE_IMAGE`/`MODE_TEMP`) hoặc `196,608` byte (`MODE_DUAL`).
2. **Giải mã mặt phẳng nhiệt độ `MODE_TEMP` (`thermal_rgb`)**:
   - Đọc `49,152` phần tử 16-bit Little-Endian (`array('H')`).
   - Áp dụng công thức vật lý của cảm biến Tiny1C:
     $$T(^\circ\text{C}) = \frac{\text{raw\_u16}}{64.0} - 273.15$$
   - Xác định tọa độ chính xác của điểm lạnh nhất `min_xy = (min_x, min_y)`, điểm nóng nhất `max_xy = (max_x, max_y)`, và điểm tâm `(128, 96)`.
   - Ánh xạ sang bảng màu giả nhiệt (Xanh dương $\rightarrow$ Xanh lơ $\rightarrow$ Vàng $\rightarrow$ Đỏ) để hiển thị, giữ nguyên số liệu gốc cho đo kiểm.
3. **Giải mã mặt phẳng ảnh `MODE_IMAGE` (`yuyv_rgb`) và `MODE_DUAL` (`decode_frame`)**:
   - Giải mã từng cụm 4 byte `[Y0, U, Y1, V]` thành 2 điểm ảnh RGB24.
   - Ở `MODE_DUAL`, ghép nửa trên `256×192` (ảnh YUYV từ gói VoSPI `0xAA`) và nửa dưới `256×192` (bản đồ nhiệt từ gói VoSPI `0xCC`) thành khung `256×384`.

---

## 2. Các chế độ vận hành tích hợp trong `thermal_viewer.py`

Mặc định `--fps` là `25` (giá trị hợp lệ trong `(0, 25]`).

### 2.1. Chế độ 1: CLI Data Inspector (`--cli` / `--inspect`)
- Hỗ trợ `--mode temp|image|dual|all` (mặc định `all`), `--frames N`, `--dump-dir <thư_mục>`, `--no-ansi`.
- In báo cáo giải mã từng trường của Header 64-byte kèm chuỗi Hex gốc và `format_i2c_diag()` nếu `THERMAL_START` lỗi.
- Hiển thị 16 byte đầu của từng mặt phẳng (`image_plane`, `temp_plane`).
- Lập bảng đối chiếu từ địa chỉ `byte_offset` $\rightarrow$ 2 byte Little-Endian Hex $\rightarrow$ `raw_u16` $\rightarrow$ `Kelvin` $\rightarrow$ `Celsius` tại 4 vị trí đặc trưng: Góc trên trái `(0, 0)`, Tâm `(128, 96)`, Điểm lạnh nhất `Min` và Điểm nóng nhất `Max`.
- Vẽ **Bản đồ nhiệt ASCII / ANSI 64×16** trực tiếp trên Terminal kèm nhiệt độ theo từng hàng ngang.
- Tùy chọn `--dump-dir <thư_mục>` tự động xuất bộ 4 tệp cho mỗi khung hình: `.bin`, `.json`, `.ppm`, `.bmp`.

### 2.2. Chế độ 2: Built-in Web Browser Viewer (`--run stream` hoặc `--web <HTTP_PORT>`)
- `--run stream` tự chọn `--web 8080` nếu chưa chỉ định cổng; mở `ThreadingHTTPServer` nội bộ phục vụ HTML5 tại `http://127.0.0.1:<port>`.
- Endpoint `/frame.bmp` (24-bit BMP), `/status.json` (header, chẩn đoán lỗi, `render_ms`) và `POST /cmd?op=t|i|d|f`.
- `CameraWorker` bật `TCP_NODELAY`, dùng `queue.Queue(maxsize=1)` chỉ giữ khung mới nhất đang chờ hiển thị, tự kết nối lại sau `1.5s` nếu lỗi; nếu `status != 0` (`-2`, `-3`, `-4`), UI hiển thị thông báo lỗi VoSPI và xóa bitmap về đen.

### 2.3. Chế độ 3: Thu dữ liệu thô (`--run data`)
- Dùng `run_data()` nhận và kiểm tra ABI từng gói qua TCP (`TCP_NODELAY`) mà không dựng RGB/GUI, hỗ trợ `--mode temp|image|dual`, `--count N` (`0` = chạy liên tục tới Ctrl+C) và `--output <file.thm>`.
- Mỗi khung được ghi nối tiếp đúng định dạng `[64B THM1 header via pack_header()][payload_bytes payload]`, in thống kê mỗi giây ra `stderr` (`seq | fps | MiB/s | receive=ms`).

### 2.4. Chế độ 4: Quét địa chỉ I2C (`--scan`) & Desktop GUI (`tkinter`)
- `--scan`: gửi lệnh `'?'` (`THERMAL_INFO`) để đọc kết quả address scan `0x08..0x77` và `format_i2c_diag(h)` sau khi `START` gặp lỗi I2C.
- Khi không truyền `--cli`/`--web`/`--run`/`--scan`, mở cửa sổ `Tkinter` (`512×384` hoặc `512×768`, `Auto scale` / `Min..Max °C`, nút `Luu frame`); nếu thiếu `python3-tk`, tự chuyển về `--cli`.

## 3. Tối ưu đường xử lý ảnh & cách chạy test

- **Tối ưu hiệu năng**: `thermal_rgb()` ánh xạ bảng `THERMAL_LUT` theo `set(raw)` và hỗ trợ `render=False` khi chỉ cần thống kê; `yuyv_rgb()` có nhánh nhanh `GRAY_LUT` khi mọi byte `U = V = 0x80`; `encode_bmp()` đảo RGB $\rightarrow$ BGR theo slice và `run_web()` dựng ảnh ngoài khóa `threading.Lock()`.
- **Chạy kiểm thử tự động & mô phỏng trên PC**:

```bash
# Chạy đủ 11 bài unittest trong tests/test_pipeline.py (FastPaths, Pipeline, RuntimeModes)
make -C Doc_md/Tiny-1C/App test

# Hoặc chạy trực tiếp với bridge giả lập:
./Doc_md/Tiny-1C/App/build/thermal_bridge_host --simulate --port 9000 &
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py 127.0.0.1 --port 9000 --cli --mode all --frames 1
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py 127.0.0.1 --port 9000 --run stream --mode temp --fps 25
python3 Doc_md/Tiny-1C/App/pc/thermal_viewer.py 127.0.0.1 --port 9000 --run data --mode dual --count 3 --output /tmp/tiny1c-dual.thm
```

Xem thêm [App README](../App/README.md) và [Lộ trình đọc T3 → App](../LO_TRINH_DOC_T3_APP.md).
