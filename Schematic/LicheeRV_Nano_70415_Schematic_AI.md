# LicheeRV Nano 70415 - Schematic dạng văn bản dành cho AI

Tài liệu này chuyển nội dung sơ đồ điện sang Markdown để có thể đọc độc lập, không cần AI nhìn được PDF. Nội dung gồm kiến trúc mạch, đường nguồn, linh kiện, kết nối giữa các khối, bảng chân đầu nối và toàn bộ chức năng chân được in trong symbol U1. Tên net và ký hiệu linh kiện được giữ theo bản gốc.

| Thuộc tính | Giá trị trong nguồn |
| --- | --- |
| Tệp nguồn | `LicheeRV_Nano-70415_Schematic.pdf` |
| Tên thiết kế | `LicheeRV Nano 70415` |
| Đơn vị trên khung tên | Shenzhen Sipeed Techno. Co,Ltd. |
| Revision | `1.3` |
| Ngày trên sơ đồ | `24-2-28`, tức 2024-02-28 |
| Số trang | 4 |
| Trang 1 | `Config`: hình bo mạch và định nghĩa pad |
| Trang 2 | `GC2002-QFN88`: U1, nguồn cấp cho U1, thạch anh, điện trở cấu hình |
| Trang 3 | `Power/SYS/WiFi`: nguồn, Wi-Fi/BT, backlight, LED, nút nhấn |
| Trang 4 | `Interface`: đầu nối, lưu trữ, camera, Ethernet, audio, pad mở rộng |
| SHA-256 của PDF | `ee8211a4c75c40f2327114a93b6d4aa4a80b8a6cf057b5eda5bb5a64365e4955` |

## 1. Ngữ cảnh và quy tắc diễn giải cho AI

Đây là bản mô tả schematic, không phải netlist xuất từ phần mềm CAD, không phải PCB layout và không phải datasheet. Thông tin được lấy từ chữ và các đường nối nhìn thấy trong cả bốn trang PDF. Các số chân U1, U13, U4... là số chân của symbol trong sơ đồ; `J1` đến `J28` là định danh pad trên bo, không phải số GPIO trong Linux.

Trong tài liệu, một tên net lặp lại ở các khối khác nhau biểu thị cùng tín hiệu theo sơ đồ. Đường nối qua điện trở, tụ, cuộn dây hoặc IC phải được hiểu là đi qua linh kiện đó, không được gộp thành nối trực tiếp. `P/N`, `+/-` trong tên tín hiệu được giữ nguyên; không tự đảo cực để khớp một sơ đồ mẫu khác.

Chuỗi chức năng phân cách bằng `/` trong symbol U1 là các khả năng ghép kênh của một chân. Chúng không đồng thời hoạt động và không chứng minh firmware hiện tại đã chọn chức năng nào. Hình pinout trang 1 chỉ thể hiện một tập chức năng; khi tên bus trong hình khác tên trong symbol, tài liệu giữ cả hai cách ghi.

`NC` ở vị trí giá trị linh kiện, chẳng hạn `R7`, `R8`, `R50`, `RN1`, `C54`, là vị trí không lắp/để hở theo cách thể hiện của sơ đồ. `NC` hoặc dấu gạch chéo ở chân IC/đầu nối nghĩa là chân không nối trong sơ đồ. `DNP` là nhánh được đánh dấu không lắp. PDF không phải BOM lắp ráp của từng bo thực tế, nên trạng thái linh kiện trên một bo cụ thể vẫn có thể khác theo biến thể.

Điện áp ghi bằng tên rail là điện áp danh định của thiết kế. Các nhận xét về trạng thái logic, cầu chia điện áp và dòng backlight được đánh dấu là suy luận từ mạch. Không suy ra giới hạn tuyệt đối, khả năng chịu 5 V, dòng tối đa, dung lượng RAM/flash, tốc độ CPU, địa chỉ I2C, baud rate UART hoặc cấu hình hệ điều hành khi PDF không cung cấp.

## 2. Kiến trúc tổng thể

| Khối | Linh kiện/đầu nối chính | Kết nối và vai trò |
| --- | --- | --- |
| Xử lý trung tâm | U1, trang 2 | Kết nối SDIO0, SDIO1, USB, Ethernet, MIPI, audio, GPIO và điều khiển hệ thống; tên sheet là `GC2002-QFN88` |
| Nguồn đầu vào | USB1, J13, J14, R65 | `VBUS` nối `VSYS` qua R65 = 0 ohm; trang 1 ghi hai pad này là 5 V |
| Ba nguồn DC/DC | U4 = `TMI7003C`, L1-L3 | Tạo `VDD3V3_SYS`, `VDD1V35_DRAM`, `VDD0V9_CPU` từ `VSYS` |
| Nguồn 1,8 V | U3 = `XC6206P182MR-MS` | LDO từ `VDD3V3_SYS` sang `VDD1V8_SYS` |
| Clock | X1 = 25 MHz | Giữa `XTAL_XOUT` và `XTAL_XIN` của U1 |
| Wi-Fi/BT | U13 = `AIC8800D-44Pin`, RF1 | Wi-Fi dùng SDIO1; các đường UART Bluetooth có RN1 ghi NC |
| Lưu trữ | U9 và U10 | Cùng nối bus SDIO0; U9 là symbol SD NAND, U10 là ổ thẻ SD theo symbol |
| USB Type-C | USB1 | D+/D- đi thẳng đến U1; SBU1/SBU2 nối GPIOA_16/GPIOA_17 |
| Màn hình | FPC2 | Ba cặp `MIPI_TX0/1/2`, nguồn, reset và backlight |
| Camera | FPC3 | Năm cặp `MIPI_RX0..4`, tín hiệu điều khiển, I2C và nguồn |
| Touch | FPC1 | Chung `CAM_SCL/CAM_SDA` với camera, kèm reset và interrupt riêng |
| Backlight | U14 = `AW9962EDNR`, L4, D1 | Mạch nâng áp, điều khiển bằng `BL_PWM`, xuất `LCD0_BL+/-` |
| Ethernet | U1, C49-C52, L5/L6, U7/U8, RJ1 | Khối được ghi `10/100M PHY`; không có IC PHY rời mang mã linh kiện trong khối này |
| Audio ra | U11 | Khuếch đại từ `AUD_OUT`, xuất hai net `VO_P/VO_N` |
| Microphone | U12 | Tín hiệu analog qua C57 vào `AUD_IN` |
| Pad mở rộng | J1-J28 | GPIO, SDIO1, audio, ADC và nguồn |

PDF không ghi mã thương mại đầy đủ của U1 ngay dưới symbol. Không tự thay tên `GC2002-QFN88` bằng một model khác chỉ dựa trên tên sản phẩm.

## 3. Các pad bên cạnh bo mạch: J1-J28

Nguồn: hình trang 1 và khối `Pin` trang 4. Nhìn như hình pinout trang 1, USB-C ở dưới: cột trái từ trên xuống là J1-J14, cột phải từ trên xuống là J15-J28. Vị trí này chỉ mô tả hướng nhìn trong PDF.

