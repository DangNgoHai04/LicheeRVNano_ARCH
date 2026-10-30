# Hướng dẫn đọc source driver SPI tầng 0–2

**Đối tượng:** người đọc và phát triển driver SPI0 dùng chung trên SG2002/CV181x FreeRTOS. Tài liệu này mô tả source hiện có, không phải báo cáo nghiệm thu phần cứng. Kiến trúc và lộ trình ở [tài liệu tổng quan](spi_driver_architecture_and_roadmap.md).

**Trạng thái:** T0–T2 triển khai polling và đã compile/link ở lần build trước. DMA/IRQ chưa hoạt động; clock PLL và waveform của source hiện tại chưa được đo trên board.

## 1. Đọc theo thứ tự nào

| Bước | File | Câu hỏi cần trả lời |
| --- | --- | --- |
| 1 | [`dw_spi_regs.h`](../../freertos/cvitek/hal/cv181x/spi/src/dw_spi_regs.h) | Register và bit DW SSI nào được sử dụng? |
| 2 | [`hal_spi_platform.c`](../../freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform.c) | Clock, reset, FPC2 và GPIO CS được chuẩn bị ra sao? |
| 3 | [`hal_dw_spi.h`](../../freertos/cvitek/hal/cv181x/spi/include/hal_dw_spi.h) | State và API nối T0 với T1/T2 là gì? |
| 4 | [`hal_dw_spi.c`](../../freertos/cvitek/hal/cv181x/spi/src/hal_dw_spi.c) | Controller phát/thu và dọn lỗi thế nào? |
| 5 | [`spi.h`](../../freertos/cvitek/driver/spi/include/spi.h) | Caller được phép yêu cầu điều gì? |
| 6 | [`spi.c`](../../freertos/cvitek/driver/spi/src/spi.c) | T2 bảo vệ bus và ghép các bước HAL thành một giao dịch thế nào? |
| 7 | Các `CMakeLists.txt` của HAL/driver | Source được build và cài header ra sao? |

Source T1 đặt `snapshot()` và `abort()` ở đầu file, nhưng nên đọc theo thứ tự vận hành: `init → configure → begin → poll → finish → snapshot → abort`. Sau khi hiểu T0–T2, một task ngoại vi bất kỳ có thể làm ví dụ caller; protocol của task không thuộc driver SPI chung.

## 2. T0 — platform SG2002 và board

**Vai trò:** biến yêu cầu “bus SPI0 với clock và chân ngoài” thành register clock/reset/pad/pinmux/GPIO cụ thể. Không truyền frame SPI và không có mutex FreeRTOS.

### 2.1. Các nhóm register đã set

| Nhóm | Source dùng để làm gì |
| --- | --- |
| `CLOCK_GATE_APB`, `CLOCK_GATE_SPI` | Cấp clock truy cập APB và clock chạy SPI0 |
| `CLOCK_BYPASS`, `CLOCK_DIV_SPI` | Chọn XTAL hoặc đường FPLL/divider |
| `RESET_BANK1` | Deassert soft reset SPI0; source không phát reset pulse mới |
| `GPIOC_DATA`, `GPIOC_DIR` | Đặt GPIOC17 output, CS active-low |
| `PINMUX_MIPI_TX*` | Nối FPC2 sang SCK, SDO, SDI và GPIO CS |
| `PADCTRL_MIPI_BASE`, các register D-PHY | Đặt pad và đường digital qua D-PHY cho FPC2 |

Địa chỉ và mask cụ thể nằm ở đầu `hal_spi_platform.c`. [`top_reg.h`](../../freertos/cvitek/hal/cv181x/config/top_reg.h) có thêm tên SPI/clock dùng chung và [`hal_pinmux.c`](../../freertos/cvitek/hal/cv181x/pinmux/src/hal_pinmux.c) có case `PINMUX_SPI0`. **Đường chạy T0 hiện tại dùng MMIO trực tiếp**, không gọi helper `hal_pinmux_config()`; hai file này không thay thế recipe trong `hal_spi_platform.c`.

### 2.2. Hàm và thứ tự thực thi

1. `hal_spi_platform_prepare(hw, force_xtal, external_pins)` zero `hal_spi_t`, chọn clock, bật gate, deassert reset và readback. Chỉ sau readback thành công mới gán `base=0x04180000` và `clock_hz`. `force_xtal=1` khai báo 25 MHz; `force_xtal=0` đặt divider FPLL/8 và khai báo 187.5 MHz. Giá trị PLL là giả định source, cần đo trên board.
2. `hal_spi_time_us()` đổi system time sang microsecond cho deadline T1/T2.
3. Sau khi T1 init controller, `hal_spi_platform_connect(hw)` gọi `prepare_fpc2()` nếu `external_pins=1`. Hàm nạp CS cao trước khi chuyển mux GPIO, cấu hình tám register D-PHY, bốn pad, rồi mux SCK/SDO/SDI. Nó readback GPIO/mux/pad trước khi báo thành công.
4. `hal_spi_platform_cs(hw, active)` kéo GPIOC17 thấp khi `active=true`, cao khi false, rồi readback output. Khi không dùng chân ngoài, hook trả OK mà không đổi pad.

