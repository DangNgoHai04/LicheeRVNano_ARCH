#!/usr/bin/env python3
"""
Trình xem và kiểm tra dữ liệu Camera nhiệt Tiny1C (thermal_viewer.py).
Chỉ sử dụng thư viện chuẩn Python (Standard Library Only).

Hỗ trợ 3 chế độ hiển thị & kiểm tra dữ liệu truyền sang:
  1. Chế độ CLI Inspector (--cli / --inspect):
     Hiển thị chi tiết gói Header 64-byte (THM1), mẫu byte Hex của Payload,
     giá trị từ 16-bit thô (raw_u16) -> Kelvin -> °C tại các điểm đặc trưng
     (Min, Max, Tâm, Góc), vẽ bản đồ nhiệt ASCII/ANSI ngay trên Terminal
     và xuất file (.bin, .json, .ppm, .bmp).
  2. Chế độ Web Viewer (--web PORT, ví dụ --web 8080):
     Mở máy chủ HTTP nội bộ xem trực tiếp luồng ảnh nhiệt / YUYV / Dual trên
     trình duyệt web kèm bảng thông số gói tin thời gian thực (không cần cài tkinter).
  3. Chế độ Cửa sổ Desktop GUI (mặc định khi máy có cài đặt python3-tk / tkinter).
"""
import argparse
import array
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import queue
import socket
import struct
import sys
import threading
import time
from pathlib import Path

# Cấu trúc Header 64-byte Little-Endian khớp với thermal_header_t trong thermal_protocol.h
HEADER = struct.Struct('<IIIi12I')
MAGIC = 0x314D4854  # Chuỗi byte 'THM1' ở định dạng Little-Endian
MAX_PAYLOAD = 196608

MODE_NAMES = {
    0: 'MODE_IMAGE (0 - YUYV422 256x192, 98304 bytes)',
    1: 'MODE_TEMP  (1 - RAW Y16 256x192, 98304 bytes)',
    2: 'MODE_DUAL  (2 - YUYV + Y16 256x384, 196608 bytes)',
}

FLAG_LABELS = (
    (1, 'SIMULATED'),
    (2, 'SHUTTER_CLOSED'),
    (4, 'FROZEN'),
    (8, 'HIGH_GAIN'),
    (16, 'DUAL_INDEX_MISMATCH'),
)


def recv_exact(sock, size):
    """Đọc đủ đúng `size` byte từ kết nối TCP."""
    data = bytearray()
    while len(data) < size:
        part = sock.recv(size - len(data))
        if not part:
            raise ConnectionError('Bridge disconnected')
        data.extend(part)
    return bytes(data)


def read_packet(sock):
    """Đọc và kiểm tra tính hợp lệ của 1 gói tin (64-byte Header + Payload) từ thermal_bridge."""
    raw_hdr = recv_exact(sock, HEADER.size)
    fields = HEADER.unpack(raw_hdr)
    names = ('magic', 'version', 'header_bytes', 'status', 'seq', 'mode', 'width',
             'height', 'payload_bytes', 'timestamp_ms', 'index_image', 'index_temp',
             'flags', 'vtemp', 'reserved0', 'reserved1')
    h = dict(zip(names, fields))
    size = h['payload_bytes']
    if (h['magic'] != MAGIC or h['version'] != 1 or h['header_bytes'] != 64
            or h['mode'] not in (0, 1, 2) or h['width'] != 256
            or h['height'] != (384 if h['mode'] == 2 else 192)
            or size > MAX_PAYLOAD
            or (size and (h['status'] or size != (196608 if h['mode'] == 2 else 98304)))):
        raise ValueError('Invalid frame ABI/length')
    return h, recv_exact(sock, size)


def format_i2c_diag(h):
    """Giải mã hai trường chẩn đoán từ header RTOS, kể cả status BB âm."""
    raw = h['reserved0']
    bb = raw - (1 << 32) if raw & (1 << 31) else raw
    step = h['reserved1'] >> 24
    phase = (h['reserved1'] >> 16) & 0xff
    reg = h['reserved1'] & 0xffff
    step_name = {1: 'PROJECT', 2: 'FW_DATE', 3: 'FW_VERSION',
                 4: 'PN', 5: 'SN'}.get(step, 'INIT' if step == 0 else str(step))
    bb_name = {0: 'OK', -1: 'INVALID', -2: 'TIMEOUT', -3: 'NACK',
               -4: 'BUSY', -5: 'ARBITRATION', -6: 'NOT_READY'}.get(bb, 'UNKNOWN')
    if bb == -3:
        msg = 1 if phase & 0x80 else 0
        byte = phase & 0x7f
        where = f'msg{msg}.address' if byte == 0 else f'msg{msg}.byte{byte - 1}'
    else:
        where = '-'
    return f'probe={step_name}, reg=0x{reg:04x}, BB={bb_name}({bb}), nack_at={where}'


def clamp(value):
    return max(0, min(255, int(value)))