| Pad | Net trong schematic | Điểm nối U1 hoặc khối khác | Chức năng thể hiện trên hình pinout trang 1 |
| --- | --- | --- | --- |
| J1 | `GPIOA_17` | U1.19; USB1.SBU2; R40 kéo lên 3,3 V | GPIOA17, UART0 RX, PWM5 |
| J2 | `GPIOA_16` | U1.18; USB1.SBU1; R39 kéo lên 3,3 V | GPIOA16, UART0 TX, PWM4 |
| J3 | `GND` | GND | GND |
| J4 | `GPIOA_15` | U1.17 | GPIOA15 |
| J5 | `VO_P` | U11.VOP | VOP, ngõ audio |
| J6 | `VO_N` | U11.VON | VON, ngõ audio |
| J7 | `GPIOA_24` | U1.25 | GPIOA24, EMMC D1, SPI1 CS |
| J8 | `GPIOA_23` | U1.24 | GPIOA23, EMMC CMD, SPI1 MISO |
| J9 | `GPIOA_27` | U1.23 | GPIOA27, EMMC D3 |
| J10 | `GPIOA_25` | U1.22 | GPIOA25, EMMC D0, SPI1 MOSI |
| J11 | `GPIOA_22` | U1.21 | GPIOA22, EMMC CLK, SPI1 SCK |
| J12 | `GPIOA_26` | U1.20; R60/R58 thuộc cấu hình Wi-Fi | GPIOA26, EMMC D2 |
| J13 | `VSYS` | Nguồn hệ thống; nối VBUS qua R65 | 5 V |
| J14 | `VBUS` | VBUS của USB1; nối VSYS qua R65 | 5 V |
| J15 | `GPIOA_19` | U1.26; R63; một nhánh RN1 | GPIOA19, UART1 TX, JTAG TMS, PWM7 |
| J16 | `GPIOA_18` | U1.27; R62; một nhánh RN1 | GPIOA18, UART1 RX, JTAG TCK, PWM6 |
| J17 | `GND` | GND | GND |
| J18 | `GPIOA_29` | U1.29; R69; một nhánh RN1 | GPIOA29, UART2 RX, JTAG TDO |
| J19 | `ADC_PIN` | Qua R6 đến `ADC1` tại U1.59 | GPIOB3/ADC trong hình; thực tế có cầu chia trở |
| J20 | `GPIOA_28` | U1.28; R68; một nhánh RN1 | GPIOA28, UART2 TX, JTAG TDI |
| J21 | `SDIO1_D3` | U1.51 và U13.15 | GPIOP18, UART3 CTS, IIC1 SCL, PWM4, SPI2 CS, SDIO1 D3 |
| J22 | `SDIO1_D2` | U1.52 và U13.14 | GPIOP19, UART3 TX, PWM5, SDIO1 D2 |
| J23 | `SDIO1_D0` | U1.54 và U13.18 | GPIOP21, UART3 RTS, IIC1 SDA, PWM7, SPI2 MISO, SDIO1 D0 |
| J24 | `SDIO1_CMD` | U1.55 và U13.16 | GPIOP22, IIC3 SCL, PWM8, SPI2 MOSI, SDIO1 CMD |
| J25 | `SDIO1_CLK` | U1.56 và U13.17 | GPIOP23, IIC3 SDA, PWM9, SPI2 SCK, SDIO1 CLK |
| J26 | `SDIO1_D1` | U1.53 và U13.19 | GPIOP20, UART3 RX, PWM6, SDIO1 D1 |
| J27 | `GPIOA_14` | U1.15 và LED1 | GPIOA14 |
| J28 | `VDD3V3_SYS` | Nguồn 3,3 V | 3V3 |

Chú giải trang 1 ghi `GPIOA, Vio=3.3V`, `GPIOB, Vio=3.3V`, `GPIOP, Vio=3.3V` cho các nhóm trong hình. Điều này không thay thế giới hạn điện áp trong datasheet, và không áp dụng mặc định cho các tín hiệu camera/touch được ghi riêng là 1,8 V.

`SPI1` trong hình pinout của J7/J8/J10/J11 tương ứng các chân có tên `SPINOR_*`/`SPINAND_*` trong symbol U1. PDF không cung cấp ánh xạ từ cách đặt tên này sang số bus SPI của hệ điều hành.

## 4. Hệ thống nguồn

### 4.1. Nguồn vào và các rail

Nguồn: trang 3 và khối USB/Pin trang 4. USB1 có hai chân được ghi `VBUS`, cùng nối net `VBUS`. C25 và C26 đều 10 uF, mắc từ VBUS xuống GND. R65 = 0 ohm nối `VBUS` với `VSYS`; J14 nối phía VBUS, J13 nối phía VSYS. Vì có R65, hai pad không phải hai nguồn độc lập trong cấu hình có lắp điện trở này.

| Net | Mức danh định theo nguồn | Nguồn tạo/cấp | Tải chính được vẽ |
| --- | --- | --- | --- |
| `VBUS` | 5 V trên hình pinout | USB1 hoặc pad J14 | Qua R65 sang VSYS; C25/C26 |
| `VSYS` | 5 V trên hình pinout | VBUS qua R65 hoặc pad J13 | U4, U14, U11 |
| `VDD3V3_SYS` | 3,3 V | U4 kênh 1, L1 | U1, U3, U13, U9/U10, U12, FPC, LED2, các pull-up, J28 |
| `VDD1V35_DRAM` | 1,35 V theo tên rail | U4 kênh 2, L2 | U1.31/.32/.33/.34 |
| `VDD0V9_CPU` | 0,9 V theo tên rail | U4 kênh 3, L3 | U1.9/.16/.50/.61/.71 |
| `VDD1V8_SYS` | 1,8 V | U3 | Nguồn analog/RTC I/O U1, điện trở strap, reset, FPC2 và pull-up I2C |
| `VDDC_RTC` | Không có mức điện áp ghi rõ | Nối U1.37 và C21 | PDF không thể hiện bộ nguồn rời tạo net này |
| `ADU_VREF` | Điện áp tham chiếu audio, không ghi trị số | Nối U1.3 và C22/C23 | Giữ nguyên cách viết `ADU`, không đổi thành tên net khác |

### 4.2. U4 - TMI7003C, ba kênh DC/DC

| Nhóm chân U4 | Kết nối |
| --- | --- |
| VIN1.10, VIN2.7, VIN3.19 | `VSYS`; C30 = 10 uF xuống GND |
| EN1.9, EN2.8, EN3.18 | Đều nối `VSYS` |
| SW1.11 | Qua L1 = 2,2 uH đến `VDD3V3_SYS` |
| FB1.14 | Điểm giữa R15 và R12 |
| SW2.6 | Qua L2 = 2,2 uH đến `VDD1V35_DRAM` |
| FB2.3 | Điểm giữa R16 và R13 |
| SW3.20 | Qua L3 = 2,2 uH đến `VDD0V9_CPU` |
| FB3.2 | Điểm giữa R17 và R14; có nhánh tùy chọn R141 |
| GND.1/.4/.5/.12/.13/.15/.17 và EP.21 | GND |
| NC.16 | Không nối |

| Kênh | Trở từ đầu ra đến FB | Trở từ FB xuống GND | Tụ đầu ra xuống GND |
| --- | --- | --- | --- |
| 3,3 V | R15 = 150 kohm | R12 = 33 kohm | C76 = 10 uF, C77 = 10 uF, C78 = 22 uF |
| 1,35 V | R16 = 43 kohm | R13 = 33 kohm | C79 = 22 uF, C75 = 10 uF |
| 0,9 V | R17 = 33 kohm | R14 = 56 kohm | C80 = 22 uF, C72 = 10 uF |

Nhánh trong khung `DNP`: `PWR_PWM` từ U1.58 đi qua R140 = 1 kohm đến một nút có C81 = 2,2 uF xuống GND; từ nút đó đi qua R141 = 40,2 kohm đến FB3 của U4. Có thể suy luận đây là nhánh lọc PWM tác động vào hồi tiếp nguồn CPU nếu được lắp, nhưng PDF đánh dấu DNP nên không được coi là tính năng đang hoạt động mặc định.

### 4.3. U3 - nguồn 1,8 V

U3 có mã `XC6206P182MR-MS`. Chân 3 VIN nối `VDD3V3_SYS`; chân 2 VOUT nối `VDD1V8_SYS`; chân 1 VSS nối GND. C36 = 10 uF lọc phía vào, C42 = 10 uF lọc phía ra, đều xuống GND. Không có mức dòng tối đa của LDO trong PDF.

### 4.4. Tụ decoupling ở U1

Nguồn: trang 2. Tất cả các tụ dưới đây mắc từ net tương ứng xuống GND.

