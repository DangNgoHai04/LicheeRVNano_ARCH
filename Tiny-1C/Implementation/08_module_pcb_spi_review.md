# Đối chiếu pad SPI của PCB Tiny_1C

Nguồn kiểm tra: `/home/haidang/Downloads/tiny/src/Tiny_1C.kicad_pcb`,
`Tiny_1C.kicad_sch`, `Tiny_1C.pdf` và `huong_dan.md`. Kiểm tra tĩnh từ
net/pad trong KiCad; chưa thay thế phép đo dạng sóng trên PCB thật. Máy hiện
không có `kicad-cli`, nên chưa chạy DRC của KiCad.

## Kết quả ánh xạ

| Tín hiệu | LicheeRV Nano FPC2 | PCB J4 | U4 TXS0108E phía 1,8 V | U4 phía 3,3 V | PCB J1 đến camera |
|---|---:|---:|---:|---:|---:|
| SCK | 15 (`SPI0_SCK`) | 4 | A1, chân IC 1 | B1, chân IC 20 | 10 (`SCLK`) |
| MOSI | 18 (`SPI0_SDO`) | 6 | A2, chân IC 3 | B2, chân IC 18 | 8 (`MOSI`) |
| CS active low | 14 (`SPI0_CS`) | 8 | A3, chân IC 4 | B3, chân IC 17 | 6 (`CSN`) |
| MISO | 17 (`SPI0_SDI`) | 10 | A4, chân IC 5 | B4, chân IC 16 | 14 (`MISO`) |

Các net trên PCB và sơ đồ khớp nhau. J1 là đầu nối 40 tiếp điểm
`WP7B-P040VA1`; đây là đầu nối khác FPC2 31 tiếp điểm của Nano. Bảng camera
trong `huong_dan.md` ghi J1 pad 6/8/10/14 tới chân camera 3/4/5/7 thông qua
cáp/mating connector. File KiCad chỉ xác nhận phía PCB J1; đường qua cáp tới
module phải dựa vào phép đo thông mạch bạn đã thực hiện.

## Điều kiện để đường SPI đi qua U4

- U4 VCCA/chân 2 = net `1V8_NANO`, lấy tại J4.2 từ Nano FPC2.26.
- U4 VCCB/chân 19 = net `3V3_CAM`, lấy từ nguồn PCB.
- U4 OE/chân 10 = net `NANO_IO_EN`; JP1 phải nối **1–2** để đưa 1,8 V tới OE.
  R7 100 kΩ kéo OE xuống GND nếu không có shunt. Theo datasheet TI, OE thấp
  đặt các chân A/B ở trạng thái trở kháng cao.
- GND Nano và PCB camera phải nối chung, ví dụ Nano FPC2.25 tới PCB J5.3.
- SW4 là nhánh chọn host J3 riêng; khi không hàn, đường J4 → U4 → J1 vẫn
  đúng theo netlist.

