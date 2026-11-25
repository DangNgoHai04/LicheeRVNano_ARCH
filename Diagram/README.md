# Kiến Trúc & Sơ Đồ Hệ Thống Camera Nhiệt Tiny1C (Tầng 0 → Tầng App)

Để đọc mã đi kèm các sơ đồ, xem [lộ trình T3 → App](../Tiny-1C/LO_TRINH_DOC_T3_APP.md).

Thư mục này chứa bộ sơ đồ kỹ thuật mô tả trọn vẹn hệ thống giao tiếp giữa **module camera nhiệt InfiRay Tiny1C (ASIC-2121W)**, **board LicheeRV-Nano (SG2002: FreeRTOS C906L + Linux C906B)** và **ứng dụng xem ảnh/nhiệt trên Host PC**, đi xuyên suốt từ **Tầng 0 (Thanh ghi phần cứng)** lên đến **Tầng App (Web/CLI Viewer)**.

## File draw.io

Các file dưới đây dùng định dạng XML `.drawio`, với khối, chữ và đường nối có thể chỉnh sửa trực tiếp:

| File | Các trang |
| :--- | :--- |
| [01_luong_chay.drawio](01_luong_chay.drawio) | 3 trang: khởi động hai nhân, trình tự START/CAPTURE/FFC/STOP, xử lý lỗi và phục hồi. |
| [02_luong_du_lieu.drawio](02_luong_du_lieu.drawio) | 5 trang: hai luồng dữ liệu, giao dịch I2C/SPI, bộ nhớ và TCP, bảng header VoSPI 32 byte, bảng header TCP 64 byte. |
| [03_tong_quan_he_thong.drawio](03_tong_quan_he_thong.drawio) | 3 trang: kiến trúc phân tầng, artifact triển khai, bảng phân quyền tài nguyên. |

Mở https://app.diagrams.net/, chọn **File → Open From → Device** rồi chọn file `.drawio`, hoặc kéo thả file vào trình biên tập. Chuyển sơ đồ bằng các tab trang phía dưới. Có thể sửa từng khối và đường nối, sau đó lưu lại file `.drawio`.

Các file này được chuyển từ nội dung Markdown hiện tại; thay đổi trong `.md` và `.drawio` không tự đồng bộ.

## Tài liệu nguồn

| STT | Tài liệu | Nội dung chính |
| :---: | :--- | :--- |
| 1 | [01_luong_chay.md](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/Doc_md/Diagram/01_luong_chay.md) | **Luồng chạy (Execution & Runtime Flow):** Khởi động 2 nhân (BootROM → `fip.bin` + `boot.sd`), trình tự `THERMAL_START` (I2C5 + SPI0), vòng lặp thu khung `THERMAL_CAPTURE`, chuyển chế độ / `FFC` / `STOP`, và luồng xử lý lỗi / khôi phục bus. |
| 2 | [02_luong_du_lieu.md](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/Doc_md/Diagram/02_luong_du_lieu.md) | **Luồng dữ liệu (Data Flow & Packet Layout):** Đường đi dữ liệu kênh điều khiển I2C5 VDCMD (`0x1D00`/`0x9D00`/`0x0200`), kênh ảnh/nhiệt SPI0 VoSPI (`0xAA`/`0xCC` + 512B + 96KB/192KB), bộ nhớ dùng chung `0x8FE00000`, gói 64B `thermal_header_t` + Payload qua TCP `:9000`, và công thức giải mã `Y16` / `YUYV422` → `RGB24`/`BMP`. |
| 3 | [03_tong_quan_he_thong.md](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/Doc_md/Diagram/03_tong_quan_he_thong.md) | **Tổng quan hệ thống (System Architecture Overview):** Sơ đồ phân tầng đầy đủ **T0 → T1 → T2 → T3 → App** trên 3 miền thực thi (Phần cứng SG2002/Tiny1C, FreeRTOS C906L, Linux C906B và Host PC) cùng bảng ánh xạ trực tiếp sang mã nguồn. |

---

## Bảng ánh xạ nhanh 5 tầng (T0 → App)

| Tầng | Phạm vi trách nhiệm | Kênh điều khiển I2C5 | Kênh dữ liệu SPI0 VoSPI | Tầng Ứng dụng & Giao tiếp |
| :--- | :--- | :--- | :--- | :--- |
| **T0 — Platform** | Clock, Reset, Pinmux, PADCTRL, DSI D-PHY Bypass, DMA/GPIO MMIO | [hal_i2c_gpio_platform.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c) | [hal_spi_platform_io.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform_io.c), [hal_spi_platform.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform.c) | Mailbox phần cứng SG2002 & Reserved RAM `0x8FE00000` |
| **T1 — HAL Core** | Máy trạng thái bit-bang I2C / bộ điều khiển thanh ghi `DW_apb_ssi` | [hal_i2c_bitbang.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_bitbang.c) | [hal_dw_spi.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/hal/cv181x/spi/src/hal_dw_spi.c) | `rtos_cmdqu` driver trên Linux ([rtos_cmdqu.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/osdrv/interdrv/rtos_cmdqu/rtos_cmdqu.c)) |
| **T2 — Driver Core** | Quản lý bus, mutex tĩnh FreeRTOS, đọc/ghi thanh ghi 16-bit, recovery | [soft_i2c.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/i2c/src/soft_i2c.c) | [spi.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/spi/src/spi.c) | [comm_main.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/task/comm/src/riscv64/comm_main.c) (`prvCmdQuRunTask`, `comm_poll_mailbox`) |
| **T3 — Camera Driver** | Giao thức VDCMD ASIC-2121W, lệnh `falcon_cmd`, thu khung VoSPI, quản lý vòng đời camera | [data_rw.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/sensor/tiny1C/src/data_rw.c), [vdcmd.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/sensor/tiny1C/src/vdcmd.c), [falcon_cmd.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/sensor/tiny1C/src/falcon_cmd.c) | [hal_spi.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/sensor/tiny1C/src/hal_spi.c), [vospi.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/sensor/tiny1C/src/vospi.c) | [core.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/sensor/tiny1C/src/core.c) |
| **App — RTOS / Linux / PC** | Điều phối dịch vụ, cầu nối TCP/Mailbox và giao diện hiển thị ảnh/nhiệt | — | — | [thermal_service.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/freertos/cvitek/driver/sensor/tiny1C/rtos/thermal_service.c), [thermal_bridge.c](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/Doc_md/Tiny-1C/App/linux/thermal_bridge.c), [thermal_viewer.py](file:///home/haidang/DNH_workspace/LicheeRV-Nano/LicheeRV-Nano-Test/Doc_md/Tiny-1C/App/pc/thermal_viewer.py) |
