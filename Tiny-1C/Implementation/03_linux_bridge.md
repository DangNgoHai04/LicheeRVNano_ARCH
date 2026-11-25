# 03 — Bridge Linux

Mã nguồn ở [thermal_bridge.c](../App/linux/thermal_bridge.c). Binary target là RISC-V 64-bit musl static.

## Công việc

1. Giữ `/run/thermal-camera.lock` (`flock LOCK_EX | LOCK_NB`) để chỉ có một chủ sở hữu.
2. Mở `/dev/cvi-rtos-cmdqu` và `/dev/mem` (read-only, `O_SYNC`), ánh xạ vùng reserved RTOS DDR mặc định `0x8fe00000` kích thước `0x200000` (`--rtos-base`, `--rtos-size`).
3. Gửi `ioctl(SEND_WAIT)` với timeout 30 giây (`30000 ms`). Khi giao dịch lỗi, bridge đóng phiên client hiện tại, thử `THERMAL_STOP` ở cleanup rồi quay lại `accept()`. Lệnh đang chờ không được gửi lại tự động.
4. Kiểm tra response nằm trong reserved RTOS DDR, căn 64 byte (`(q.param & 63) == 0`), đúng `magic`/`version`/`header_bytes`, `mode`, `width`/`height` và `payload_bytes` không vượt giới hạn.
5. Copy dữ liệu trước yêu cầu kế tiếp; thiết lập `SO_REUSEADDR`, bỏ qua `SIGPIPE`, bật `TCP_NODELAY` và timeout socket `35s` (`SO_RCVTIMEO`/`SO_SNDTIMEO`) trên client, rồi gửi gộp 64B header + payload trong một lời gọi `send_all(client, &packet, 64 + packet.h.payload_bytes)`.
6. Nhận 7 lệnh TCP 1 byte: `'t'` (`START MODE_TEMP`), `'i'` (`START MODE_IMAGE`), `'d'` (`START MODE_DUAL`), `'c'` (`CAPTURE`), `'f'` (`FFC`), `'s'` (`STOP`), `'?'` (`INFO`).
7. Mặc định nghe `127.0.0.1:9000` để dùng SSH tunnel; hỗ trợ `--bind`, `--port`, `--resources-owned`, `--dma-reserved` và `--simulate` (header mang `THERMAL_FLAG_SIMULATED | THERMAL_FLAG_HIGH_GAIN`). Khi chạy không truyền `--simulate` lẫn `--resources-owned`, chương trình thoát với mã lỗi `2`.

## Cách build & chạy kiểm thử Bridge

```bash
# Build cả bản host (thermal_bridge_host), bản static RISC-V Linux (thermal_bridge) và chạy 11 test App
make -C Doc_md/Tiny-1C/App all linux test

# Chạy bridge giả lập độc lập trên PC để kiểm thử với thermal_viewer.py
./Doc_md/Tiny-1C/App/build/thermal_bridge_host --simulate --port 9000
```

## Sửa lỗi mailbox kernel cần thiết

`osdrv/interdrv/rtos_cmdqu/rtos_cmdqu.c` đã được sửa ở đường SEND_WAIT:
- Bỏ kfree(wait_list) với con trỏ chưa khởi tạo trong ioctl.
- Trả lỗi copy_from/to_user, lỗi send, timeout và signal về userspace.
- Dọn wait-list khi send thất bại.

Phải dùng module đã rebuild, đúng kernel trên board. Kernel workspace hiện tại:
`5.10.260-tag-20260926-00002-g235afa875600`.

Bridge mặc định kiểm tra vùng 0x8fe00000 + 0x200000. Nếu dùng memory map khác,
phải build firmware phù hợp và đặt --rtos-base/--rtos-size đúng vùng Linux đã
reserve; không dùng tham số này để map tùy ý RAM đang thuộc Linux.

## Thứ tự đọc

Theo `main` từ byte TCP đến `transact`. Đối chiếu lệnh và địa chỉ mailbox với `thermal_service_task`. Đọc `valid_header` để hiểu tại sao ACK/lỗi có thể dài đúng 64 byte. Xem `send_all` để biết cách xử lý gửi một phần và `EINTR`, rồi so với `recv_exact` phía PC. [Lộ trình đọc T3 → App](../LO_TRINH_DOC_T3_APP.md)