| Net | Linh kiện và giá trị |
| --- | --- |
| `VDD0V9_CPU` | C1, C2, C4: mỗi tụ 2,2 uF; C5: 100 nF |
| `VDD1V35_DRAM` | C7: 2,2 uF; C8: 100 nF |
| `VDD3V3_SYS` | C9, C10: mỗi tụ 2,2 uF; C11, C12, C13: mỗi tụ 100 nF |
| `VDD1V8_SYS` | C14: 2,2 uF; C17, C19, C20: mỗi tụ 100 nF |
| `VDDC_RTC` | C21: 100 nF |
| `ADU_VREF` | C22: 2,2 uF; C23: 100 nF |

## 5. Clock, cấu hình khởi động, reset và LED

### 5.1. Thạch anh

Nguồn: trang 2. X1 = 25 MHz nằm giữa `XTAL_XOUT` tại U1.46 và `XTAL_XIN` tại U1.45. X1.1 nối XTAL_XOUT; X1.3 nối XTAL_XIN. R1 = 1 Mohm mắc song song giữa hai net này. C3 = 10 pF từ XTAL_XOUT xuống GND, C6 = 10 pF từ XTAL_XIN xuống GND. Các chân vỏ X1.2 và X1.4 nối GND.

### 5.2. Các điện trở strap và PWR_DET

| Tín hiệu | Chân U1 | Mạch nối | Ghi chú trong sơ đồ / suy luận |
| --- | --- | --- | --- |
| `CPU_SEL` | 67 | R2 = 5,1 kohm lên VDD1V8_SYS; R7 = NC xuống GND | Ghi rõ: 0 = A53, 1 = C906. Suy luận theo giá trị lắp được ghi: kéo cao, chọn C906 |
| `USB_DET` | 60 | R3 = 5,1 kohm lên VDD1V8_SYS; R8 = NC xuống GND | Ghi rõ: 1 = Device, 0 = Host. Suy luận: mặc định kéo cao, Device |
| `PWR_DET` | 38 | VDD1V8_SYS qua R4 = 33 kohm đến PWR_DET; R9 = 56 kohm xuống GND | Đây là cầu chia từ nguồn 1,8 V, không phải dây nối trực tiếp VBUS |

Từ R4/R9, điện áp lý tưởng tại PWR_DET là `1,8 x 56 / (33 + 56) ≈ 1,133 V`, khi bỏ qua tải. Đây là phép tính từ schematic, không phải ngưỡng phát hiện được datasheet xác nhận.

### 5.3. Nút BOOT và RESET

Nguồn: trang 3. SW1 là nút nhấn BOOT/USER: `BOOT_KEY` nối U1.30, có R25 = 10 kohm kéo lên `VDD3V3_SYS`, nhấn nút nối net xuống GND. Vì vậy nhấn tạo mức thấp theo mạch.

SW2 là nút RESET: `SYS_RSTN` nối U1.39, có R64 = 10 kohm kéo lên `VDD1V8_SYS`, C63 = 100 nF xuống GND, nhấn nối xuống GND. Hằng số RC danh định từ R64/C63 là 1 ms; không đồng nghĩa với toàn bộ thời gian reset thực tế.

Tên linh kiện nút nhấn `SW1` không phải chân chuyển mạch `SW1` của U4; phải phân biệt theo reference và số chân.

### 5.4. LED

| LED | Đường nối | Diễn giải từ sơ đồ |
| --- | --- | --- |
| LED1 | `GPIOA_14` -> LED1 -> R28 = 5,1 kohm -> GND | LED do U1.15 điều khiển; cùng net với J27; hướng diode cho thấy mức cao có thể làm sáng |
| LED2 | `VDD3V3_SYS` -> LED2 -> R30 = 5,1 kohm -> GND | LED báo nguồn 3,3 V |

PDF không ghi màu hoặc mã linh kiện LED1/LED2.

## 6. Wi-Fi và Bluetooth

Nguồn: trang 3, khối `Wi-Fi & BT`, và trang 4, khối `Pin`. U13 được ghi `AIC8800D-44Pin`. Nguồn VBAT và VDDIO đều dùng `VDD3V3_SYS`. C46 = 10 uF, C47 = 10 uF, C48 = 100 nF mắc từ rail này xuống GND.

### 6.1. Nguồn, SDIO và tín hiệu điều khiển của U13

| Chân U13 | Tên chân trong symbol | Net/kết nối |
| --- | --- | --- |
| 1, 3, 5, 20, 31, 33, 36 | GND | GND |
| 2 | WL_BT_ANT | `WL_BT_2_4G`, đến mạng RF |
| 9 | VBAT | `VDD3V3_SYS` |
| 12 | WL_PWR | `WF_PWR_EN`; R53 = 33 kohm kéo lên VDD3V3_SYS; liên quan R60 ở trang 4 |
| 14 | SDIO_DATA_2 | `SDIO1_D2`, U1.52, J22; R47 = 33 kohm kéo lên 3,3 V |
| 15 | SDIO_DATA_3 | `SDIO1_D3`, U1.51, J21; R48 = 33 kohm kéo lên 3,3 V |
| 16 | SDIO_DATA_CMD | `SDIO1_CMD`, U1.55, J24; R49 = 33 kohm kéo lên 3,3 V |
| 17 | SDIO_DATA_CLK | `SDIO1_CLK`, U1.56, J25; vị trí pull-up R50 ghi NC |
| 18 | SDIO_DATA_0 | `SDIO1_D0`, U1.54, J23; R51 = 33 kohm kéo lên 3,3 V |
| 19 | SDIO_DATA_1 | `SDIO1_D1`, U1.53, J26; R52 = 33 kohm kéo lên 3,3 V |
| 22 | VDDIO | `VDD3V3_SYS` |
| 24 | HOST_WAKE_WF | `HOST_WAKE_WF`; R54 = 33 kohm lên VDD3V3_SYS; ghi chú `Fn-Link N240 R54 = 33K` |
| 41 | UART1_RTS | Net được đặt tên `BT_RTX`, giữ đúng cách viết của PDF |
| 42 | UART1_TX | `BT_TXD` |
| 43 | UART1_RX | `BT_RXD` |
| 44 | UART1_CTS | `BT_CTS` |

Các chân còn lại có dấu không nối: U13.4 NC, .6 HOST_WAKE_BT, .7 BT_WAKE_HOST, .8 NC, .10 USB DM, .11 USB DP, .13 WL_WAKE_HOST, .21 NC, .23 GPIO_B4, .25 PCM_DOUT, .26 PCM_CLK, .27 PCM_DIN, .28 PCM_SYNC, .29 UART0 TX, .30 UART0 RX, .32 NC, .34 NC, .35 GPIOB6, .37/.38/.39/.40 NC. Đặc biệt, USB của U13 không nối với USB-C trong thiết kế này.

### 6.2. RN1 và đường UART Bluetooth

RN1 gồm bốn nhánh, nhưng giá trị chung ghi `NC`.

| Phía U13 | Qua RN1 đến phía U1 |
| --- | --- |
| `BT_CTS`, U13.44 | `GPIOA_19`, U1.26 |
| `BT_RTX`, U13.41 | `GPIOA_18`, U1.27 |
| `BT_TXD`, U13.42 | `GPIOA_29`, U1.29 |
| `BT_RXD`, U13.43 | `GPIOA_28`, U1.28 |

Do RN1 ghi NC, chỉ có thể khẳng định schematic dành vị trí cho các đường này; không khẳng định Bluetooth UART đã được nối trên cấu hình không lắp RN1. Các chức năng UART/RTS/CTS chính xác còn phụ thuộc pinmux U1.

### 6.3. GPIOA_26 và tùy chọn Fn-Link N240

Trang 4 vẽ `WF_PWR_EN` qua R60 = 0 ohm đến `GPIOA_26`, đồng thời `HOST_WAKE_WF` qua R58 = 0 ohm đến chính net `GPIOA_26`. Có ghi chú `Fn-Link N240 R58 = 0R`.

