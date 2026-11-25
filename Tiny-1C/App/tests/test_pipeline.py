"""
Bộ kiểm thử tự động toàn tuyến cho Ứng dụng Camera nhiệt Tiny1C (test_pipeline.py):
  1. test_modes_and_conversion: Kiểm tra cả 3 chế độ (MODE_TEMP 't', MODE_IMAGE 'i', MODE_DUAL 'd'),
     thu khung hình ('c'), giải mã PPM & nhiệt độ (°C), hiệu chuẩn FFC ('f') và dừng luồng ('s').
  2. test_invalid_packet: Kiểm tra bộ thu từ chối gói tin có Header 64-byte sai định dạng ABI.
  3. test_temperature_endian_and_scale: Kiểm tra giải mã Little-Endian Y16 -> °C và thang màu RGB.
  4. test_hardware_requires_resource_confirmation: Kiểm tra thermal_bridge từ chối chạy phần cứng
     nếu người vận hành chưa truyền cờ `--resources-owned`.
  5. test_cli_inspector_and_bmp_export: Kiểm tra tính năng xem chi tiết gói dữ liệu truyền sang
     (`inspect_packet`, `render_ascii_heatmap`, `encode_bmp` và chạy `--cli --dump-dir`).
"""
import importlib.util
import socket
import struct
import subprocess
import tempfile
import time
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('viewer', ROOT / 'pc/thermal_viewer.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


class Pipeline(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with socket.socket() as s:
            s.bind(('127.0.0.1', 0))
            cls.port = s.getsockname()[1]
        cls.proc = subprocess.Popen(
            [str(ROOT / 'build/thermal_bridge_host'), '--simulate', '--port', str(cls.port)],
            stdout=subprocess.DEVNULL
        )
        for _ in range(100):
            try:
                cls.sock = socket.create_connection(('127.0.0.1', cls.port), .1)
                cls.sock.settimeout(5)
                return
            except OSError:
                time.sleep(.02)
        raise RuntimeError('Bridge did not start')

    @classmethod
    def tearDownClass(cls):
        cls.sock.close()
        cls.proc.terminate()
        cls.proc.wait(timeout=5)

    def request(self, op):
        self.sock.sendall(op.encode())
        return v.read_packet(self.sock)

    def test_modes_and_conversion(self):
        for op, mode, size in [('t', 1, 98304), ('i', 0, 98304), ('d', 2, 196608)]:
            h, data = self.request(op)
            self.assertEqual(h['status'], 0)
            self.assertFalse(data)
            h, data = self.request('c')
            self.assertEqual((h['mode'], len(data)), (mode, size))
            self.assertTrue(h['flags'] & 1)
            ppm, stats = v.decode_frame(h, data)
            self.assertTrue(ppm.startswith(b'P6\n256 '))
            if mode != 0:
                self.assertAlmostEqual(stats['max_c'], 38, delta=.02)
                self.assertAlmostEqual(stats['min_c'], 22, delta=.02)
            self.assertEqual(self.request('f')[0]['status'], 0)
        self.request('s')
        h, data = self.request('c')
        self.assertNotEqual(h['status'], 0)
        self.assertFalse(data)

    def test_invalid_packet(self):
        a, b = socket.socketpair()
        try:
            a.sendall(bytes(64))
            with self.assertRaises(ValueError):
                v.read_packet(b)
        finally:
            a.close()
            b.close()

    def test_i2c_diag_decoding(self):
        h = {'reserved0': (-3) & 0xffffffff, 'reserved1': (1 << 24) | (3 << 16) | 0x1d00}
        self.assertEqual(v.format_i2c_diag(h),
                         'probe=PROJECT, reg=0x1d00, BB=NACK(-3), nack_at=msg0.byte2')

    def test_temperature_endian_and_scale(self):
        data = struct.pack('<H', int((25 + 273.15) * 64)) * 49152
        rgb, stats = v.thermal_rgb(data, 20, 40)
        self.assertEqual(len(rgb), 147456)
        self.assertAlmostEqual(stats['center_c'], 25, delta=.02)

    def test_hardware_requires_resource_confirmation(self):
        p = subprocess.run([str(ROOT / 'build/thermal_bridge_host')], capture_output=True)
        self.assertEqual(p.returncode, 2)

    def test_cli_inspector_and_bmp_export(self):
        self.request('d')
        h, data = self.request('c')
        info = v.inspect_packet(h, data)
        self.assertIn('image_plane', info)
        self.assertIn('temp_plane', info)
        self.assertEqual(info['temp_plane']['offset_bytes'], 98304)
        heatmap = v.render_ascii_heatmap(data[98304:], cols=32, rows=8, use_ansi=False)
        self.assertEqual(len(heatmap.splitlines()), 8)
        ppm, _ = v.decode_frame(h, data)
        rgb = ppm.split(b'\n', 3)[3]
        bmp = v.encode_bmp(h['width'], h['height'], rgb)
        self.assertTrue(bmp.startswith(b'BM'))
        self.request('s')


class FastPaths(unittest.TestCase):
    def test_bmp_pixels_and_padding(self):
        bmp = v.encode_bmp(1, 2, bytes([255, 0, 0, 0, 255, 0]))
        self.assertEqual(bmp[54:], bytes([0, 255, 0, 0, 0, 0, 255, 0]))

    def test_grayscale_matches_color_formula(self):
        data = b''.join(bytes([y, 128, y, 128]) for y in range(256)) * 96
        expected = b''.join(bytes([v.clamp(1.164 * max(0, y - 16))]) * 6
                            for y in range(256)) * 96
        self.assertEqual(v.yuyv_rgb(data), expected)

    def test_stats_without_render_and_latest_frame(self):
        from unittest.mock import patch
        import queue
        payload = struct.pack('<H', 19000) * 49152
        with patch.object(v, 'THERMAL_LUT', None):
            rgb, stats = v.thermal_rgb(payload, render=False)
        self.assertIsNone(rgb)
        self.assertAlmostEqual(stats['center_c'], 19000 / 64 - 273.15)
        results = queue.Queue(maxsize=1)
        worker = v.CameraWorker('', 0, queue.Queue(), results, 25, 'dual')
        worker.publish(('frame', 1))
        worker.publish(('frame', 2))
        self.assertEqual(results.get_nowait(), ('frame', 2))


class RuntimeModes(unittest.TestCase):
    def setUp(self):
        with socket.socket() as sock:
            sock.bind(('127.0.0.1', 0))
            self.port = sock.getsockname()[1]
        self.bridge = subprocess.Popen(
            [str(ROOT / 'build/thermal_bridge_host'), '--simulate', '--port', str(self.port)],
            stdout=subprocess.PIPE, text=True)
        self.addCleanup(self.stop_bridge)
        self.assertIn('SIMULATED', self.bridge.stdout.readline())

    def stop_bridge(self):
        self.bridge.terminate()
        self.bridge.wait(timeout=5)
        self.bridge.stdout.close()

    def test_data_recording_all_modes(self):
        import sys
        for mode, size in [('temp', 98304), ('image', 98304), ('dual', 196608)]:
            with tempfile.TemporaryDirectory() as directory:
                path = Path(directory) / 'frames.thm'
                proc = subprocess.run([sys.executable, str(ROOT / 'pc/thermal_viewer.py'),
                    '--port', str(self.port), '--run', 'data', '--mode', mode,
                    '--count', '3', '--output', str(path)], capture_output=True, timeout=10)
                self.assertEqual(proc.returncode, 0, proc.stderr)
                raw = path.read_bytes()
                self.assertEqual(len(raw), 3 * (64 + size))
                for offset in range(0, len(raw), 64 + size):
                    fields = v.HEADER.unpack(raw[offset:offset + 64])
                    self.assertEqual(fields[0], v.MAGIC)
                    self.assertEqual(fields[8], size)
                    self.assertEqual(fields[3], 0)

    def test_web_stream_commands_and_status(self):
        import sys
        import json
        import urllib.request
        with socket.socket() as sock:
            sock.bind(('127.0.0.1', 0))
            port = sock.getsockname()[1]
        proc = subprocess.Popen([sys.executable, str(ROOT / 'pc/thermal_viewer.py'),
            '--port', str(self.port), '--run', 'stream', '--web', str(port)],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        try:
            base = 'http://127.0.0.1:%d' % port
            def get(path):
                with urllib.request.urlopen(base + path, timeout=2) as response:
                    return response.read()
            for _ in range(100):
                try:
                    html = get('/')
                    break
                except OSError:
                    time.sleep(.02)
            else:
                self.fail('Web server did not start')
            self.assertNotIn(b'__POLL_MS__', html)
            for op, mode in [('t', 1), ('i', 0), ('d', 2), ('f', 2)]:
                with urllib.request.urlopen(urllib.request.Request(
                        base + '/cmd?op=' + op, method='POST'), timeout=2):
                    pass
                for _ in range(100):
                    info = json.loads(get('/status.json'))
                    h = info.get('header', {})
                    if h.get('mode') == mode and h.get('payload_bytes', 0):
                        break
                    time.sleep(.02)
                else:
                    self.fail('No frame after command ' + op)
                self.assertEqual(h['status'], 0)
                self.assertTrue(get('/frame.bmp').startswith(b'BM'))
                self.assertIn('render_ms', info)
        finally:
            proc.terminate()
            proc.communicate(timeout=5)