`update()` là read-modify-write từng register để giữ các bit khác. Nó không phải khóa liên lõi. Ứng dụng phải bảo đảm Linux, display, GPIO khác và code trực tiếp MMIO không cùng sở hữu các tài nguyên này.

### 2.3. Tại sao T0 tách khỏi T1

Clock/pad/pinmux là đặc thù board/SoC; cách vận hành FIFO và CTRLR0 thuộc IP DW SSI. Nếu chuyển sang board khác nhưng vẫn dùng DW SSI, có thể giữ phần lớn T1/T2 và thay T0. Nếu board dùng chân CS khác, chỉ T0 và cấu hình capability cần đổi; protocol slave không phải biết địa chỉ GPIO.

## 3. T1 — DesignWare SSI controller HAL

**Vai trò:** điều khiển register SPI, không biết task, slave hay nội dung dữ liệu. [`hal_dw_spi.h`](../../freertos/cvitek/hal/cv181x/spi/include/hal_dw_spi.h) định nghĩa `hal_spi_t` (base, clock, FIFO depth, bit width, trạng thái) và các mã lỗi nội bộ.

### 3.1. Từng bước trong vòng đời

| Hàm | Việc thực hiện | Điều kiện quan trọng |
| --- | --- | --- |
| `hal_spi_init` | Tắt IMR/DMACR/SSI/SER; probe TX FIFO depth; đặt mặc định | T0 đã cấp clock/base; chân ngoài chưa nối trong luồng T2 |
| `hal_spi_configure` | Kiểm tra mode 0–3, bits 4–16; tính BAUDR; ghi CTRLR0/BAUDR và readback | Controller idle, clock/base/fifo hợp lệ; `max_hz>0` |
| `hal_spi_begin` | Bật SSI/SER rồi gọi T0 assert CS | Đã configure; chưa active |
| `hal_spi_poll` | Truyền và nhận full-duplex từng frame; kiểm deadline/RISR | Đã active; số frame > 0 |
| `hal_spi_finish` | Chờ TFE và `!BUSY`, không xóa cờ lỗi | Chưa nhả CS |
| `hal_spi_snapshot` | Đọc SR và RISR trước cleanup | T2 giữ kết quả để chẩn đoán |
| `hal_spi_abort` | Tắt request/SSI/SER, nhả CS qua T0, kiểm readback | Gọi cả thành công lẫn lỗi |

BAUDR là số chẵn tối thiểu 2. T1 làm tròn lên để SCK tính toán không vượt `max_hz`, đồng thời áp trần `SPI_MAX_HZ=46,875,000` trong `dw_spi_regs.h`. `actual_hz` dựa trên `hw->clock_hz`; nó không phản ánh sai lệch clock nguồn hay tín hiệu ngoài pad.

### 3.2. Vì sao polling vừa ghi TX vừa đọc RX

DW SSI ở TMOD full-duplex: mỗi frame TX tạo một frame RX. Kể cả caller không cần dữ liệu nhận, T1 phải đọc DR để tránh RX FIFO đầy. T1 theo dõi `sent` và `got`, chỉ xếp thêm TX khi `sent-got < fifo_depth` và TX FIFO còn chỗ. Một task bị preempt có thể tạo khoảng trống SCK; driver không hứa clock liên tục. `received` tăng sau khi đọc RX, kể cả khi `rx==NULL`.

Nếu `tx==NULL`, HAL ghi `fill_word` để tạo clock. Nếu `rx==NULL`, dữ liệu nhận bị bỏ sau khi drain. Bit cao ngoài độ rộng frame bị mask. Với 4–8 bit, buffer dùng phần tử `uint8_t`; 9–16 bit dùng `uint16_t`.

### 3.3. Lỗi và giới hạn T1

T1 kiểm `RISR` cho lỗi controller và deadline trong vòng polling/finish. `finish()` chỉ xác nhận dữ liệu đã ra dây, còn `abort()` luôn thực hiện cleanup. Việc ghi `DMACR=0` hiện chỉ tắt request DMA của SPI; **nó chưa dừng một channel DMAC**, nên không thể dùng code này như backend DMA. Mode TX-only/RX-only chuyên biệt, IRQ và DMA chưa được cài đặt.

## 4. T2 — bus/device API FreeRTOS

**Vai trò:** public API trong `spi.h`, một bus SPI0, pool tĩnh bốn device và một mutex. T2 không ghi register SoC trực tiếp.

### 4.1. Các kiểu dữ liệu public

- `spi_bus_config_t`: chọn XTAL/PLL và có nối chân FPC2 hay chỉ loopback nội.
- `spi_device_config_t`: `max_hz`, mode, bit order, bits/frame, CS, loopback, CS setup/hold. Source hiện chấp nhận CS index 0, MSB-first.
- `spi_segment_t`: TX/RX/fill và số **frame SPI**, không phải số byte của một packet giao thức. Một message có 1–32 segment.
- `spi_message_t`: mảng segment, timeout chung và backend. Hiện chỉ `SPI_BACKEND_POLL` được nhận.
- `spi_result_t`: SCK tính toán, số segment/frame nhận được, snapshot SR/RISR, `transfer_status` và `cleanup_status`.