Không được tự bỏ một nhánh khỏi bản mô tả: PDF hiển thị cả hai giá trị là 0 ohm. Nếu cả R60 và R58 cùng được lắp, ba net này sẽ được nối với nhau qua hai điện trở; đây là hệ quả của đường nối. PDF không giải thích đầy đủ bảng lắp theo từng module, nên cần BOM/bo thực tế để kết luận nhánh nào có mặt. J12 cũng ở cùng net GPIOA_26.

### 6.4. RF 2,4 GHz

RF1 là đầu nối RF được ghi `2.4G`. Chân 1 RF1 đi qua C68 đến net `WL_BT_2_4G` tại U13.2. C69 nối từ phía RF1 của C68 xuống GND; C70 nối từ phía U13 của C68 xuống GND. RF1.2/.3/.4 nối GND.

Giá trị hiển thị là C68 = `Pi0`, C69 = `Pi2`, C70 = `Pi1`. Đây là nhãn cho mạng matching được ghi trong nguồn, không phải các giá trị điện dung có thể quy đổi ra pF. Không tự điền giá trị RF còn thiếu.

## 7. USB Type-C

Nguồn: trang 4. USB1 là đầu nối Type-C theo symbol; PDF đặt tên chân chức năng, không cung cấp đầy đủ định danh tiếp điểm A1/B1... của một receptacle tiêu chuẩn.

| Chân chức năng USB1 | Kết nối trong thiết kế |
| --- | --- |
| Hai VBUS | Net `VBUS`; C25/C26 = 10 uF xuống GND; R65 = 0 ohm sang VSYS |
| Hai D+ | Cùng net `USB_D_P`, đến U1.69 USB_DP |
| Hai D- | Cùng net `USB_D_N`, đến U1.70 USB_DM |
| CC1 | Qua R35 = 5,1 kohm xuống GND |
| CC2 | Qua R34 = 5,1 kohm xuống GND |
| SBU1 | `GPIOA_16`, U1.18, J2; hình pinout gán UART0 TX |
| SBU2 | `GPIOA_17`, U1.19, J1; hình pinout gán UART0 RX |
| EP và hai GND | GND |

Hai SBU là dây đưa GPIO/UART ra đầu nối, không phải IC chuyển đổi USB-UART. PDF không vẽ một USB-UART bridge. Hai cặp D+/D- trên symbol được nối chung theo cực để phục vụ hai hướng cắm; không phải hai cổng USB độc lập.

`USB_DET` tại U1.60 là net riêng có cầu strap ở trang 2; không được đồng nhất với `VBUS`. Sơ đồ ghi USB_DET cao chọn Device và thấp chọn Host, nhưng chỉ đổi strap không chứng minh bo đã có đủ mạch cấp VBUS hoặc cấu hình phần mềm cho mọi chế độ Host.

## 8. Lưu trữ SDIO0: U9 và U10

Nguồn: trang 4, khối `SD NAND (PU by SOC)`. Dòng `PU by SOC` cho biết pull-up được dự kiến từ SoC. Không thấy các điện trở pull-up rời cho SDIO0 trong khối này.

### 8.1. U9 - symbol SD NAND

| Chân U9 | Tên chân | Net | Chân U1 |
| --- | --- | --- | --- |
| 1 | SDD2 | `SDIO0_D2` | 11 |
| 2 | SDD3 | `SDIO0_D3` | 12 |
| 3 | SCLK | `SDIO0_CLK` | 6 |
| 4 | VSS | GND | - |
| 5 | CMD | `SDIO0_CMD` | 7 |
| 6 | SDD0 | `SDIO0_D0` | 8 |
| 7 | SDD1 | `SDIO0_D1` | 10 |
| 8 | VCC | `VDD3V3_SYS` | - |

C54 nằm giữa nguồn 3,3 V và GND nhưng giá trị ghi `NC`. PDF không ghi mã chip hoặc dung lượng U9.

### 8.2. U10 - ổ thẻ SD theo symbol

| Chân U10 | Tên chân | Net |
| --- | --- | --- |
| 1 | DAT2 | `SDIO0_D2` |
| 2 | CD/DAT3 | `SDIO0_D3` |
| 3 | CMD | `SDIO0_CMD` |
| 4 | VDD | `VDD3V3_SYS` |
| 5 | CLK | `SDIO0_CLK` |
| 6 | VSS | GND |
| 7 | DAT0 | `SDIO0_D0` |
| 8 | DAT1 | `SDIO0_D1` |
| 9 | SWITCH | Không nối |
| 10, 11, 12, 13 | Các chân phụ/vỏ theo symbol | GND |

C53 = 100 nF từ nguồn 3,3 V xuống GND. U1.14 mang chức năng `SDIO0_CD/XGPIOA[13]` nhưng được nối GND trên trang 2; nó không nối tới SWITCH của U10 trong PDF.

U9 và U10 dùng chung các net SDIO0, không có bus switch hoặc chip-select riêng được vẽ giữa chúng. PDF không đưa đủ thông tin để kết luận cả hai được lắp và sử dụng đồng thời trên mọi biến thể.

## 9. DSI, CSI, touch và cấu hình camera

### 9.1. FPC2 - DSI/màn hình

Nguồn: trang 4. FPC2 có 31 chân tín hiệu được đánh số 1-31, cùng hai chân 32/33 nối GND. Chuỗi cạnh symbol là `1002.1163`; PDF không giải thích thêm về mã đầu nối.

| Chân FPC2 | Net | Liên hệ |
| --- | --- | --- |
| 1, 2, 3 | `LCD0_BL+` | Đầu ra dương mạch backlight |
| 4 | NC | Không nối |
| 5, 6, 7, 8 | `LCD0_BL-` | Đường hồi về sense của mạch backlight, không phải nối thẳng GND |
| 9, 10 | GND | - |
| 11, 12 | NC | Không nối |
| 13 | GND | - |
| 14 | `MIPI_TX2_P` | U1.84 |
| 15 | `MIPI_TX2_N` | U1.83 |
| 16 | GND | - |
| 17 | `MIPI_TX1_P` | U1.86 |
| 18 | `MIPI_TX1_N` | U1.85 |
| 19 | GND | - |
| 20 | `MIPI_TX0_P` | U1.88 |
| 21 | `MIPI_TX0_N` | U1.87 |
| 22 | GND | - |
| 23, 24 | NC | Không nối |
| 25 | GND | - |
| 26 | `VDD1V8_SYS` | 1,8 V |
| 27 | `LCD0_RESET` | U1.47 |
| 28 | GND | - |
| 29 | `VDD1V8_SYS` | 1,8 V |
| 30, 31 | `VDD3V3_SYS` | 3,3 V |
| 32, 33 | GND | Chân phụ/vỏ theo symbol |

PDF không gán tường minh cặp TX nào là clock và cặp nào là data trong một panel cụ thể. Chỉ giữ đúng ba cặp TX0/TX1/TX2; không tự thêm một cặp clock riêng ngoài các net đã vẽ.

### 9.2. FPC3 - CSI/camera

| Chân FPC3 | Net | Chân U1 hoặc liên hệ |
| --- | --- | --- |
| 1 | GND | - |
| 2 | `MIPI_RX4_N` | U1.72 |
| 3 | `MIPI_RX4_P` | U1.73 |
| 4 | GND | - |
| 5 | `MIPI_RX3_N` | U1.74 |
| 6 | `MIPI_RX3_P` | U1.75 |
| 7 | GND | - |
| 8 | `MIPI_RX2_N` | U1.76 |
| 9 | `MIPI_RX2_P` | U1.77 |
| 10 | GND | - |
| 11 | `MIPI_RX1_N` | U1.78 |
| 12 | `MIPI_RX1_P` | U1.79 |
| 13 | GND | - |
| 14 | `MIPI_RX0_N` | U1.80; đồng thời qua R72 đến CAM_IO1 |
| 15 | `MIPI_RX0_P` | U1.81 |
| 16 | GND | - |
| 17 | `CAM_IO0` | U1.48; ghi 1,8 V |
| 18 | `CAM_IO1` | Qua R72 = 0 ohm đến MIPI_RX0_N/U1.80; ghi 1,8 V |
| 19 | GND | - |
| 20 | `CAM_SCL` | U1.43; ghi 1,8 V; chung với FPC1.2 |
| 21 | `CAM_SDA` | U1.44; ghi 1,8 V; chung với FPC1.1 |
| 22 | `VDD3V3_SYS` | Nguồn camera 3,3 V |
| 23, 24 | GND | Chân phụ/vỏ theo symbol |

