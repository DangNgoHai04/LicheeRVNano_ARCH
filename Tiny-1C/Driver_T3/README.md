# Trình Điều Khiển Ngoại Vi Tầng 3 – Camera Nhiệt Tiny1C (ASIC-2121W)

Thư mục này chứa bản triển khai hoàn chỉnh và tối ưu hóa **Tầng 3 (Peripheral Sensor Driver)** cho mô-đun Camera nhiệt hồng ngoại **Tiny1C (ASIC-2121W)** trên vi xử lý **SG2002 (LicheeRV Nano)**, bám sát kiến trúc phân tầng **T0 – T1 – T2 – T3** của dự án FreeRTOS (`freertos/cvitek`).

---

## 1. Kiến trúc liên kết 4 tầng (T0 → T1 → T2 → T3)

```
┌────────────────────────────────────────────────────────────────────────────┐
│ TẦNG 3 (Thư mục này: /home/duong/Workspace/BKFast)                         │
│  - Điều khiển ASIC-2121W: data_rw, vdcmd (Mutex T3), falcon_cmd, core      │
│  - Thu nhận khung ảnh VoSPI: hal_spi, vospi (96KB Đơn / 192KB Kép)         │
│  - Liên nhân FreeRTOS <-> Linux: ipc (Shared DDR 0x8FE80000 + Mailbox)     │
└───────────────────┬────────────────────────────────────┬───────────────────┘
                    │ I2C5 (Điều khiển VDCMD)            │ SPI0 (Dữ liệu VoSPI)
                    ▼                                    ▼
┌───────────────────────────────────────┐ ┌──────────────────────────────────┐
│ TẦNG 2 (freertos/cvitek/driver/i2c)   │ │ TẦNG 2 (freertos/cvitek/driver/spi)│
│  - soft_i2c_init / soft_i2c_transfer  │ │  - spi_bus_init / spi_device_init│
│  - soft_i2c_read_reg16 / write_reg16  │ │  - spi_transfer_one / recover    │
│  - StaticMutex bảo vệ bus I2C5        │ │  - StaticMutex + BinarySemaphore │
└───────────────────┬───────────────────┘ └──────────────┬───────────────────┘
                    ▼                                    ▼
┌───────────────────────────────────────┐ ┌──────────────────────────────────┐
│ TẦNG 1 (hal/cv181x/i2c/hal_i2c_bitbang)│ │ TẦNG 1 (hal/cv181x/spi/hal_dw_spi)│
│  - Máy trạng thái khung I2C chuẩn     │ │  - Thanh ghi Synopsys DW SSI     │
│  - Clock stretching & khôi phục 9 xung│ │  - Polling 2 chiều & DMA 4KB/lần │
└───────────────────┬───────────────────┘ └──────────────┬───────────────────┘
                    ▼                                    ▼
┌───────────────────────────────────────┐ ┌──────────────────────────────────┐
│ TẦNG 0 (hal_i2c_gpio_platform.c)      │ │ TẦNG 0 (hal_spi_platform*.c)     │
│  - GPIOA15 (SCL), GPIOA27 (SDA)       │ │  - Pinmux SPI0 + CS0 GPIOC17     │
│  - Bộ đếm thời gian rdtime 25 MHz     │ │  - DW-DMAC ch0/1 + D-Cache C906  │
└───────────────────────────────────────┘ └──────────────────────────────────┘
```

---

## 2. Cấu trúc thư mục

```text
BKFast/
├── CMakeLists.txt          # Tích hợp vào hệ thống build CMake của FreeRTOS
├── Makefile                # Kiểm thử độc lập trên Host & kiểm tra biên dịch chéo RISC-V
├── README.md               # Tài liệu kiến trúc và hướng dẫn sử dụng Tầng 3
├── include/
│   ├── all_config.h        # Hằng số phần cứng I2C5, SPI0, vùng nhớ DDR & mã lỗi
│   ├── data_rw.h           # Giao diện đọc/ghi thanh ghi 16-bit qua I2C5 (T3 -> T2)
│   ├── vdcmd.h             # Giao thức Short / Standard / Long VDCMD có khóa Mutex T3
│   ├── falcon_cmd.h        # Tập lệnh điều khiển ASIC-2121W (ISP, TPD, DPC, SPI Flash)
│   ├── hal_spi.h           # Adapter SPI0 kết nối Tầng 3 xuống Tầng 2 (spi_device_t)
│   ├── vospi.h             # Giao thức đồng bộ gói và thu nhận khung ảnh nhiệt VoSPI
│   ├── core.h              # API nghiệp vụ cấp cao (Vòng đời, Đo nhiệt, ISP, Hiệu chuẩn)
│   └── ipc.h               # Giao thức liên nhân (Shared DDR 0x8FE80000 + Mailbox)
└── src/
    ├── data_rw.c           # Kết nối xuống soft_i2c (T2) & bb_gpio_platform (T0)
    ├── vdcmd.c             # Khóa thiết bị StaticSemaphore_t bảo vệ trọn chuỗi VDCMD
    ├── falcon_cmd.c        # Triển khai toàn bộ tập lệnh ASIC-2121W
    ├── hal_spi.c           # Kết nối xuống spi_bus_t / spi_device_t (T2)
    ├── vospi.c             # Thu nhận khung 96KB/192KB qua DMA T2 + tự động phục hồi bus
    ├── core.c              # Quy đổi nhiệt độ 1/16K -> 0.1°C và tọa độ 1-based <-> 0-based
    ├── ipc.c               # Đồng bộ D-Cache 64B (th.dcache.cva/iva) và xử lý lệnh Linux
    └── main.c              # Tiến trình thermal_camera_task trên FreeRTOS
```