THERMAL_LUT = tuple(bytes((clamp(510 * i / 255),
                           clamp(510 * (1 - abs(2 * (i / 255) - 1))),
                           clamp(255 - 510 * i / 255))) for i in range(256))
GRAY_LUT = bytes(clamp(1.164 * max(0, y - 16)) for y in range(256))


def thermal_rgb(payload, low=None, high=None, render=True):
    """
    Chuyển đổi mặt phẳng nhiệt độ Y16 Little-Endian (256x192x2 = 98,304 byte)
    sang ảnh giả màu RGB24 và tính toán thống kê nhiệt độ (°C).
    Công thức chuẩn cảm biến Tiny1C: Celsius = raw_u16 / 64.0 - 273.15
    """
    raw = array.array('H')
    raw.frombytes(payload)
    if sys.byteorder != 'little':
        raw.byteswap()
    if len(raw) != 256 * 192:
        raise ValueError('Invalid thermal plane size')
    lo, hi = min(raw), max(raw)
    min_idx = raw.index(lo)
    max_idx = raw.index(hi)
    lower = lo if low is None else (low + 273.15) * 64
    upper = hi if high is None else (high + 273.15) * 64
    span = max(upper - lower, 1)
    rgb = None
    if render:
        # Map each distinct Y16 value once, then only look up pixels.
        colors = {v: THERMAL_LUT[clamp((v - lower) * 255 / span)] for v in set(raw)}
        rgb = b''.join(map(colors.__getitem__, raw))
    center_raw = raw[96 * 256 + 128]
    stats = {
        'min_c': lo / 64 - 273.15,
        'max_c': hi / 64 - 273.15,
        'center_c': center_raw / 64 - 273.15,
        'scale_min_c': lower / 64 - 273.15,
        'scale_max_c': upper / 64 - 273.15,
        'min_raw': lo,
        'max_raw': hi,
        'center_raw': center_raw,
        'min_xy': (min_idx % 256, min_idx // 256),
        'max_xy': (max_idx % 256, max_idx // 256),
    }
    return rgb, stats


def yuyv_rgb(payload):
    """
    Chuyển đổi mặt phẳng ảnh YUYV422 (256x192x2 = 98,304 byte) sang RGB24.
    Mỗi cụm 4 byte [Y0, U, Y1, V] giải mã thành 2 điểm ảnh RGB liên tiếp.
    """
    if len(payload) != 98304:
        raise ValueError('Invalid YUYV plane size')
    if payload[1::2] == b'\x80' * (len(payload) // 2):
        gray = payload[0::2].translate(GRAY_LUT)
        out = bytearray(len(gray) * 3)
        out[0::3] = out[1::3] = out[2::3] = gray
        return bytes(out)
    out = bytearray()
    for i in range(0, len(payload), 4):
        y0, u, y1, v = payload[i:i + 4]
        u -= 128
        v -= 128
        for y in (y0, y1):
            c = max(0, y - 16)
            out.extend((clamp(1.164 * c + 1.596 * v),
                        clamp(1.164 * c - .392 * u - .813 * v),
                        clamp(1.164 * c + 2.017 * u)))
    return bytes(out)


def decode_frame(h, data, low=None, high=None):
    """Giải mã gói dữ liệu thành ảnh PPM (P6) và từ điển thống kê nhiệt độ."""
    if not data:
        return None, {}
    mode = h['mode']
    stats = {}
    if mode == 0:
        rgb = yuyv_rgb(data)
    elif mode == 1:
        rgb, stats = thermal_rgb(data, low, high)
    else:
        first = yuyv_rgb(data[:98304])
        second, stats = thermal_rgb(data[98304:], low, high)
        rgb = first + second  # Nửa trên 256x192 là ảnh YUYV, nửa dưới 256x192 là bản đồ nhiệt
    return b'P6\n256 %d\n255\n' % h['height'] + rgb, stats


def encode_bmp(width, height, rgb):
    """Đóng gói dữ liệu RGB24 thành ảnh BMP 24-bit chuẩn để hiển thị trực tiếp trên trình duyệt."""
    row_stride = (width * 3 + 3) & ~3
    padding = b'\x00' * (row_stride - width * 3)
    pixel_bytes = bytearray()
    # Định dạng BMP lưu các hàng từ dưới lên trên (Bottom-Up) và thứ tự byte BGR
    for y in range(height - 1, -1, -1):
        row = rgb[y * width * 3:(y + 1) * width * 3]
        bgr = bytearray(len(row))
        bgr[0::3], bgr[1::3], bgr[2::3] = row[2::3], row[1::3], row[0::3]
        pixel_bytes.extend(bgr)
        pixel_bytes.extend(padding)
    file_size = 54 + len(pixel_bytes)
    bmp_header = struct.pack('<2sIHHI', b'BM', file_size, 0, 0, 54)
    dib_header = struct.pack('<IiiHHIIiiII', 40, width, height, 1, 24, 0,
                             len(pixel_bytes), 2835, 2835, 0, 0)
    return bmp_header + dib_header + bytes(pixel_bytes)


def format_flags(flags):
    names = [label for bit, label in FLAG_LABELS if flags & bit]
    return ' | '.join(names) if names else 'NONE'


def render_ascii_heatmap(thermal_payload, cols=64, rows=24, use_ansi=True):
    """
    Vẽ bản đồ nhiệt trực quan ngay trên màn hình Terminal từ 98,304 byte dữ liệu Y16.
    Mỗi ô ký tự đại diện cho 1 vùng (256/cols) x (192/rows) điểm ảnh.
    """
    raw = array.array('H')
    raw.frombytes(thermal_payload)
    if sys.byteorder != 'little':
        raw.byteswap()
    lo, hi = min(raw), max(raw)
    span = max(hi - lo, 1)
    chars = ' .:-=+*#%@'
    ansi_colors = (19, 21, 27, 39, 51, 48, 82, 190, 226, 208, 196)
    lines = []
    for r in range(rows):
        y = min(191, int(r * 192 / rows))
        line = []
        for c in range(cols):
            x = min(255, int(c * 256 / cols))
            v = raw[y * 256 + x]
            norm = (v - lo) / span
            ch = chars[min(len(chars) - 1, int(norm * (len(chars) - 1)))]
            if use_ansi:
                color = ansi_colors[min(len(ansi_colors) - 1, int(norm * (len(ansi_colors) - 1)))]
                line.append(f'\033[38;5;{color}m{ch}\033[0m')
            else:
                line.append(ch)
        row_c = (raw[y * 256 + 128] / 64.0) - 273.15
        lines.append(f'  y={y:03d} |' + ''.join(line) + f'| {row_c:5.1f}°C')
    return '\n'.join(lines)


def inspect_packet(h, data, stats=None):
    """
    Phân tích chi tiết cấu trúc gói tin truyền từ RTOS -> Linux Bridge -> Viewer
    để hiển thị log kiểm chứng đầy đủ từng byte và từng điểm ảnh.
    """
    raw_hdr = HEADER.pack(
        h['magic'], h['version'], h['header_bytes'], h['status'],
        h['seq'], h['mode'], h['width'], h['height'],
        h['payload_bytes'], h['timestamp_ms'], h['index_image'], h['index_temp'],
        h['flags'], h['vtemp'], h['reserved0'], h['reserved1']
    )
    info = {
        'header': h,
        'header_hex_64b': raw_hdr.hex(' '),
        'mode_desc': MODE_NAMES.get(h['mode'], str(h['mode'])),
        'flags_desc': format_flags(h['flags']) if h['status'] == 0 else 'DIAGNOSTIC (không phải cờ ảnh)',
    }
    if h['status'] != 0:
        info['error'] = ('VoSPI header/đường SPI lỗi' if h['status'] == -2 else
                         'VoSPI payload lỗi' if h['status'] == -3 else
                         'VoSPI frame_valid=0' if h['status'] == -4 else
                         'Camera trả lỗi %d' % h['status'])
    if not data:
        return info

    if h['mode'] in (0, 2):
        img_plane = data[:98304]
        y0, u, y1, v = img_plane[0:4]
        cy, cu, cy1, cv = img_plane[(96 * 256 + 128) * 2:(96 * 256 + 128) * 2 + 4]
        info['image_plane'] = {
            'offset_bytes': 0,
            'length_bytes': len(img_plane),
            'first_16_bytes_hex': img_plane[:16].hex(' '),
            'pixel_0_0_yuyv': {'Y0': y0, 'U': u, 'Y1': y1, 'V': v},
            'pixel_center_128_96_yuyv': {'Y0': cy, 'U': cu, 'Y1': cy1, 'V': cv},
        }

    if h['mode'] in (1, 2):
        temp_offset = 98304 if h['mode'] == 2 else 0
        temp_plane = data[temp_offset:temp_offset + 98304]
        if stats is None:
            _, stats = thermal_rgb(temp_plane, render=False)
        raw = array.array('H')
        raw.frombytes(temp_plane)
        if sys.byteorder != 'little':
            raw.byteswap()
        samples = []
        for label, (px, py) in [
            ('Góc trên trái (0, 0)', (0, 0)),
            ('Tâm khung hình (128, 96)', (128, 96)),
            ('Điểm lạnh nhất Min', stats['min_xy']),
            ('Điểm nóng nhất Max', stats['max_xy']),
        ]:
            idx = py * 256 + px
            val = raw[idx]
            b0, b1 = temp_plane[idx * 2], temp_plane[idx * 2 + 1]
            samples.append({
                'label': label,
                'x': px,
                'y': py,
                'byte_offset': temp_offset + idx * 2,
                'le_bytes_hex': f'{b0:02x} {b1:02x}',
                'raw_u16': val,
                'kelvin': round(val / 64.0, 2),
                'celsius': round(val / 64.0 - 273.15, 2),
            })
        info['temp_plane'] = {
            'offset_bytes': temp_offset,
            'length_bytes': len(temp_plane),
            'first_16_bytes_hex': temp_plane[:16].hex(' '),
            'stats': stats,
            'sample_points': samples,
        }
    return info


def print_inspection_report(h, data, use_ansi=True):
    """In báo cáo giải mã chi tiết gói dữ liệu Camera nhiệt ra màn hình console."""
    rep = inspect_packet(h, data)
    magic_bytes = struct.pack('<I', h['magic']).decode('ascii', errors='replace')
    print('=' * 88)
    print(f"[GÓI DỮ LIỆU CAMERA NHIỆT TINY1C] seq={h['seq']} | {rep['mode_desc']}")
    print('=' * 88)
    print('1. HEADER 64-BYTE (thermal_header_t - Little-Endian ABI):')
    print(f"   Hex (00..31): {rep['header_hex_64b'][:95]}")
    print(f"   Hex (32..63): {rep['header_hex_64b'][96:]}")
    print(f"   - magic / version / hdr : 0x{h['magic']:08X} ('{magic_bytes}') / v{h['version']} / {h['header_bytes']} bytes")
    print(f"   - status / seq          : {h['status']} / {h['seq']}")
    print(f"   - width x height        : {h['width']} x {h['height']} pixels (payload_bytes = {h['payload_bytes']} B)")
    print(f"   - index_image / temp    : {h['index_image']} / {h['index_temp']} (timestamp = {h['timestamp_ms']} ms)")
    print(f"   - vtemp / flags         : {h['vtemp']} / 0x{h['flags']:02X} ({rep['flags_desc']})")

    if 'image_plane' in rep:
        ip = rep['image_plane']
        p0 = ip['pixel_0_0_yuyv']
        pc = ip['pixel_center_128_96_yuyv']
        print('\n2. MẶT PHẲNG ẢNH QUANG HỌC (YUYV422 - 256x192x2 = 98,304 bytes):')
        print(f"   - Phạm vi byte Payload  : offset {ip['offset_bytes']} .. {ip['offset_bytes'] + ip['length_bytes'] - 1}")
        print(f"   - 16 byte đầu (Hex)     : {ip['first_16_bytes_hex']}")
        print(f"   - Cụm Macropixel (0,0)  : Y0={p0['Y0']}, U={p0['U']}, Y1={p0['Y1']}, V={p0['V']}")
        print(f"   - Cụm Tâm (128,96)      : Y0={pc['Y0']}, U={pc['U']}, Y1={pc['Y1']}, V={pc['V']}")

    if 'temp_plane' in rep:
        tp = rep['temp_plane']
        st = tp['stats']
        sec_num = '3' if 'image_plane' in rep else '2'
        print(f"\n{sec_num}. MẶT PHẲNG NHIỆT ĐỘ BỨC XẠ (RAW Y16 Little-Endian - 256x192x2 = 98,304 bytes):")
        print(f"   - Phạm vi byte Payload  : offset {tp['offset_bytes']} .. {tp['offset_bytes'] + tp['length_bytes'] - 1}")
        print(f"   - 16 byte đầu (Hex)     : {tp['first_16_bytes_hex']}")
        print('   - Công thức giải mã     : Celsius = (raw_u16 / 64.0) - 273.15')
        print(f"   - Thống kê toàn khung   : Min = {st['min_c']:.2f}°C tại {st['min_xy']} | "
              f"Max = {st['max_c']:.2f}°C tại {st['max_xy']} | Tâm (128,96) = {st['center_c']:.2f}°C")
        print('   - Bảng đối chiếu byte thô -> nhiệt độ tại các tọa độ đặc trưng:')
        for sp in tp['sample_points']:
            print(f"     + {sp['label']:<25}: (x={sp['x']:3d}, y={sp['y']:3d}) | offset={sp['byte_offset']:6d} | "
                  f"LE=[{sp['le_bytes_hex']}] -> raw={sp['raw_u16']:5d} -> {sp['kelvin']:6.2f} K -> {sp['celsius']:6.2f} °C")
        temp_offset = tp['offset_bytes']
        print('\n   - Bản đồ nhiệt trực quan (256x192 thu gọn 64x16 trên Terminal):')
        print(render_ascii_heatmap(data[temp_offset:temp_offset + 98304], cols=64, rows=16, use_ansi=use_ansi))
    print('=' * 88)


class CameraWorker(threading.Thread):
    def __init__(self, host, port, commands, results, fps, mode="temp"):
        super().__init__(daemon=True)
        self.host, self.port, self.commands, self.results, self.fps = host, port, commands, results, fps
        self.mode_op = {"temp": "t", "image": "i", "dual": "d", "all": "t"}[mode]
        self.stop_event = threading.Event()
        self.sock = None

    def publish(self, item):
        # Chỉ giữ khung hình mới nhất cho hiển thị, không ghi đè vùng nhớ của Camera
        try:
            self.results.put_nowait(item)
        except queue.Full:
            try:
                self.results.get_nowait()
            except queue.Empty:
                pass
            self.results.put_nowait(item)

    def run(self):
        while not self.stop_event.is_set():
            try:
                with socket.create_connection((self.host, self.port), timeout=5) as sock:
                    self.sock = sock
                    sock.settimeout(35)
                    sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
                    op = self.mode_op
                    while not self.stop_event.is_set():
                        begin = time.monotonic()
                        try:
                            op = self.commands.get_nowait()
                        except queue.Empty:
                            pass
                        if op in 'tid':
                            self.mode_op = op
                        sock.sendall(op.encode('ascii'))
                        packet = read_packet(sock)
                        self.publish(('frame', packet))
                        if packet[0]['status'] and op in 'tid':
                            raise RuntimeError('Camera start failed (status=%d, %s); retrying...'
                                               % (packet[0]['status'], format_i2c_diag(packet[0])))
                        was_capture = op == 'c'
                        op = 'c'
                        if not was_capture:
                            continue
                        self.stop_event.wait(max(0, 1 / self.fps - (time.monotonic() - begin)))
            except Exception as exc:
                if not self.stop_event.is_set():
                    self.publish(('error', str(exc)))
                    self.stop_event.wait(1.5)
            finally:
                self.sock = None

    def close(self):
        self.stop_event.set()
        if self.sock:
            try:
                self.sock.shutdown(socket.SHUT_RDWR)
            except OSError:
                pass


def run_cli(args):
    """Chạy chế độ kiểm tra dữ liệu trực tiếp trên Terminal (--cli / --inspect)."""
    mode_map = {'temp': [('t', 'temp')], 'image': [('i', 'image')], 'dual': [('d', 'dual')],
                'all': [('t', 'temp'), ('i', 'image'), ('d', 'dual')]}
    plan = mode_map[args.mode]
    dump_dir = Path(args.dump_dir) if args.dump_dir else None
    if dump_dir:
        dump_dir.mkdir(parents=True, exist_ok=True)

    with socket.create_connection((args.host, args.port), timeout=5) as sock:
        sock.settimeout(35)
        for op, mode_name in plan:
            sock.sendall(op.encode('ascii'))
            start_h, _ = read_packet(sock)
            if start_h['status'] != 0:
                raise RuntimeError(f"THERMAL_START ({op}) failed with status={start_h['status']}, "
                                   f"{format_i2c_diag(start_h)}")
            for idx in range(args.frames):
                sock.sendall(b'c')
                h, data = read_packet(sock)
                print_inspection_report(h, data, use_ansi=not args.no_ansi)
                if dump_dir and data:
                    stem = dump_dir / f"frame_{mode_name}_seq{h['seq']:04d}"
                    stem.with_suffix('.bin').write_bytes(data)
                    ppm, stats = decode_frame(h, data)
                    stem.with_suffix('.ppm').write_bytes(ppm)
                    rgb = ppm.split(b'\n', 3)[3]
                    stem.with_suffix('.bmp').write_bytes(encode_bmp(h['width'], h['height'], rgb))
                    meta = inspect_packet(h, data)
                    stem.with_suffix('.json').write_text(json.dumps(meta, indent=2, ensure_ascii=False))
                    print(f"[ĐÃ LƯU FILE KIỂM CHỨNG] {stem}.{{bin,json,ppm,bmp}}")
                if idx + 1 < args.frames:
                    time.sleep(max(0.0, 1.0 / args.fps))
        sock.sendall(b's')
        read_packet(sock)


def pack_header(h):
    """Preserve the wire ABI for replay with read_packet()."""
    return HEADER.pack(*(h[k] for k in (
        'magic', 'version', 'header_bytes', 'status', 'seq', 'mode', 'width',
        'height', 'payload_bytes', 'timestamp_ms', 'index_image', 'index_temp',
        'flags', 'vtemp', 'reserved0', 'reserved1')))


def run_data(args):
    """Receive validated raw frames without RGB, GUI, or per-frame console output."""
    from contextlib import nullcontext
    op = {'temp': b't', 'image': b'i', 'dual': b'd', 'all': b't'}[args.mode]
    count = total = 0
    start = report = time.monotonic()
    with (open(args.output, 'wb') if args.output else nullcontext()) as output:
        with socket.create_connection((args.host, args.port), timeout=5) as sock:
            sock.settimeout(35)
            sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
            try:
                sock.sendall(op)
                h, _ = read_packet(sock)
                if h['status']:
                    raise RuntimeError('Camera start failed: %s' % h)
                start = report = time.monotonic()
                while not args.count or count < args.count:
                    begin = time.monotonic()
                    sock.sendall(b'c')
                    h, data = read_packet(sock)
                    if h['status'] or not data:
                        raise RuntimeError('Capture failed: %s' % h)
                    # Integration point: process h/data here; Y16 remains exact.
                    if output:
                        output.write(pack_header(h))
                        output.write(data)
                    count += 1
                    total += len(data)
                    now = time.monotonic()
                    if now - report >= 1:
                        print('seq=%d | %.1f fps | %.2f MiB/s | receive=%.1f ms' % (
                            h['seq'], count / (now - start), total / (now - start) / 1048576,
                            (now - begin) * 1000), file=sys.stderr, flush=True)
                        report = now
                    if args.count and count >= args.count:
                        break
                    time.sleep(max(0, 1 / args.fps - (time.monotonic() - begin)))
            except KeyboardInterrupt:
                pass
            finally:
                # Closing also makes the bridge stop the camera, even on a partial read.
                print('Received %d frames, %d payload bytes' % (count, total), file=sys.stderr)


def run_scan(args):
    """Đọc kết quả address-only scan từ lệnh INFO sau START lỗi."""
    with socket.create_connection((args.host, args.port), timeout=5) as sock:
        sock.settimeout(35)
        sock.sendall(b'?')
        h, _ = read_packet(sock)
        if h['index_image'] == 0 and h['index_temp'] == 0:
            print('Bus I2C chưa được khởi tạo; gửi START trước rồi thử --scan.')
        elif h['index_temp'] == 0:
            print('Address scan: không có địa chỉ 0x08..0x77 ACK.')
        else:
            print(f"Address scan: {h['index_temp']} địa chỉ ACK; đầu tiên=0x{h['index_image']:02x}.")
        print(format_i2c_diag(h))


WEB_HTML = """<!doctype html>
<html lang="vi">
<head>
<meta charset="utf-8">
<title>Tiny1C Thermal Live Inspector</title>
<style>
  body { font-family: system-ui, sans-serif; background: #0f141c; color: #e6edf3; margin: 0; padding: 18px; }
  .wrap { max-width: 1080px; margin: 0 auto; display: grid; grid-template-columns: 540px 1fr; gap: 18px; }
  .card { background: #161b22; border: 1px solid #30363d; border-radius: 8px; padding: 14px; }
  h2 { margin: 0 0 10px 0; font-size: 18px; color: #58a6ff; }
  .btns button { background: #21262d; color: #c9d1d9; border: 1px solid #30363d; padding: 7px 12px;
                 margin-right: 6px; border-radius: 6px; cursor: pointer; font-weight: 600; }
  .btns button:hover { background: #30363d; }
  img { width: 512px; image-rendering: pixelated; border: 1px solid #30363d; background: #090d13; display: block; margin-top: 10px; }
  pre { background: #0d1117; padding: 10px; border-radius: 6px; overflow-x: auto; font-size: 12.5px; line-height: 1.45; border: 1px solid #21262d; }
  .badge { display: inline-block; padding: 3px 8px; border-radius: 4px; font-size: 12px; font-weight: 700; background: #1f6feb; }
</style>
</head>
<body>
<div class="wrap">
  <div class="card">
    <h2>Luồng Hình Ảnh / Nhiệt Độ Tiny1C (256x192 / 256x384)</h2>
    <div class="btns">
      <button onclick="cmd('t')">Chế độ Nhiệt (Y16)</button>
      <button onclick="cmd('i')">Chế độ Ảnh (YUYV)</button>
      <button onclick="cmd('d')">Chế độ Kép (Ảnh + Nhiệt)</button>
      <button onclick="cmd('f')">Hiệu chuẩn Màn trập (FFC)</button>
    </div>
    <img id="view" src="/frame.bmp" alt="Thermal stream">
  </div>
  <div class="card">
    <h2>Chi Tiết Gói Dữ Liệu Truyền Sang (64B Header + Payload)</h2>
    <div id="summary" class="badge">Đang kết nối...</div>
    <pre id="meta">Đang đọc gói tin từ thermal_bridge...</pre>
  </div>
</div>
<script>
let lastSeq = null;
function cmd(op) { fetch('/cmd?op=' + op, {method: 'POST'}); }
async function tick() {
  try {
    const r = await fetch('/status.json?t=' + Date.now());
    const d = await r.json();
    if (d.header) {
      document.getElementById('summary').textContent =
        d.error ? `LỖI: ${d.error} | status=${d.header.status} | ${d.mode_desc}` :
        `status=${d.header.status} | seq=${d.header.seq} | ${d.mode_desc} | flags=${d.flags_desc}`;
      document.getElementById('meta').textContent = JSON.stringify(d, null, 2);
      if (lastSeq !== d.header.seq || d.error) {
        const view = document.getElementById('view');
        await new Promise(resolve => {
          view.onload = view.onerror = resolve;
          view.src = '/frame.bmp?s=' + d.header.seq + '&t=' + Date.now();
        });
        lastSeq = d.header.seq;
      }
    } else if (d.error) {
      document.getElementById('summary').textContent = 'LỖI: ' + d.error;
      document.getElementById('meta').textContent = JSON.stringify(d, null, 2);
    }
  } catch (e) {}
  setTimeout(tick, __POLL_MS__);
}
tick();
</script>
</body>
</html>
"""


def run_web(args):
    """Chạy máy chủ HTTP nội bộ để xem trực tiếp luồng dữ liệu Camera nhiệt trên trình duyệt."""
    commands, results = queue.Queue(), queue.Queue(maxsize=1)
    worker = CameraWorker(args.host, args.port, commands, results, args.fps, args.mode)
    worker.start()
    state = {'bmp': encode_bmp(256, 192, bytes(256 * 192 * 3)), 'info': {'status': 'connecting'}}
    lock = threading.Lock()

    def updater():
        while not worker.stop_event.is_set():
            try:
                kind, value = results.get(timeout=0.2)
            except queue.Empty:
                continue
            if kind == 'error':
                with lock:
                    state['info'] = dict(state['info'], error=value)
                continue
            h, data = value
            # Expensive conversion happens outside the HTTP snapshot lock.
            stats = None
            bmp = None
            begin = time.monotonic()
            if data and h['status'] == 0:
                ppm, stats = decode_frame(h, data)
                bmp = encode_bmp(h['width'], h['height'], ppm.split(b'\n', 3)[3])
            elif h['status'] != 0:
                bmp = encode_bmp(h['width'], h['height'], bytes(h['width'] * h['height'] * 3))
            info = inspect_packet(h, data, stats)
            info['render_ms'] = round((time.monotonic() - begin) * 1000, 2)
            with lock:
                state['info'] = info
                if bmp is not None:
                    state['bmp'] = bmp

    threading.Thread(target=updater, daemon=True).start()

    class Handler(BaseHTTPRequestHandler):
        def log_message(self, fmt, *a):
            pass

        def do_GET(self):
            if self.path.startswith('/frame.bmp'):
                with lock:
                    body = state['bmp']
                self.send_response(200)
                self.send_header('Content-Type', 'image/bmp')
                self.send_header('Cache-Control', 'no-store')
                self.end_headers()
                self.wfile.write(body)
            elif self.path.startswith('/status.json'):
                with lock:
                    body = json.dumps(state['info'], ensure_ascii=False).encode('utf-8')
                self.send_response(200)
                self.send_header('Content-Type', 'application/json; charset=utf-8')
                self.send_header('Cache-Control', 'no-store')
                self.end_headers()
                self.wfile.write(body)
            else:
                body = WEB_HTML.replace('__POLL_MS__', str(max(1, round(1000 / args.fps)))).encode('utf-8')
                self.send_response(200)
                self.send_header('Content-Type', 'text/html; charset=utf-8')
                self.end_headers()
                self.wfile.write(body)

        def do_POST(self):
            if self.path.startswith('/cmd?op='):
                op = self.path.split('=', 1)[1][:1]
                if op in ('t', 'i', 'd', 'f'):
                    commands.put(op)
            self.send_response(204)
            self.end_headers()

    srv = ThreadingHTTPServer(('0.0.0.0', args.web), Handler)
    print(f"Tiny1C Web Viewer đang chạy tại: http://127.0.0.1:{args.web} (Nhấn Ctrl+C để dừng)", flush=True)
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        worker.close()
        srv.server_close()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('host', nargs='?', default='127.0.0.1')
    parser.add_argument('--port', type=int, default=9000)
    parser.add_argument('--fps', type=float, default=25, help='Maximum requested display rate; not sensor FPS')
    parser.add_argument('--cli', '--inspect', action='store_true', dest='cli',
                        help='Chế độ kiểm tra chi tiết gói dữ liệu và vẽ bản đồ nhiệt trên Terminal')
    parser.add_argument('--scan', action='store_true',
                        help='Gửi INFO để quét địa chỉ I2C sau khi START lỗi (không ghi thanh ghi camera)')
    parser.add_argument('--mode', choices=('temp', 'image', 'dual', 'all'), default='all',
                        help='Chế độ thu nhận khi chạy --cli (mặc định: all - kiểm tra cả 3 chế độ)')
    parser.add_argument('--frames', type=int, default=1,
                        help='Số khung hình thu nhận mỗi chế độ khi chạy --cli')
    parser.add_argument('--dump-dir', type=str, default=None,
                        help='Thư mục xuất file .bin, .json, .ppm, .bmp khi chạy --cli')
    parser.add_argument('--no-ansi', action='store_true',
                        help='Tắt mã màu ANSI khi in bản đồ nhiệt trên Terminal')
    parser.add_argument('--web', type=int, default=0, metavar='HTTP_PORT',
                        help='Chạy Web Viewer tại cổng HTTP_PORT (ví dụ: --web 8080)')
    parser.add_argument('--run', choices=('stream', 'data'),
                        help='stream: Web đầy đủ; data: chỉ nhận dữ liệu, không dựng ảnh')
    parser.add_argument('--count', type=int, default=0, help='Data: số khung; 0 = liên tục')
    parser.add_argument('--output', help='Data: lưu liên tiếp header THM1 + payload vào file')
    args = parser.parse_args()
    if args.count < 0:
        parser.error('--count must be >= 0')
    if args.run and (args.cli or args.scan):
        parser.error('--run cannot be combined with --cli/--scan')
    if args.run == 'data' and args.web:
        parser.error('--run data cannot be combined with --web')
    if not 0 < args.fps <= 25:
        parser.error('--fps must be in (0,25]')

    if args.run == 'data':
        run_data(args)
        return
    if args.run == 'stream' and not args.web:
        args.web = 8080

    if args.scan:
        run_scan(args)
        return
    if args.cli:
        run_cli(args)
        return
    if args.web > 0:
        run_web(args)
        return

    try:
        import tkinter as tk
        from tkinter import filedialog, messagebox
    except ImportError:
        print('[THÔNG BÁO] Máy hiện tại chưa cài gói python3-tk (tkinter). '
              'Tự động chuyển sang chế độ kiểm tra trực tiếp trên Terminal (--cli).\n'
              '(Gợi ý: Bạn cũng có thể chạy `--web 8080` để xem trực tiếp trên trình duyệt web!)\n',
              file=sys.stderr)
        run_cli(args)
        return

    root = tk.Tk()
    root.title('Tiny1C thermal camera')
    root.geometry('760x900')
    commands, results = queue.Queue(), queue.Queue(maxsize=1)
    status = tk.StringVar(value='Connecting...')
    temperatures = tk.StringVar(value='No frame')
    auto = tk.BooleanVar(value=True)
    low, high = tk.StringVar(value='20'), tk.StringVar(value='40')
    top = tk.Frame(root)
    top.pack(fill='x', padx=10, pady=8)
    for label, op in [('Nhiet', 't'), ('Anh YUYV', 'i'), ('Anh + Nhiet', 'd'), ('FFC', 'f')]:
        tk.Button(top, text=label, command=lambda c=op: commands.put(c)).pack(side='left', padx=3)
    latest = [None]

    def save():
        if latest[0] is None:
            return
        filename = filedialog.asksaveasfilename(defaultextension='.bin', filetypes=[('Raw frame', '*.bin')])
        if not filename:
            return
        try:
            h, data = latest[0]
            path = Path(filename)
            path.write_bytes(data)
            path.with_suffix('.json').write_text(json.dumps(h, indent=2))
            ppm, _ = decode_frame(h, data)
            path.with_suffix('.ppm').write_bytes(ppm)
            status.set('Saved raw + metadata + PPM: ' + filename)
        except OSError as exc:
            messagebox.showerror('Save error', str(exc))

    tk.Button(top, text='Luu frame', command=save).pack(side='left', padx=3)
    scales = tk.Frame(root)
    scales.pack(fill='x', padx=10)
    tk.Checkbutton(scales, text='Auto scale', variable=auto).pack(side='left')
    for text, var in [('Min C', low), ('Max C', high)]:
        tk.Label(scales, text=text).pack(side='left')
        tk.Entry(scales, textvariable=var, width=6).pack(side='left')
    tk.Label(root, textvariable=status, wraplength=730, justify='left').pack(fill='x', padx=10, pady=6)
    tk.Label(root, textvariable=temperatures).pack()
    canvas = tk.Label(root, bg='#151923')
    canvas.pack(expand=True, fill='both', padx=10, pady=10)
    worker = CameraWorker(args.host, args.port, commands, results, args.fps, args.mode)
    worker.start()
    last_time = [None]

    def poll():
        try:
            kind, value = results.get_nowait()
            if kind == 'error':
                status.set('ERROR: ' + value)
            else:
                h, data = value
                if h['status']:
                    status.set('Camera error %d (frame rejected); seq=%d' % (h['status'], h['seq']))
                elif data:
                    latest[0] = value
                    try:
                        lo, hi = (None, None) if auto.get() else (float(low.get()), float(high.get()))
                        if lo is not None and hi <= lo:
                            raise ValueError('Max must exceed Min')
                        ppm, stats = decode_frame(h, data, lo, hi)
                        photo = tk.PhotoImage(data=ppm, format='PPM').zoom(2, 2)
                        canvas.configure(image=photo)
                        canvas.image = photo
                        now = time.monotonic()
                        fps = 0 if last_time[0] is None else 1 / (now - last_time[0])
                        last_time[0] = now
                        source = 'SIMULATED - KHONG PHAI CAMERA THAT' if h['flags'] & 1 else 'CAMERA'
                        quality = ', '.join(label for bit, label in ((2, 'SHUTTER CLOSED'), (4, 'FROZEN'), (16, 'DUAL INDEX MISMATCH')) if h['flags'] & bit)
                        if quality:
                            source += ' | ' + quality
                        status.set('%s | seq %d | %.1f display fps | index %d/%d | flags 0x%x' %
                                   (source, h['seq'], fps, h['index_image'], h['index_temp'], h['flags']))
                        temperatures.set(('Min %.2f C   Max %.2f C   Center %.2f C   Scale %.1f..%.1f C' %
                                          tuple(stats[k] for k in ('min_c', 'max_c', 'center_c', 'scale_min_c', 'scale_max_c')))
                                         if stats else 'YUYV image (not temperature pixels)')
                    except ValueError as exc:
                        status.set(str(exc))
        except queue.Empty:
            pass
        root.after(30, poll)

    def close():
        worker.close()
        root.destroy()

    root.protocol('WM_DELETE_WINDOW', close)
    poll()
    root.mainloop()


if __name__ == '__main__':
    main()