R72 = 0 ohm nối `MIPI_RX0_N` với `CAM_IO1`. Với cấu hình GC4653 được mô tả trong PDF, net này được dùng làm MCLK. Do đó CAM_IO1 không phải một chân GPIO độc lập bổ sung, và không được đồng thời giả định RX0_N vẫn là tín hiệu CSI độc lập trong cấu hình dùng R72 cho MCLK.

### 9.3. FPC1 - Touch

| Chân FPC1 | Net | Liên hệ |
| --- | --- | --- |
| 1 | `CAM_SDA` | U1.44; R31 = 5,1 kohm kéo lên VDD1V8_SYS |
| 2 | `CAM_SCL` | U1.43; R32 = 5,1 kohm kéo lên VDD1V8_SYS |
| 3 | `TP0_RST` | U1.41 |
| 4 | `TP0_INT` | U1.40 |
| 5 | `VDD3V3_SYS` | Nguồn 3,3 V |
| 6 | GND | - |
| 7, 8 | GND | Chân phụ/vỏ theo symbol |

Camera và touch dùng chung hai net I2C CAM_SCL/CAM_SDA. Pull-up của hai net nằm trong khối Touch nhưng áp dụng cho cả net trên toàn sơ đồ. Nguồn FPC là 3,3 V không có nghĩa mức logic I2C là 3,3 V: điện trở đang kéo lên 1,8 V. PDF không cung cấp địa chỉ I2C hoặc model touch controller.

### 9.4. Bảng vai trò CAM_IO0/CAM_IO1

Chép theo bảng `Camera configuration description` ở trang 4.

| Camera/cáp | CAM_IO0 | CAM_IO1 |
| --- | --- | --- |
| Sipeed GC4653 + 70416 FPC | RESET | MCLK; nối MIPI 0N qua R72 theo cấu hình mặc định được chú thích |
| Sipeed OS04A10 + cáp FPC cùng chiều | RESET | NC |
| Raspberry Pi camera | Power EN | LED Indicator |

Hàng Raspberry Pi có ghi `level matching required`, tức cần phù hợp/chuyển mức điện áp. Không suy ra khả năng cắm trực tiếp chỉ vì hình dạng đầu nối tương tự.

### 9.5. Mapping camera đúng cho bản 70415

Bảng này lấy từ phía phải của phần so sánh cuối trang 4, tiêu đề `70415 Camera Interface`. Ô trống của bản gốc được ghi là `Không ghi`.

| Đường trên LicheeRV Nano 70415 | Sipeed GC4653 | Sipeed OS04A10 | RPi camera 2 lane |
| --- | --- | --- | --- |
| MIPI RX4 | CSI CLK | CSI D3 | CSI D0 |
| MIPI RX3 | CSI D0 | CSI D1 | CSI D1 |
| MIPI RX2 | CSI D1 | CSI CLK | CSI CLK |
| MIPI RX1 | Không ghi | CSI D0 | Không ghi |
| MIPI RX0 | Không ghi | CSI D2 | Không ghi |
| IO0 | RST | RST | Power EN |
| IO1, ghi `Connect R0N` | MCLK | NC | LED |

Các nhãn RX0..RX4 là tên đường trên bo, không phải cố định số lane data của mọi camera. Ví dụ cùng RX4 là clock với GC4653 nhưng data D3 với OS04A10.

### 9.6. Bảng 70405 có trong PDF để so sánh lịch sử

Bảng phía trái mang tiêu đề `70405 Camera Interface`, không phải mapping mặc định của bo 70415.

| Đường trên LicheeRV Nano 70405 | Sipeed GC4653 | Sipeed OS04A10 | RPi camera 2 lane |
| --- | --- | --- | --- |
| MIPI RX4 | Không ghi | CSI D3 | CSI D0 |
| MIPI RX3 | Không ghi | CSI D1 | CSI D1 |
| MIPI RX2 | CSI CLK | CSI CLK | CSI CLK |
| MIPI RX1 | CSI D0 | CSI D0 | Không ghi |
| MIPI RX0 | CSI D1 | CSI D2 | Không ghi |
| IO0, ghi `Connect R4N` | MCLK | RST | Power EN |
| IO1 | RST | NC | LED |

Khác biệt đáng chú ý với GC4653: bản 70415 chuyển CLK sang RX4, D0 sang RX3, D1 sang RX2; IO0 làm reset, IO1 làm MCLK. Không trộn bảng 70405 vào cấu hình 70415.

## 10. Mạch backlight

Nguồn: trang 3, khối `Backlight LED Driver`. U14 được ghi `AW9962EDNR`.

| Thành phần | Kết nối/giá trị |
| --- | --- |
| U14.6 VIN | `VSYS`; C35 = 2,2 uF xuống GND |
| U14.5 CTRL | `BL_PWM` từ U1.49; R11 = 10 kohm kéo xuống GND |
| U14.4 SW | Nút chuyển mạch giữa L4 và anode D1 |
| L4 | 10 uH, từ VSYS đến nút SW |
| D1 | Anode ở nút SW, cathode ở `LCD0_BL+`; PDF không ghi mã diode |
| C40 | 10 uF/25 V, từ `LCD0_BL+` xuống GND |
| U14.1 FB | `LCD0_BL-` |
| R19 | 10 ohm, từ LCD0_BL- xuống GND |
| R18 | 10 ohm được vẽ song song R19; nằm trong khung ghi `BL` |
| U14.3 GND và EP | GND |
| U14.2 NC | Không nối |

Đây là topology nâng áp cấp backlight. Dòng đi về qua LCD0_BL- được cảm nhận trên mạng điện trở R19/R18. Điện áp LCD0_BL+ không được ghi cố định trong PDF; trị số 25 V là điện áp định mức in cho C40, không phải xác nhận đầu ra bằng 25 V.

Ghi chú gốc: `R = 200mV/20mA/N = 10Ω/N`.

| Cột `Backlight LED (N)` trong nguồn | Cột `BL (R18)` trong nguồn |
| --- | --- |
| 1 Series | NC |
| 2 Series | 10 ohm |
| 4 Series | 3,3 ohm |

Giữ nguyên chữ `Series` của bảng gốc; PDF không giải thích đầy đủ cấu trúc chuỗi LED của panel. Suy luận từ công thức: chỉ R19 = 10 ohm thì mức tham chiếu 200 mV tương ứng khoảng 20 mA; nếu lắp thêm R18 = 10 ohm song song thì điện trở tương đương 5 ohm, tương ứng khoảng 40 mA tổng. Đây không phải thông số dòng được kiểm chứng cho một màn hình cụ thể.

## 11. Audio: khuếch đại và microphone

### 11.1. U11 - PA

Nguồn: trang 4, khối `PA`. PDF không ghi model U11 và không in số chân vật lý; chỉ dùng được tên chân của symbol.

| Chân/tín hiệu U11 | Kết nối |
| --- | --- |
| AVDD, PVDD | Cùng nối `VSYS`; C58 = 2,2 uF xuống GND |
| EN | Qua R36 = 10 kohm lên `VDD3V3_SYS` |
| INP | `AUD_OUT` từ U1.4 -> C55 = 100 nF -> R37 = 33 kohm -> INP |
| INN | GND -> C56 = 100 nF -> R38 = 33 kohm -> INN |
| VOP | Net `VO_P`, ra J5 |
| VON | Net `VO_N`, ra J6 |
| Hai chân GND | GND |

