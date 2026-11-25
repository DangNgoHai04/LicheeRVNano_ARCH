# Tổng quan Tài liệu Kiến trúc I²C Bitbang (T0 – T2) cho Tiny1C

Nhánh điều khiển I²C cho camera nhiệt Tiny1C trên nhân FreeRTOS (C906L) sử dụng kiến trúc **I²C Bitbang 3 tầng (`T0` $\rightarrow$ `T1` $\rightarrow$ `T2`)** trên hai chân **`GPIOA15 = SCL`** và **`GPIOA27 = SDA`**, tốc độ cấu hình mặc định `100 kHz`; firmware bring-up 18/11 dùng `20 kHz`.

Cấu trúc tài liệu trong thư mục này được chuẩn hóa đồng bộ với [`Doc_md/SPI_Arch/`](../SPI_Arch/spi_driver_architecture_and_roadmap.md):

## 1. Danh mục Tài liệu và Mã nguồn theo Tầng

| Tầng / Hạng mục | Mã nguồn Thực thi | Tài liệu Kiến trúc & Đánh giá Source |
| :--- | :--- | :--- |
| **Tổng quan & Lộ trình** | — | • [Kiến trúc Driver I²C Bitbang và Lộ trình Phát triển](i2c_driver_architecture_and_roadmap.md) |
| **Tầng 0 (T0 — Platform SoC)** | [`hal_i2c_gpio_platform.c`](../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_gpio_platform.c)<br>[`hal_i2c_gpio_platform.h`](../../freertos/cvitek/hal/cv181x/i2c/include/hal_i2c_gpio_platform.h)<br>[README T0–T1](../../freertos/cvitek/hal/cv181x/i2c/README.md) | • [Kiến trúc Chốt Tầng 0 (GPIO Platform)](T0_layer_arch/i2c_t0_platform_architecture.md)<br>• [Đánh giá Source & Trạng thái Hoàn thiện T0](T0_layer_arch/i2c_t0_source_review.md) |
| **Tầng 1 (T1 — Bitbang HAL)** | [`hal_i2c_bitbang.c`](../../freertos/cvitek/hal/cv181x/i2c/src/hal_i2c_bitbang.c)<br>[`hal_i2c_bitbang.h`](../../freertos/cvitek/hal/cv181x/i2c/include/hal_i2c_bitbang.h)<br>[README T0–T1](../../freertos/cvitek/hal/cv181x/i2c/README.md) | • [Kiến trúc Chốt Tầng 1 (Bitbang Controller HAL)](T1_layer_arch/i2c_t1_controller_architecture.md)<br>• [Đánh giá Source & Trạng thái Hoàn thiện T1](T1_layer_arch/i2c_t1_source_review.md) |
| **Tầng 2 (T2 — RTOS Bus API)** | [`soft_i2c.c`](../../freertos/cvitek/driver/i2c/src/soft_i2c.c)<br>[`soft_i2c.h`](../../freertos/cvitek/driver/i2c/include/soft_i2c.h)<br>[README T2](../../freertos/cvitek/driver/i2c/README.md) | • [Kiến trúc Chốt Tầng 2 (RTOS Soft-I²C Driver)](T2_layer_arch/i2c_t2_driver_architecture.md)<br>• [Đánh giá Source & Trạng thái Hoàn thiện T2](T2_layer_arch/i2c_t2_source_review.md) |
| **Tích hợp Tầng 3 (T3)** | [Driver Tiny1C (freertos)](../../freertos/cvitek/driver/sensor/tiny1C/README.md) | • [Hướng dẫn Phối hợp I²C và SPI cho Tầng 3 Tiny1C](tiny1c_integration.md) |

## 2. Kiểm thử Nhanh từ Thư mục Gốc Repository

```bash
bash freertos/cvitek/hal/cv181x/i2c/tests/run.sh
bash freertos/cvitek/hal/cv181x/i2c/tests/compile-riscv.sh
```

Đọc tiếp [lộ trình T3 → App](../Tiny-1C/LO_TRINH_DOC_T3_APP.md) để lần theo START, VDCMD, CAPTURE và gói THM1.