### 4.2. `spi_bus_init()` và `spi_device_bind()`

`spi_bus_init(0, cfg)` tạo mutex tĩnh, gọi T0 `prepare`, T1 `init`, T0 `connect`, rồi mới công bố `ready=true`. Init lần nữa khi bus ready trả BUSY. Lỗi giữa init không rollback toàn bộ shared register. `spi_device_bind()` kiểm tra capability, sao chép config vào một slot tĩnh; bind/unbind phải được caller tuần tự hóa, không unbind trong khi transfer còn dùng handle.

### 4.3. `spi_transfer()` theo thứ tự thực tế

1. Zero `result`, lấy thời điểm bắt đầu. Kiểm handle, message, timeout, segment count, frame count, căn chỉnh buffer, tràn số học và mọi cặp TX/RX chồng lấn; backend không hỗ trợ bị từ chối trước khi phát xung.
2. Tính một deadline tuyệt đối cho cả lời gọi, gồm validation và chờ mutex. Chờ mutex theo tick hữu hạn; sau khi lấy khóa, kiểm lại `ready` và deadline.
3. Gọi T1 `configure` theo device, rồi `begin` để assert CS một lần. Chờ `cs_setup_us` nếu có.
4. Gọi `hal_spi_poll()` lần lượt cho từng segment, không nhả CS giữa các đoạn. Cộng `frames_received`, tăng `segments_completed` chỉ khi đoạn hoàn tất.
5. Gọi `finish` để chờ bit cuối ra dây. Nếu thành công, chờ `cs_hold_us`. Luôn snapshot SR/RISR rồi `abort`; lưu cả lỗi truyền và lỗi cleanup.
6. Nếu cleanup lỗi, đặt bus NOT_READY, trả IO. Nhả mutex trên mọi đường đã lấy khóa.

Sơ đồ trình tự có trong [tài liệu kiến trúc](spi_driver_architecture_and_roadmap.md#5-luồng-truyền-kết-thúc-và-lỗi).

### 4.4. Ý nghĩa các lỗi public

| Mã | Ý nghĩa thực tế |
| --- | --- |
| `SPI_ERR_ARGUMENT` | Handle, cấu hình, buffer hoặc message không hợp lệ |
| `SPI_ERR_UNSUPPORTED` | Bus/CS/bit order/backend hoặc tốc độ ngoài khả năng source |
| `SPI_ERR_NOT_READY` | Bus chưa init hoặc đã bị khóa sau cleanup lỗi |
| `SPI_ERR_TIMEOUT` | Hết deadline khi chờ mutex, truyền hoặc chờ idle |
| `SPI_ERR_IO` | Controller, pin/CS hoặc cleanup báo lỗi |
| `SPI_ERR_BUSY` | Bus đã init hoặc pool device hết slot |

Khi return lỗi, `frames_received` chỉ cho biết số frame đã drain, không khẳng định slave đã xử lý bao nhiêu dữ liệu. T2 không tự retry vì lặp một message có thể tạo tác dụng phụ ở thiết bị.

## 5. Các file được thêm và sửa để ghép build

| File | Vai trò |
| --- | --- |
| [`hal/cv181x/spi/CMakeLists.txt`](../../freertos/cvitek/hal/cv181x/spi/CMakeLists.txt) | Build T0/T1 thành object `halspi`, install HAL header |
| [`hal/cv181x/CMakeLists.txt`](../../freertos/cvitek/hal/cv181x/CMakeLists.txt) | Đưa thư mục `spi` vào danh sách HAL |
| [`driver/spi/CMakeLists.txt`](../../freertos/cvitek/driver/spi/CMakeLists.txt) | Build T2 thành object `spi`, install public header |
| [`driver/CMakeLists.txt`](../../freertos/cvitek/driver/CMakeLists.txt) | Chọn driver SPI khi build CV181x |
| [`top_reg.h`](../../freertos/cvitek/hal/cv181x/config/top_reg.h), [`hal_pinmux.c`](../../freertos/cvitek/hal/cv181x/pinmux/src/hal_pinmux.c) | Định nghĩa SoC và helper pinmux SPI0; T0 hiện dùng register trực tiếp |

Các file task kiểm thử, linker telemetry và tài liệu slave cụ thể ở ngoài T0–T2. Chúng là caller hoặc công cụ quan sát, không phải điều kiện để API SPI chung tồn tại.

## 6. Ranh giới trước khi phát triển tiếp

Baseline phần mềm hiện dùng được để đọc và phát triển tầng trên qua `spi.h`, với các giới hạn polling, một bus và một CS. Để dùng như nền cho tải dữ liệu lớn cần hoàn thiện backend DMA và xác minh ownership DMAC/cache. Để gọi là nghiệm thu phần cứng cần đo clock, waveform, dữ liệu và phục hồi lỗi trên board mục tiêu. Không suy ra kết quả đo từ `actual_hz` hay từ việc compile/link thành công.