Hai đầu VO_P/VO_N đều là đầu ra của U11; VO_N không được nối GND trong sơ đồ. Kết nối gợi ý ngõ ra audio dạng cầu/vi sai, nhưng không đủ dữ liệu để khẳng định class khuếch đại, công suất, trở kháng loa hoặc gán số chân vật lý cho U11.

### 11.2. U12 - MIC

| Chân U12 | Tên chân | Kết nối |
| --- | --- | --- |
| 1 | OUT | Qua C57 = 2,2 uF đến `AUD_IN`, U1.2 |
| 2 | GND | GND |
| 3 | GND | GND |
| 4 | VDD | `VDD3V3_SYS`; C59 = 100 nF xuống GND |

PDF không ghi model microphone. Đường audio U1 còn có nguồn analog 1,8 V ở U1.5, mass analog ở U1.1 và net tham chiếu `ADU_VREF` ở U1.3.

## 12. Ethernet 10/100M

Nguồn: trang 2 và khối `10/100M PHY` trang 4. U1 nối trực tiếp ra bốn net EPHY; các phần tử rời nằm giữa những net này và RJ1. Không tự chèn một IC PHY ngoài hoặc một biến áp theo sơ đồ Ethernet thông dụng khác.

### 12.1. Bất nhất giữa tên chân trong symbol và tên net

| Chân U1 | Tên chức năng Ethernet in trong symbol | Net bên ngoài symbol |
| --- | --- | --- |
| 65 | `ETH_RXM)` | `EPHY_TX_P` |
| 64 | `ETH_RXP` | `EPHY_TX_N` |
| 63 | `ETH_TXM` | `EPHY_RX_P` |
| 62 | `ETH_TXP` | `EPHY_RX_N` |

Bảng giữ nguyên cả tên chân và tên net dù cách ghi TX/RX/P/N không tương ứng trực giác. Đây là điểm cần đối chiếu datasheet/CAD nếu sửa thiết kế; không được tự đổi net trong lúc đọc tài liệu.

### 12.2. Đường tín hiệu ra RJ1

| Từ U1/net | Linh kiện nối tiếp | Net phía RJ1 | Chân RJ1 |
| --- | --- | --- | --- |
| U1.65, `EPHY_TX_P` | C50 = 100 nF, rồi L5 từ chân 4 sang chân 3 | `EPHY_CON_T_P` | 1 |
| U1.64, `EPHY_TX_N` | C49 = 100 nF, rồi L5 từ chân 1 sang chân 2 | `EPHY_CON_T_N` | 2 |
| U1.63, `EPHY_RX_P` | C52 = 100 nF, rồi L6 từ chân 1 sang chân 2 | `EPHY_CON_R_P` | 3 |
| U1.62, `EPHY_RX_N` | C51 = 100 nF, rồi L6 từ chân 4 sang chân 3 | `EPHY_CON_R_N` | 6 |

RJ1.4/.5/.7/.8 không nối; RJ1.9/.10 nối GND. PDF không ghi model RJ1, L5 hoặc L6. L5/L6 được vẽ bằng symbol cuộn dây ghép bốn chân; chỉ từ symbol không xác nhận được mã linh kiện, đặc tính trở kháng hoặc tiêu chuẩn cách ly.

### 12.3. Các nhánh U7/U8 đúng như sơ đồ

U7 và U8 cũng được vẽ bằng symbol hai cuộn ghép bốn chân, không có mã/giá trị. Các chân nối như sau; không thay bằng giả định về một magjack thông thường.

| Linh kiện | Chân 1 | Chân 2 | Chân 3 | Chân 4 |
| --- | --- | --- | --- | --- |
| U7 | `EPHY_CON_T_P` | GND | `EPHY_CON_T_N` | GND |
| U8 | GND | `EPHY_CON_R_P` | GND | `EPHY_CON_R_N` |

Không suy ra khả năng PoE, Gigabit hoặc một mức cách ly an toàn từ PDF này. Tiêu đề của khối chỉ ghi 10/100M.

## 13. ADC và các điện trở kéo ở pad

Nguồn: trang 4, khối `Pin`.

`ADC_PIN` tại J19 đi qua R6 = 10 kohm đến net `ADC1` tại U1.59. R10 = 5,1 kohm nối ADC1 xuống GND. Vì vậy J19 không nối trực tiếp chân ADC của U1.

Phép tính lý tưởng: `V_ADC1 = V_ADC_PIN x 5,1 / (10 + 5,1) ≈ 0,33775 x V_ADC_PIN`. Ví dụ nếu J19 có 3,3 V thì ADC1 khoảng 1,115 V khi bỏ qua tải. Đây chỉ là tỷ lệ cầu chia; không dùng ví dụ này để kết luận điện áp đầu vào tối đa hoặc khả năng chịu áp của chân ADC.

| Điện trở | Giá trị | Đường nối |
| --- | --- | --- |
| R69 | 10 kohm | VDD3V3_SYS đến GPIOA_29 |
| R68 | 10 kohm | VDD3V3_SYS đến GPIOA_28 |
| R63 | 10 kohm | VDD3V3_SYS đến GPIOA_19 |
| R62 | 10 kohm | VDD3V3_SYS đến GPIOA_18 |
| R40 | 10 kohm | VDD3V3_SYS đến GPIOA_17 |
| R39 | 10 kohm | VDD3V3_SYS đến GPIOA_16 |

## 14. Bảng chân đầy đủ của U1

Nguồn: symbol U1 ở trang 2, sheet `GC2002-QFN88`. Có 88 chân đánh số cùng EP số 89. Cột chức năng dưới đây giữ chuỗi văn bản in trong PDF, kể cả một số chỗ lặp hoặc dấu ngoặc bất thường. Cột net được đối chiếu với nhãn/đường nối bên ngoài symbol.

Không dùng thứ tự các chức năng trong một ô để suy ra chức năng mặc định. `PWR_GPIO[n]` trong symbol tương ứng cách viết `GPIOP n` trên một số nhãn pinout trang 1; điều này không định nghĩa số GPIO toàn cục trong phần mềm.

