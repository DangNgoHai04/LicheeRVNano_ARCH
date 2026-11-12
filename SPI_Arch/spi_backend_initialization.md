# Chọn backend và khởi tạo IRQ/DMA theo nhu cầu

## 1. Hành vi mới

`spi_bus_init()` chỉ chuẩn bị SPI cơ bản. Nó không đăng ký PLIC, tạo semaphore IRQ, remap request DMA, bật DMAC hoặc dừng channel DMA. Các bit IMR/DMACR bên trong SPI vẫn được ghi 0 để controller không phát request ngoài ý muốn.

Ba trường mới ở cuối `spi_bus_config_t`:

| Trường | Mặc định | Ý nghĩa |
| --- | --- | --- |
| `enable_irq` | false | Cho phép device/AUTO chọn IRQ |
| `enable_dma` | false | Cho phép device/AUTO chọn DMA |
| `dma_resources_reserved` | false | Board đã cấp quyền channel 0/1 và cấu hình clock/reset/remap/interrupt routing/global DMAC cho SPI |

Nếu bật DMA nhưng chưa xác nhận reservation, init bus trả `SPI_ERR_UNSUPPORTED` trước MMIO platform. Cờ reservation không tự vô hiệu hóa Linux driver, sửa DTS hoặc khóa liên lõi; board integration phải thực sự cấp quyền và giữ quyền đó trong suốt thời gian driver có thể dùng DMAC.

## 2. Luồng mới

```mermaid
sequenceDiagram
    participant A as Caller
    participant B as T2
    participant H as T1
    participant P as T0
    A->>B: spi_bus_init(config)
    B->>H: hal_spi_init()
    H->>P: clock/reset/pinmux/CS
    Note over H,P: Chưa init IRQ/DMAC
    A->>B: spi_transfer(message)
    B->>B: lấy mutex; chọn backend cho từng segment
    B->>H: prepare_irq / prepare_dma nếu cần
    H->>P: init backend nếu chưa ready
    alt Init backend lỗi
        P-->>B: lỗi được truyền qua T1
        B-->>A: trả lỗi; chưa CS active/chưa phát frame
    else Backend đã sẵn sàng
        B->>H: configure nếu cần
        B->>P: CS active
        B->>H: begin / transfer / finish
        B->>P: CS inactive khi xong
    end
```

T2 chuẩn bị các backend cần dùng cho **toàn message** trước configure/CS. Ví dụ segment đầu polling, segment sau DMA nhưng DMA init lỗi: segment polling cũng chưa được phát. Lỗi init không tự fallback sang backend khác. Backend đã prepare thành công được giữ lại; không rollback IRQ đã đăng ký nếu sau đó prepare DMA thất bại.

`hal_spi_prepare_irq()` dùng wrapper callback đúng kiểu `void (*)(void *)`. `hal_spi_prepare_dma()` yêu cầu reservation. `irq_ready`/`dma_ready` chỉ được đặt true sau init thành công. T1 transfer từ chối backend chưa prepare. T0 DMA transfer cũng từ chối nếu chưa init, không tự bật DMAC.

## 3. Cách cấu hình

Polling mặc định, kể cả khi device chọn AUTO:

```c
spi_bus_config_t cfg = {
    .reg_base = 0x04180000UL,
    .ref_clock_hz = 0, /* dùng clock mặc định của T0, không phải phép đo */
};
```

Để cho phép IRQ, đặt `.enable_irq = true` trong cấu hình trước `spi_bus_init()`. Tài nguyên IRQ vẫn chưa init cho tới message đầu tiên cần IRQ.

Để cho phép DMA, cấu hình trước init:

```c
/* Chỉ thực hiện sau khi board/OS đã dành tài nguyên DMAC cho SPI. */
cfg.enable_dma = true;
cfg.dma_resources_reserved = true;
```

Hai trường enable độc lập. Sau init bus, cấu hình `spi_device_config_t.backend` hoặc dùng `spi_device_set_backend()` để chọn POLL/IRQ/DMA/AUTO trong phạm vi bus cho phép. Setter giữ mutex, không khởi tạo phần cứng tại thời điểm đổi. IRQ/DMA explicit bị từ chối nếu bus không cho phép.

| Bus cho phép | AUTO với ngưỡng mặc định |
| --- | --- |
| Chỉ polling | Mọi kích thước dùng polling |
| Polling + IRQ | <32 frame polling, ≥32 IRQ |
| Polling + DMA | <64 frame polling, ≥64 DMA |
| Cả ba | <32 polling, 32–63 IRQ, ≥64 DMA |

Các ngưỡng vẫn có thể đặt trong bus config. Driver ưu tiên DMA khi đủ ngưỡng và được phép, rồi IRQ, rồi polling.

## 4. Ownership và vòng đời

T0 kiểm reservation trước mọi MMIO DMA. Lần đầu prepare có reservation: bật clock/nhả reset, đọc CHEN; nếu bất kỳ channel nào đang chạy trả BUSY trước khi đổi remap/global config. Đây là kiểm tra trạng thái, không loại bỏ race với một writer khác. Board vẫn phải bảo đảm quyền sở hữu độc quyền khi cấu hình DMAC.

Đổi device từ DMA về polling dừng dùng DMA để truyền các segment mới; nó không tắt global DMAC hay unregister IRQ đã chuẩn bị. Bus deinit/recover cũng chưa trả tài nguyên cho Linux. Reservation phải tồn tại suốt vòng đời firmware này; chưa có API runtime chuyển quyền DMAC.

Abort của một HAL chưa prepare DMA không gọi T0 DMA abort. Sau khi DMA đã ready, cleanup có thể tiếp tục dùng hook abort của DMA. Các hạn chế xác nhận channel đã dừng và cache/timeout từ review trước vẫn chưa được sửa trong thay đổi này.

## 5. Danh sách thay đổi

| File | Thay đổi |
| --- | --- |
| `freertos/cvitek/driver/spi/include/spi.h` | Thêm 3 trường policy/reservation vào bus config/state; mô tả mặc định polling |
| `freertos/cvitek/driver/spi/src/spi.c` | Gate backend explicit, AUTO theo quyền, preflight trước CS, truyền lỗi prepare; setter backend giữ mutex |
| `freertos/cvitek/hal/cv181x/spi/include/hal_dw_spi.h` | Thêm ready state và API prepare IRQ/DMA; thêm reservation vào hook DMA init |
| `freertos/cvitek/hal/cv181x/spi/src/hal_dw_spi.c` | Bỏ eager init, init một lần khi cần, callback wrapper, guard transfer/abort |
| `freertos/cvitek/hal/cv181x/spi/src/hal_spi_platform.c` | Gate reservation, từ chối DMAC bận, không tự init từ transfer, không disable channel đang có chủ khi init |
| `freertos/cvitek/hal/cv181x/spi/tests/test_spi.c` | Regression mặc định polling, policy/AUTO, lazy init, lỗi trước CS, retry và HAL chưa prepare |

Public struct và hook signature đã đổi: rebuild mọi caller, dùng initializer có tên trường hoặc zero-init. Caller dùng HAL trực tiếp phải gọi prepare lúc idle trước begin. Không copy ABI/header cũ vào image mới.

## 6. Phạm vi kiểm tra

Host tests dùng mock T0 kiểm hành vi T1/T2 và fault injection init. Source T0/T1 và T2 được cross-compile qua target `halspi` và `spi`. Đây không phải kiểm chứng MMIO trên board. Sai lệch clock/reset/pinmux, timeout/cache/abort còn lại nằm trong các tài liệu review T0/T1.