TXS0108E là bộ chuyển mức tự nhận chiều. [Datasheet TI](https://www.ti.com/lit/ds/symlink/txs0108e.pdf) cho phép push-pull nhưng
lưu ý điện dung của trace/cáp và xung phản xạ có thể làm hỏng biên dạng xung;
việc nối đúng net và thông mạch DC chưa chứng minh SPI 11,71875 MHz hoạt động.

## Đánh giá thiết kế mạch

**Không thấy lỗi tráo pad SPI trong hai file KiCad.** U4 được nối đúng A1↔B1
cho SCK, A2↔B2 cho MOSI, A3↔B3 cho CS và A4↔B4 cho MISO. Nguồn camera dùng
`5V_CAM` tại J1.1/3 và `3V3_CAM` tại J1.35/37/38/39; nguồn 1,8 V từ Nano
chỉ là nguồn tham chiếu phía A của U4 và OE, không cấp nguồn chính cho camera.

Các rủi ro thiết kế cần xác minh trên board đã lắp:

1. **OE phụ thuộc jumper JP1.** OE có điện trở R7 100 kΩ kéo xuống GND.
   Nếu jumper thiếu/sai vị trí hoặc J4.2 không đủ 1,8 V, cả sáu kênh U4 bị
   cô lập dù đường đồng trên PCB và cáp vẫn thông mạch.
2. **TXS0108E với SPI và cáp.** Bộ này tự nhận chiều, có mạch tăng tốc cạnh.
   Datasheet TI cho phép push-pull nhưng yêu cầu kiểm soát điện dung và độ dài
   đường mạch/cáp. Không coi tốc độ danh định của IC là bằng chứng xung tại
   module đạt ở 11,71875 MHz. Đo cả hai phía U4; chỉ cân nhắc đổi IC hoặc
   thêm điện trở nối tiếp sau khi thấy dạng sóng thực tế bị méo/ringing.
3. **RESETN không do Nano điều khiển trực tiếp.** J1.30 (`RESETN`) dựa vào
   mạng RC/diode, đồng thời ra J3.9. J4 của Nano không có chân RESETN. Nếu
   camera không thoát reset đúng lúc, firmware Nano hiện không thể phát một
   xung reset phần cứng; cần đo J1.30 sau khi bật nguồn và khi chụp.
4. **Đường cáp tới module chưa có trong file KiCad.** Netlist chỉ kết thúc ở
   J1. Bảng ánh xạ J1 sang chân camera trong `huong_dan.md` là thông tin về
   cáp/mating connector, cần đối chiếu phép đo trên cáp thật; phép đo này đã
   được người vận hành thực hiện nhưng chưa có số liệu từng cặp trong hồ sơ.
5. **SW4 nếu được lắp/đóng** nối các đường host J3 song song vào đường camera.
   Khi chỉ dùng Nano, để SW4 OFF hoặc không lắp; nếu J3 có host khác đang lái
   sẽ gây tranh chấp tín hiệu.

Chưa chạy được KiCad ERC/DRC vì môi trường không có `kicad-cli`/`pcbnew`.
Kết luận trên là kiểm tra sơ đồ/net/pad, không phải chứng nhận PCB sản xuất
không có lỗi hàn, đứt vi mạch hoặc ngắn mạch.

## Thứ tự đo khi CAPTURE còn trả `status=-2`

1. Đo DC so với GND PCB: J4.2/U4.2 khoảng 1,8 V; U4.19 khoảng 3,3 V;
   U4.10 (OE) gần mức J4.2; J1.1/3 khoảng 5 V; J1.30 (`RESETN`) lên mức
   HIGH sau khởi động. Nếu OE thấp, kiểm tra JP1 nối 1–2.
2. Dùng oscilloscope/logic analyzer đo lúc CAPTURE: J4.8 (CS 1,8 V),
   J4.4 (SCK 1,8 V), J4.6 (MOSI 1,8 V). CS phải hạ LOW và SCK có xung.
3. Đo cùng tín hiệu phía camera: J1.6 (CS 3,3 V), J1.10 (SCK 3,3 V),
   J1.8 (MOSI 3,3 V). Nếu phía J4 có xung mà J1 không, kiểm tra U4/OE/nguồn.
4. Đo MISO tại J1.14 (phía 3,3 V) và J4.10/U4.5 (phía 1,8 V) trong lúc CS
   LOW. Nếu J1.14 có xung mà J4.10 không, tập trung vào U4 B4↔A4. Nếu cả
   hai không có xung nhưng CS/SCK/MOSI đến module, tập trung vào trạng thái
   camera, nguồn/reset, đường cáp và cấu hình VoSPI.

Không suy lỗi TXS chỉ từ MISO HIGH lúc rảnh: thiết bị SPI slave có thể nhả
MISO khi CS HIGH. Phép thử pull nội trước đây cho thấy MISO không được lái
trong phiên CAPTURE, nhưng chưa chỉ ra lỗi ở camera hay U4.