| Chân U1 | Net ngoài symbol | Tên chân và toàn bộ chức năng in trong symbol |
| --- | --- | --- |
| 1 | `GND` | `VSS18A_AUD` |
| 2 | `AUD_IN` | `AUD_AINL_MIC/XGPIOC[23]/IIS1_BCLK/IIS2_BCLK` |
| 3 | `ADU_VREF` | `PAD_AUD_AVREF` |
| 4 | `AUD_OUT` | `AUD_AOUTR/XGPIOC[24]/IIS1_DI/IIS2_DO/IIS1_DO` |
| 5 | `VDD1V8_SYS` | `VDD18A_AUD` |
| 6 | `SDIO0_CLK` | `SDIO0_CLK/IIC1_SDA/SPI0_SCK/XGPIOA[7]/PWM[15]/EPHY_LNK_LED/DBG[0]` |
| 7 | `SDIO0_CMD` | `SDIO0_CMD/IIC1_SCL/SPI0_SDO/XGPIOA[8]/PWM[14]/EPHY_SPD_LED/DBG[1]` |
| 8 | `SDIO0_D0` | `SDIO0_D[0]/CAM_MCLK1/SPI0_SDI/XGPIOA[9]/UART3_TX/PWM[13]/WG0_D0/DBG[2]` |
| 9 | `VDD0V9_CPU` | `VDDC` |
| 10 | `SDIO0_D1` | `SDIO0_D[1]/IIC1_SDA/AUX0/XGPIOA[10]/UART1_TX/PWM[12]/WG0_D1/DBG[3]` |
| 11 | `SDIO0_D2` | `SDIO0_D[2]/IIC1_SCL/AUX1/XGPIOA[11]/UART1_RX/PWM[11]/WG1_D0/DBG[4]` |
| 12 | `SDIO0_D3` | `SDIO0_D[3]/CAM_MCLK0/SPI0_CS_X/XGPIOA[12]/UART3_RX/PWM[10]/WG1_D1/DBG[5]` |
| 13 | `VDD3V3_SYS` | `VDDIO_SD0_SPI` |
| 14 | `GND` | `SDIO0_CD/XGPIOA[13]` |
| 15 | `GPIOA_14` | `SDIO0_PWR_EN/XGPIOA[14]` |
| 16 | `VDD0V9_CPU` | `VDDC` |
| 17 | `GPIOA_15` | `XGPIOA[15]` |
| 18 | `GPIOA_16` | `UART0_TX/CAM_MCLK1/PWM[4]/XGPIOA[16]/UART1_TX/AUX1/DBG[6]/UART0_TX` |
| 19 | `GPIOA_17` | `UART0_RX/CAM_MCLK0/PWM[5]/XGPIOA[17]/UART1_RX/AUX0/DBG[7]/UART0_RX` |
| 20 | `GPIOA_26` | `EMMC_DAT[2]/SPINOR_HOLD_X/SPINAND_HOLD/XGPIOA[26]/SPINOR_HOLD_X` |
| 21 | `GPIOA_22` | `EMMC_CLK/SPINOR_SCK/SPINAND_CLK/XGPIOA[22]/SPINOR_SCK` |
| 22 | `GPIOA_25` | `EMMC_DAT[0]/SPINOR_MOSI/SPINAND_MOSI/XGPIOA[25]/SPINOR_MOSI` |
| 23 | `GPIOA_27` | `EMMC_DAT[3]/SPINOR_WP_X/SPINAND_WP/XGPIOA[27]/SPINOR_WP_X` |
| 24 | `GPIOA_23` | `EMMC_CMD/SPINOR_MISO/SPINAND_MISO/XGPIOA[23]/SPINOR_MISO` |
| 25 | `GPIOA_24` | `EMMC_DAT[1]/SPINOR_CS_X/SPINAND_CS/XGPIOA[24]/SPINOR_CS_X` |
| 26 | `GPIOA_19` | `CR_4WTMS/CAM_MCLK0/PWM[7]/XGPIOA[19]/UART1_RTS/AUX0/UART1_TX/VO_D[28]/JTAG_CPU_TMS` |
| 27 | `GPIOA_18` | `CR_4WTCK/CAM_MCLK1/PWM[6]/XGPIOA[18]/UART1_CTS/AUX1/UART1_RX/VO_D[29]/JTAG_CPU_TCK` |
| 28 | `GPIOA_28` | `CR_4WTDI/UART1_TX/UART2_TX/XGPIOA[28]/WG0_D0/DBG[10]/IIC0_SCL` |
| 29 | `GPIOA_29` | `CR_4WTDO/UART1_RX/UART2_RX/XGPIOA[29]/WG0_D1/WG1_D0/DBG[11]/IIC0_SDA` |
| 30 | `BOOT_KEY` | `AUX0/XGPIOA[30]/IIS1_MCLK/VO_D[31]/WG1_D1/DBG[12]` |
| 31 | `VDD1V35_DRAM` | `VDDQ_DRAM` |
| 32 | `VDD1V35_DRAM` | `VDDQ` |
| 33 | `VDD1V35_DRAM` | `VDDQ` |
| 34 | `VDD1V35_DRAM` | `VDDQ_DRAM` |
| 35 | `GPIO_ZQ` | `GPIO_ZQ)/PWR_GPIO[24]/PWM[2]/GPIO_ZQ` |
| 36 | `VDD1V8_SYS` | `VDDIO_RTC` |
| 37 | `VDDC_RTC` | `VDDC_RTC` |
| 38 | `PWR_DET` | `PWR_VBAT_DET(PWRSAR.VIN3)` |
| 39 | `SYS_RSTN` | `PWR_RSTN` |
| 40 | `TP0_INT` | `PWR_SEQ1/PWR_GPIO[3]/PWR_SEQ1` |
| 41 | `TP0_RST` | `PWR_SEQ2/PWR_GPIO[4]/PWR_SEQ2` |
| 42 | `GND` | `PTEST` |
| 43 | `CAM_SCL` | `PWR_WAKEUP0/PWR_IR0/PWR_UART0_TX/PWR_GPIO[6](PWR_IIC_SDA)/UART1_TX/IIC4_SCL/EPHY_LNK_LED/WG2_D0/PWR_WAKEUP0` |
| 44 | `CAM_SDA` | `PWR_BUTTON1/PWR_GPIO[8](PWR_IIC_SCL)/UART1_RX/IIC4_SDA/EPHY_SPD_LED/WG2_D1/PWR_BUTTON1` |
| 45 | `XTAL_XIN` | `XTAL_XIN` |
| 46 | `XTAL_XOUT` | `XTAL_XOUT` |
| 47 | `LCD0_RESET` | `PWR_GPIO[0]/UART2_TX/PWR_UART0_RX/PWM[8]/PWR_GPIO0` |
| 48 | `CAM_IO0` | `PWR_GPIO[1](PWRSAR1.VIN2)/UART2_RX/EPHY_LNK_LED/PWM[9]/PWR_IIC_SCL/IIC2_SCL/CR_SDA0/PWR_GPIO1` |
| 49 | `BL_PWM` | `PWR_GPIO[2](PWRSAR.VIN1)/EPHY_SPD_LED/PWM[10]/PWR_IIC_SDA/IIC2_SDA/CR_2WTCK/PWR_GPIO2` |
| 50 | `VDD0V9_CPU` | `VDDC` |
| 51 | `SDIO1_D3` | `PWR_SD1_D3_VO32/SPI2_CS_X/IIC1_SCL/PWR_GPIO[18]/CAM_MCLK0/UART3_CTS/PWR_SPINOR1_CS_X/PWM[4]/SD1_D3` |
| 52 | `SDIO1_D2` | `PWR_SD1_D2_VO33/IIC1_SCL/UART2_TX/PWR_GPIO[19]/CAM_MCLK0/UART3_TX/PWR_SPINOR1_HOLD_X/PWM[5]/SD1_D2` |
| 53 | `SDIO1_D1` | `PWR_SD1_D1_VO34/IIC1_SDA/UART2_RX/PWR_GPIO[20]/CAM_MCLK1/UART3_RX/PWR_SPINOR1_WP_X/PWM[6]/SD1_D1` |
| 54 | `SDIO1_D0` | `PWR_SD1_D0_VO35/SPI2_SDI/IIC1_SDA/PWR_GPIO[21]/CAM_MCLK1/UART3_RTS/PWR_SPINOR1_MISO/PWM[7]/SD1_D0` |
| 55 | `SDIO1_CMD` | `PWR_SD1_CMD_VO36/SPI2_SDO/IIC3_SCL/PWR_GPIO[22]/CAM_VS0/EPHY_LNK_LED/PWR_SPINOR1_MOSI/PWM[8]/SD1_CMD` |
| 56 | `SDIO1_CLK` | `PWR_SD1_CLK_VO37/SPI2_SCK/IIC3_SDA/PWR_GPIO[23]/CAM_HS0/EPHY_SPD_LED/PWR_SPINOR1_SCK/PWM[9]/SD1_CLK` |
| 57 | `VDD3V3_SYS` | `VDDIO_SD1` |
| 58 | `PWR_PWM` | `PWM[0]/XGPIOB[0]` |
| 59 | `ADC1` | `ADC[1]/XGPIOB[3]/KEY_COL2` |
| 60 | `USB_DET` | `USB_VBUS_DET/XGPIOB[6]/CAM_MCLK0/CAM_MCLK1` |
| 61 | `VDD0V9_CPU` | `VDDC` |
| 62 | `EPHY_RX_N` | `ETH_TXP/UART3_RX/IIC1_SCL/XGPIOB[25]/PWM[13]/CAM_MCLK0/SPI1_SDO/IIS2_LRCK` |
| 63 | `EPHY_RX_P` | `ETH_TXM/UART3_RTS/IIC1_SDA/XGPIOB[24]/PWM[12]/CAM_MCLK1/SPI1_SDI/IIS2_BCLK` |
| 64 | `EPHY_TX_N` | `ETH_RXP/UART3_TX/CAM_MCLK1/XGPIOB[27]/PWM[15]/CAM_HS0/SPI1_SCK/IIS2_DO` |
| 65 | `EPHY_TX_P` | `ETH_RXM)/UART3_CTS/CAM_MCLK0/XGPIOB[26]/PWM[14]/CAM_VS0/SPI1_CS_X/IIS2_DI` |
| 66 | `VDD3V3_SYS` | `VDD33A_ETH_USB` |
| 67 | `CPU_SEL` | `GPIO_RTX/XGPIOB[23]/PWM[1]/CAM_MCLK0` |
| 68 | `VDD1V8_SYS` | `VDD18A_USB_PLL_ETH` |
| 69 | `USB_D_P` | `USB_DP` |
| 70 | `USB_D_N` | `USB_DM` |
| 71 | `VDD0V9_CPU` | `VDDC` |
| 72 | `MIPI_RX4_N` | `CR_SCL0/VI0_CLK/VI1_D[13]/XGPIOC[2]/IIC1_SDA/CAM_MCLK0/KEY_ROW0/MUX_SPI1_SCK/MIPIRX4N` |
| 73 | `MIPI_RX4_P` | `CR_SDA0/VI0_D[0]/VI1_D[14]/XGPIOC[3]/IIC1_SCL/CAM_MCLK1/KEY_ROW1/MUX_SPI1_CS/MIPIRX4P` |
| 74 | `MIPI_RX3_N` | `CR_2WTMS/VI0_D[1]/VI1_D[15]/XGPIOC[4]/CAM_MCLK0/MUX_SPI1_MISO/MIPIRX3N` |
| 75 | `MIPI_RX3_P` | `CR_2WTCK/VI0_D[2]/VI1_D[16]/XGPIOC[5]/MUX_SPI1_MOSI/MIPIRX3P` |
| 76 | `MIPI_RX2_N` | `VI0_D[3]/VO_D[10]/XGPIOC[6]/VI1_D[17]/IIC4_SCL/DBG[6]/MIPIRX2N` |
| 77 | `MIPI_RX2_P` | `VI0_D[4]/VO_D[9]/XGPIOC[7]/VI1_D[18]/IIC4_SDA/DBG[7]/MIPIRX2P` |
| 78 | `MIPI_RX1_N` | `VI0_D[5]/VO_D[8]/XGPIOC[8]/KEY_ROW3/DBG[8]/MIPIRX1N` |
| 79 | `MIPI_RX1_P` | `VI0_D[6]/VO_D[7]/XGPIOC[9]/IIC1_SDA/KEY_ROW2/DBG[9]/MIPIRX1P` |
| 80 | `MIPI_RX0_N` | `VI0_D[7]/VO_D[6]/XGPIOC[10]/IIC1_SCL/CAM_MCLK1/DBG[10]/MIPIRX0N` |
| 81 | `MIPI_RX0_P` | `VI0_D[8]/VO_D[5]/XGPIOC[11]/CAM_MCLK0/DBG[11]/MIPIRX0P` |
| 82 | `VDD1V8_SYS` | `VDD18A_MIPI` |
| 83 | `MIPI_TX2_N` | `CR_SDA0/VI0_D[13]/VO_D[0]/XGPIOC[16]/IIC1_SDA/PWM[8]/SPI0_SCK/SD1_D2/MIPI_TXM2` |
| 84 | `MIPI_TX2_P` | `CR_SCL0/VI0_D[14]/VO_CLK0/XGPIOC[17]/IIC1_SCL/PWM[9]/SPI0_CS_X/SD1_D3/MIPI_TXP2` |
| 85 | `MIPI_TX1_N` | `CR_2WTMS/VI0_D[11]/VO_D[2]/XGPIOC[14]/IIC2_SDA/PWM[10]/SPI0_SDO/DBG[14]/MIPI_TXM1` |
| 86 | `MIPI_TX1_P` | `CR_2WTCK/VI0_D[12]/VO_D[1]/XGPIOC[15]/IIC2_SCL/PWM[11]/SPI0_SDI/DBG[15]/MIPI_TXP1` |
| 87 | `MIPI_TX0_N` | `VI0_D[9]/VO_D[4]/XGPIOC[12]/CAM_MCLK1/PWM[14]/CAM_VS0/DBG[12]/PAD_MIPI_TXM0` |
| 88 | `MIPI_TX0_P` | `VI0_D[10]/VO_D[3]/XGPIOC[13]/CAM_MCLK0/PWM[15]/CAM_HS0/DBG[13]/MIPI_TXP0` |
| 89 | `GND` | `EP` |