---

## 3. Các tối ưu hóa và sửa lỗi trọng tâm ở Tầng 3

1. **Tuân thủ hợp đồng khóa thiết bị (Device-level Mutex) của Tầng 2 (`soft_i2c.c`)**:
   - Một lệnh VDCMD gồm nhiều giao dịch I2C liên tiếp (`Write 0x9D00` $\rightarrow$ `Poll 0x0200` $\rightarrow$ `Read 0x1D08`). `vdcmd.c` sử dụng `StaticSemaphore_t` ở Tầng 3 để khóa toàn bộ chuỗi lệnh, ngăn các tác vụ khác xen ngang làm hỏng bộ đệm nội bộ của ASIC.
2. **Tuân thủ hợp đồng địa chỉ 7-bit và tốc độ Standard-Mode của Tầng 1 (`hal_i2c_bitbang.c`)**:
   - Tự động chuẩn hóa địa chỉ `0x78` (8-bit trong SDK gốc) về `0x3C` (7-bit) và khởi tạo bus ở `100 kHz` đúng giới hạn `1..100 kHz` của `bb_init()`.
   - Với gói ghi $\le 64$ byte, gọi `soft_i2c_write_reg16()`; với gói ghi tải lớn $> 64$ byte (lên tới 256 byte), tự động đóng gói và gọi `soft_i2c_transfer()` để không bị giới hạn bởi bộ đệm tạm 66 byte của `soft_i2c_write_reg16()`.
3. **Loại bỏ xung đột tên hàm giữa `falcon_cmd.h` và `spi.h` của Tầng 2**:
   - Đổi tên chính thức nhóm hàm SPI Flash nội bộ của ASIC thành `falcon_spi_read` / `falcon_spi_write` (kèm macro tương thích ngược khi không nạp `spi.h` của Tầng 2).
4. **Tối ưu hóa thu nhận khung VoSPI 96 KB / 192 KB qua Tầng 2 (`spi_transfer_one`)**:
   - Không cần bộ đệm giả `tx_dummy` cho toàn khung 96 KB: truyền `tx_buf = NULL` và `dummy_fill = 0x0055`, Tầng 2/Tầng 1 tự động phát byte điền `0x55`, chia nhỏ thành các cụm DMA 4096 byte, giữ chân CS `GPIOC17` ở mức LOW xuyên suốt và đồng bộ D-Cache 64 byte.
5. **Sửa các lỗi tiềm ẩn từ bản nháp `RTOS_V1`**:
   - Sửa lỗi tràn bộ nhớ đệm 4 byte trong `short_cmd_write()` khi `byLen == 0` (truyền đúng `sizeof(vdcmd_short_header_t)` = 4 byte thay vì `sizeof(vdcmd_std_header_t)` = 8 byte).
   - Sửa mặt nạ kiểm tra lỗi `VCMD_RST_STS_FAIL` tại thanh ghi `0x0200` thành `0x02` (khớp với `VCMD_RST_STS_BIT`).
   - Chuẩn hóa hệ tọa độ `0-based` (`0..255`, `0..191`) sang `1-based` (`1..256`, `1..192`) khi giao tiếp với mô-đun đo nhiệt TPD của ASIC-2121W.

---

## 4. Hướng dẫn kiểm thử biên dịch

- **Kiểm thử logic độc lập trên máy chủ Linux (Host GCC)**:
  ```bash
  make test
  ```
- **Kiểm tra biên dịch chéo RISC-V 64-bit kết nối trực tiếp với T0-T1-T2 trong `freertos/cvitek`**:
  ```bash
  make riscv-check
  ```