## 15. Những quan hệ dùng chung và giới hạn dữ liệu cần giữ khi phân tích

SDIO1 tại J21-J26 là chính bus nối U13. Cấu hình các pad này sang GPIO/SPI/UART khác sẽ tác động cùng các dây đang đi tới Wi-Fi; PDF không có bộ cách ly bus. Tương tự, U9 và U10 cùng dùng SDIO0.

GPIOA_16/GPIOA_17 vừa ra J2/J1 vừa nối SBU1/SBU2 của USB1. GPIOA_14 vừa ra J27 vừa nối LED1. GPIOA_26 vừa ra J12 vừa liên quan các nhánh R60/R58 của Wi-Fi. GPIOA_18/19/28/29 còn có các nhánh RN1 Bluetooth được ghi NC.

CAM_SCL/CAM_SDA là bus dùng chung camera và touch với pull-up lên 1,8 V. CAM_IO1 có nhánh 0 ohm đến MIPI_RX0_N. Mapping lane phụ thuộc camera và revision; dùng bảng 70415 cho thiết kế này. Các nhãn 1,8 V cạnh tín hiệu điều khiển không mô tả biên độ điện áp của toàn bộ các cặp MIPI.

VDDC_RTC chỉ được thể hiện nối U1.37 và C21; không tự nối nó với VDD0V9_CPU hoặc VDD1V8_SYS. GPIO_ZQ tại U1.35 có nhãn net nhưng không thấy tải/điện trở kết thúc riêng trong bốn trang. ADU_VREF chỉ có U1.3 và tụ lọc được thể hiện.

Các mã/giá trị chưa có trong PDF gồm model chính xác U9/U11/U12, model RJ1/U7/U8/L5/L6/D1, giá trị số của mạng RF C68-C70, dung lượng RAM/flash, mã panel, controller touch, cáp cụ thể ngoài các ghi chú camera, thông số trở kháng/chiều dài đường PCB và bảng BOM lắp theo từng biến thể. Riêng U11 còn thiếu số chân vật lý trên symbol. Không tự bổ sung các dữ liệu này như thể chúng có trong schematic.

Tài liệu không chứa firmware, device tree, pinmux hiện tại, tên thiết bị Linux, driver, baud rate, SSID, thông số MQTT/EMQX, địa chỉ IP hoặc chứng chỉ TLS. Nếu câu hỏi liên quan phần mềm, cần phân biệt rõ dữ kiện phần cứng ở đây với cấu hình phần mềm được cung cấp riêng.

Khi trả lời dựa trên tài liệu này, hãy nêu đúng reference, số chân và net liên quan. Khi cần suy luận, chỉ rõ đó là suy luận; khi sơ đồ thiếu dữ liệu hoặc có hai cách đặt tên khác nhau, giữ nguyên thông tin nguồn và nêu điểm chưa xác định.
