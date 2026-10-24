
## 0. GPIO explain
> Ghi chú (gốc là 1 ô tiêu đề gộp, không phải header cột): 芯片中主要有四种 IO 管脚. 管脚的电气参数如下
1. 1.8V IO
2. 18OD33 IO (1.8V mode & 3.0V mode)
3. Audio GPIO
4. ETH GPIO

| Parameter | Description | Min | Typ | Max | Unit |
| --- | --- | --- | --- | --- | --- |
| **1. 1.8V IO** |  |  |  |  |  |
| VIL | Input Low voltage | -0.3 |  | 0.35xVDDIO | V |
| VIH | Input High voltage | 0.65*VDDIO |  | 1.98 | V |
| VT (no pull) | Threshold voltage when ST[1:0] = 00 | 0.75 | 0.91 | 1.09 | V |
| VT\_PU | (no schmit trigger) | 0.74 | 0.90 | 1.08 | V |
| VT\_PD |  | 0.76 | 0.92 | 1.10 | V |
| VT+ | Threshold voltage when ST[1:0] = 01 | 0.82 | 0.97 | 1.13 | V |
| VT- | Threshold voltage when ST[1:0] = 01 | 0.72 | 0.85 | 1.02 | V |
| VT+\_PU | Threshold voltage when ST[1:0] = 01 | 0.81 | 0.96 | 1.12 | V |
| VT-\_PU | Threshold voltage when ST[1:0] = 01 | 0.71 | 0.84 | 1.01 | V |
| VT+\_PD | Threshold voltage when ST[1:0] = 01 | 0.82 | 0.98 | 1.14 | V |
| VT-\_PD | Threshold voltage when ST[1:0] = 01 | 0.73 | 0.86 | 1.03 | V |
| VT+ | Threshold voltage when ST[1:0] = 1X | 0.87 | 1.04 | 1.19 | V |
| VT- | Threshold voltage when ST[1:0] = 1X | 0.69 | 0.80 | 0.95 | V |
| VT+\_PU | Threshold voltage when ST[1:0] = 1X | 0.86 | 1.03 | 1.18 | V |
| VT-\_PU | Threshold voltage when ST[1:0] = 1X | 0.68 | 0.79 | 0.94 | V |
| VT+\_PD | Threshold voltage when ST[1:0] = 1X | 0.88 | 1.05 | 1.20 | V |
| VT-\_PD | Threshold voltage when ST[1:0] = 1X | 0.69 | 0.81 | 0.96 | V |
| Il | Input leakage (VI = 1.8V or 0V) |  |  | +/-10u | A |
| IOZ | Tri-state output leakage current (VO=1.8V or 0V) |  |  | +/-10u | A |
| RPU | Pull up resistor | 55k | 79k | 121k | Ω |
| RPD | Pull down resistor | 51k | 87k | 169k | Ω |
| VOL | Output low voltage |  |  | 0.45 | V |
| VOH | Output high voltage | 1.35 |  |  | V |
| IOL | Low level output current @ VOL (max) |  |  |  |  |
| IOL | DS[1:0] = 00 | 7.6 | 12.8 | 18.0 | mA |
| IOL | DS[1:0] = 01 | 15.2 | 25.3 | 35.5 | mA |
| IOL | DS[1:0] = 10 | 22.6 | 37.4 | 52.2 | mA |
| IOL | DS[1:0] = 11 | 29.7 | 49 | 67.9 | mA |
| IOH | High level output current @ VOH (max) |  |  |  |  |
| IOH | DS[1:0] = 00 | 4.8 | 10.8 | 18.9 | mA |
| IOH | DS[1:0] = 01 | 9.5 | 21.5 | 37.4 | mA |
| IOH | DS[1:0] = 10 | 14.3 | 32.1 | 55.9 | mA |
| IOH | DS[1:0] = 11 | 18.9 | 42.4 | 73.9 | mA |
| **2.1 18OD33 IO (VDDIO=1.8V) 电气参数** |  |  |  |  |  |
| VIL | Input Low voltage | -0.3 |  | 0.58 | V |
| VIH | Input High voltage | 1.27 |  | 2.00 | V |
| VT (no pull) | Threshold voltage when ST = 0 | 0.91 | 0.97 | 1.03 | V |
| VT\_PU | (no schmit trigger) | 0.90 | 0.96 | 1.02 | V |
| VT\_PD |  | 0.91 | 0.97 | 1.06 | V |
| VT+ (no pull) | Threshold voltage when ST = 1 | 1.03 | 1.07 | 1.12 | V |
| VT- (no pull) | Threshold voltage when ST = 1 | 0.75 | 0.83 | 0.91 | V |
| VT+\_PU | Threshold voltage when ST = 1 | 1.02 | 1.06 | 1.11 | V |
| VT-\_PU | Threshold voltage when ST = 1 | 0.74 | 0.82 | 0.90 | V |
| VT+\_PD | Threshold voltage when ST = 1 | 1.03 | 1.08 | 1.13 | V |
| VT-\_PD | Threshold voltage when ST = 1 | 0.75 | 0.83 | 0.92 | V |
| Il | Input leakage (VI = 1.8V or 0V) |  |  | +/-10u | A |
| IOZ | Tri-state output leakage current (VO=1.8V or 0V) |  |  | +/-10u | A |
| RPU | Pull up resistor | 33k | 60k | 92k | Ω |
| RPD | Pull down resistor | 34k | 61k | 158k | Ω |
| VOL | Output low voltage |  |  | 0.45 | V |
| VOH | Output high voltage | 1.40 |  |  | V |
| IOL | Low level output current @ VOL (max) |  |  |  |  |
| IOL | DS[2:0] = 000 | 4.9 | 7.8 | 11.1 | mA |
| IOL | DS[2:0] = 001 | 7.4 | 11.7 | 16.4 | mA |
| IOL | DS[2:0] = 010 | 9.8 | 15.5 | 21.7 | mA |
| IOL | DS[2:0] = 011 | 12.2 | 19.2 | 26.7 | mA |
| IOL | DS[2:0] = 100 | 14.6 | 23.0 | 31.9 | mA |
| IOL | DS[2:0] = 101 | 17.0 | 26.6 | 36.8 | mA |
| IOL | DS[2:0] = 110 | 19.4 | 30.2 | 41.6 | mA |
| IOL | DS[2:0] = 111 | 21.7 | 33.7 | 46.2 | mA |
| IOH | High level output current @ VOH (max) |  |  |  |  |
| IOH | DS[2:0] = 000 | 3.6 | 6.2 | 9.5 | mA |
| IOH | DS[2:0] = 001 | 5.4 | 9.3 | 14.3 | mA |
| IOH | DS[2:0] = 010 | 7.2 | 12.4 | 19.1 | mA |
| IOH | DS[2:0] = 011 | 9.0 | 15.4 | 23.8 | mA |
| IOH | DS[2:0] = 100 | 10.8 | 18.5 | 28.5 | mA |
| IOH | DS[2:0] = 101 | 12.6 | 21.6 | 33.1 | mA |
| IOH | DS[2:0] = 110 | 14.4 | 24.6 | 37.8 | mA |
| IOH | DS[2:0] = 111 | 16.2 | 27.7 | 42.5 | mA |
| **2.2 18OD33 IO (VDDIO=3.0V) 电气参数** |  |  |  |  |  |
| VIL | Input Low voltage | -0.3 |  | 0.25*VDDIO | V |
| VIH | Input High voltage | 0.625*VDDIO |  | 3.3 | V |
| VT (no pull) | Threshold voltage when ST = 0 | 0.82 | 0.95 | 1.11 | V |
| VT\_PU | (no schmit trigger) | 0.81 | 0.93 | 1.09 | V |
| VT\_PD |  | 0.83 | 0.96 | 1.13 | V |
| VT+ (no pull) | Threshold voltage when ST = 1 | 1.00 | 1.10 | 1.23 | V |
| VT- (no pull) | Threshold voltage when ST = 1 | 0.75 | 0.90 | 1.08 | V |
| VT+\_PU | Threshold voltage when ST = 1 | 1.00 | 1.09 | 1.21 | V |
| VT-\_PU | Threshold voltage when ST = 1 | 0.73 | 0.88 | 1.05 | V |
| VT+\_PD | Threshold voltage when ST = 1 | 1.01 | 1.11 | 1.25 | V |
| VT-\_PD | Threshold voltage when ST = 1 | 0.75 | 0.91 | 1.09 | V |
| Il | Input leakage (VI = 3.0V or 0V) |  |  | +/-10u | A |
| IOZ | Tri-state output leakage current (VO=3.0V or 0V) |  |  | +/-10u | A |
| RPU | Pull up resistor | 33k | 60k | 93k | Ω |
| RPD | Pull down resistor | 34k | 62k | 285k | Ω |
| VOL | Output low voltage |  |  | 0.125*VDDIO | V |
| VOH | Output high voltage | 0.75*VDDIO |  |  | V |
| IOL | Low level output current @ VOL (max) |  |  |  |  |
| IOL | DS[2:0] = 000 | 3.1 | 5.5 | 8.6 | mA |
| IOL | DS[2:0] = 001 | 4.7 | 8.2 | 12.7 | mA |
| IOL | DS[2:0] = 010 | 6.2 | 10.8 | 16.9 | mA |
| IOL | DS[2:0] = 011 | 7.7 | 13.4 | 20.8 | mA |
| IOL | DS[2:0] = 100 | 9.3 | 16.1 | 24.9 | mA |
| IOL | DS[2:0] = 101 | 10.8 | 18.7 | 28.8 | mA |
| IOL | DS[2:0] = 110 | 12.3 | 21.2 | 32.6 | mA |
| IOL | DS[2:0] = 111 | 13.8 | 23.7 | 36.3 | mA |
| IOH | High level output current @ VOH (max) |  |  |  |  |
| IOH | DS[2:0] = 000 | 5.0 | 7.5 | 10.5 | mA |
| IOH | DS[2:0] = 001 | 7.5 | 11.2 | 15.7 | mA |
| IOH | DS[2:0] = 010 | 10.1 | 14.9 | 21.0 | mA |
| IOH | DS[2:0] = 011 | 12.6 | 18.6 | 26.2 | mA |
| IOH | DS[2:0] = 100 | 15.1 | 22.3 | 31.4 | mA |
| IOH | DS[2:0] = 101 | 17.6 | 26.0 | 36.5 | mA |
| IOH | DS[2:0] = 110 | 20.1 | 29.8 | 41.8 | mA |
| IOH | DS[2:0] = 111 | 22.6 | 33.4 | 46.9 | mA |
| **3. Audio GPIO 电气参数** |  |  |  |  |  |
| VIL | Input Low voltage | -0.3 |  | 0.55 | V |
| VIH | Input High voltage | 1.2 |  | 1.98 | V |
| VT+ | Threshold voltage with schmitt trigger | 0.8 | 0.95 | 1.1 | V |
| VT- | Threshold voltage with schmitt trigger | 0.65 | 0.82 | 0.99 | V |
| Il | Input leakage (VI = 1.8V or 0V) |  |  | +/-4u | A |
| IOZ | Tri-state output leakage current (VO=1.8V or 0V) |  |  | +/-4u | A |
| VOL | Output low voltage |  |  | 0.4 | V |
| VOH | Output high voltage | 1.4 |  |  | V |
| IOL | Low level output current @ VOL (max) | 4.9 | 9.9 | 18.4 | mA |
| IOH | High level output current @ VOH (max) | 11.3 | 17.1 | 26.1 | mA |
| **4. ETH GPIO 电气参数** |  |  |  |  |  |
| VIL | Input Low voltage | -0.3 |  | 0.3*VDD18A | V |
| VIH | Input High voltage | 0.7*VDD18A |  | 1.98 | V |
| VT+ | Threshold voltage with schmitt trigger | 0.84 | 0.99 | 1.14 | V |
| VT- | Threshold voltage with schmitt trigger | 0.66 | 0.83 | 1.01 | V |
| Il | Input leakage (VI = 1.8V or 0V) |  |  | +/-1.3u | A |
| IOZ | Tri-state output leakage current (VO=1.8V or 0V) |  |  | +/-1.3u | A |
| VOL | Output low voltage |  |  | 0.4 | V |
| VOH | Output high voltage | VDD18A-0.4 |  |  | V |
| IOL | Low level output current @ VOL (max) DS=0 | 8.8 | 15.7 | 27.3 | mA |
| IOL | Low level output current @ VOL (max) DS=1 | 10.2 | 17.8 | 30.5 | mA |
| IOH | High level output current @ VOH (max) DS=0 | 4 | 5.3 | 7.4 | mA |
| IOH | High level output current @ VOH (max) DS=1 | 4.7 | 6.2 | 8.5 | mA |

## 1. Pinout Table (QFN)
| Pin Num | Pin Name | IO Type | IOGroup | PowerDomain | IO\_cfg\_register | Function\_select<br>\_register | fmux\_<br>default | Description | Note |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 6 | SD0\_CLK | 18OD33 IO | G10 | VDDIO\_SD0\_EMMC | IOBLK\_G10\_REG\_SD0\_CLK<br>0x0300\_1A00 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CLK<br>0x0300\_101C | 0x0 | IO SD0\_CLK function select :<br><br>    0 : SDIO0\_CLK (default)<br><br>    1 : IIC1\_SDA<br><br>    2 : SPI0\_SCK<br><br>    3 : XGPIOA[7]<br><br>    5 : PWM[15]<br><br>    6 : EPHY\_LNK\_LED<br><br>    7 : DBG[0]<br><br>    Others : Reserved |  |
| 7 | SD0\_CMD | 18OD33 IO | G10 | VDDIO\_SD0\_EMMC | IOBLK\_G10\_REG\_SD0\_CMD<br>0x0300\_1A04 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CMD<br>0x0300\_1020 | 0x0 | IO SD0\_CMD function select :<br><br>    0 : SDIO0\_CMD (default)<br><br>    1 : IIC1\_SCL<br><br>    2 : SPI0\_SDO<br><br>    3 : XGPIOA[8]<br><br>    5 : PWM[14]<br><br>    6 : EPHY\_SPD\_LED<br><br>    7 : DBG[1]<br><br>    Others : Reserved |  |
| 8 | SD0\_D0 | 18OD33 IO | G10 | VDDIO\_SD0\_EMMC | IOBLK\_G10\_REG\_SD0\_D0<br>0x0300\_1A08 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 | 0x0 | IO SD0\_D0 function select :<br><br>    0 : SDIO0\_D[0] (default)<br><br>    1 : CAM\_MCLK1<br><br>    2 : SPI0\_SDI<br><br>    3 : XGPIOA[9]<br><br>    4 : UART3\_TX<br><br>    5 : PWM[13]<br><br>    6 : WG0\_D0<br><br>    7 : DBG[2]<br><br>    Others : Reserved |  |
| 10 | SD0\_D1 | 18OD33 IO | G10 | VDDIO\_SD0\_EMMC | IOBLK\_G10\_REG\_SD0\_D1<br>0x0300\_1A0C | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 | 0x0 | IO SD0\_D1 function select :<br><br>    0 : SDIO0\_D[1] (default)<br><br>    1 : IIC1\_SDA<br><br>    2 : AUX0<br><br>    3 : XGPIOA[10]<br><br>    4 : UART1\_TX<br><br>    5 : PWM[12]<br><br>    6 : WG0\_D1<br><br>    7 : DBG[3]<br><br>    Others : Reserved |  |
| 11 | SD0\_D2 | 18OD33 IO | G10 | VDDIO\_SD0\_EMMC | IOBLK\_G10\_REG\_SD0\_D2<br>0x0300\_1A10 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C | 0x0 | IO SD0\_D2 function select :<br><br>    0 : SDIO0\_D[2] (default)<br><br>    1 : IIC1\_SCL<br><br>    2 : AUX1<br><br>    3 : XGPIOA[11]<br><br>    4 : UART1\_RX<br><br>    5 : PWM[11]<br><br>    6 : WG1\_D0<br><br>    7 : DBG[4]<br><br>    Others : Reserved |  |
| 12 | SD0\_D3 | 18OD33 IO | G10 | VDDIO\_SD0\_EMMC | IOBLK\_G10\_REG\_SD0\_D3<br>0x0300\_1A14 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 | 0x0 | IO SD0\_D3 function select :<br><br>    0 : SDIO0\_D[3] (default)<br><br>    1 : CAM\_MCLK0<br><br>    2 : SPI0\_CS\_X<br><br>    3 : XGPIOA[12]<br><br>    4 : UART3\_RX<br><br>    5 : PWM[10]<br><br>    6 : WG1\_D1<br><br>    7 : DBG[5]<br><br>    Others : Reserved |  |
| 14 | SD0\_CD | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_SD0\_CD<br>0x0300\_1900 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CD<br>0x0300\_1034 | 0x0 | IO SD0\_CD function select :<br><br>    0 : SDIO0\_CD (default)<br><br>    3 : XGPIOA[13]<br><br>    Others : Reserved |  |
| 15 | SD0\_PWR\_EN | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_SD0\_PWR\_EN<br>0x0300\_1904 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_PWR\_EN<br>0x0300\_1038 | 0x3 | IO SD0\_PWR\_EN function select :<br><br>    0 : SDIO0\_PWR\_EN<br><br>    3 : XGPIOA[14] (default)<br><br>    Others : Reserved |  |
| 17 | SPK\_EN | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_SPK\_EN<br>0x0300\_1908 | FMUX\_GPIO\_REG\_IOCTRL\_SPK\_EN<br>0x0300\_103C | 0x3 | IO SPK\_EN function select :<br><br>    3 : XGPIOA[15] (default)<br><br>    Others : Reserved |  |
| 18 | UART0\_TX | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_UART0\_TX<br>0x0300\_190C | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_TX<br>0x0300\_1040 | 0x0 | IO UART0\_TX function select :<br><br>    0 : UART0\_TX (default)<br><br>    1 : CAM\_MCLK1<br><br>    2 : PWM[4]<br><br>    3 : XGPIOA[16]<br><br>    4 : UART1\_TX<br><br>    5 : AUX1<br><br>    7 : DBG[6]<br><br>    Others : Reserved |  |
| 19 | UART0\_RX | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_UART0\_RX<br>0x0300\_1910 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_RX<br>0x0300\_1044 | 0x0 | IO UART0\_RX function select :<br><br>    0 : UART0\_RX (default)<br><br>    1 : CAM\_MCLK0<br><br>    2 : PWM[5]<br><br>    3 : XGPIOA[17]<br><br>    4 : UART1\_RX<br><br>    5 : AUX0<br><br>    7 : DBG[7]<br><br>    Others : Reserved |  |
| 20 | EMMC\_DAT2 | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_EMMC\_DAT2<br>0x0300\_1918 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT2<br>0x0300\_104C | 0x1 | IO EMMC\_DAT2 function select :<br><br>    0 : EMMC\_DAT[2]<br><br>    1 : SPINOR\_HOLD\_X (default)<br><br>    2 : SPINAND\_HOLD<br><br>    3 : XGPIOA[26]<br><br>    Others : Reserved |  |
| 21 | EMMC\_CLK | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_EMMC\_CLK<br>0x0300\_191C | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CLK<br>0x0300\_1050 | 0x1 | IO EMMC\_CLK function select :<br><br>    0 : EMMC\_CLK<br><br>    1 : SPINOR\_SCK (default)<br><br>    2 : SPINAND\_CLK<br><br>    3 : XGPIOA[22]<br><br>    Others : Reserved |  |
| 22 | EMMC\_DAT0 | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_EMMC\_DAT0<br>0x0300\_1920 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT0<br>0x0300\_1054 | 0x1 | IO EMMC\_DAT0 function select :<br><br>    0 : EMMC\_DAT[0]<br><br>    1 : SPINOR\_MOSI (default)<br><br>    2 : SPINAND\_MOSI<br><br>    3 : XGPIOA[25]<br><br>    Others : Reserved |  |
| 23 | EMMC\_DAT3 | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_EMMC\_DAT3<br>0x0300\_1924 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT3<br>0x0300\_1058 | 0x1 | IO EMMC\_DAT3 function select :<br><br>    0 : EMMC\_DAT[3]<br><br>    1 : SPINOR\_WP\_X (default)<br><br>    2 : SPINAND\_WP<br><br>    3 : XGPIOA[27]<br><br>    Others : Reserved |  |
| 24 | EMMC\_CMD | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_EMMC\_CMD<br>0x0300\_1928 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CMD<br>0x0300\_105C | 0x1 | IO EMMC\_CMD function select :<br><br>    0 : EMMC\_CMD<br><br>    1 : SPINOR\_MISO (default)<br><br>    2 : SPINAND\_MISO<br><br>    3 : XGPIOA[23]<br><br>    Others : Reserved |  |
| 25 | EMMC\_DAT1 | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_EMMC\_DAT1<br>0x0300\_192C | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT1<br>0x0300\_1060 | 0x1 | IO EMMC\_DAT1 function select :<br><br>    0 : EMMC\_DAT[1]<br><br>    1 : SPINOR\_CS\_X (default)<br><br>    2 : SPINAND\_CS<br><br>    3 : XGPIOA[24]<br><br>    Others : Reserved |  |
| 26 | JTAG\_CPU\_TMS | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS<br>0x0300\_1930 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 | 0x0 | IO JTAG\_CPU\_TMS function select :<br>    0 : CR\_4WTMS (default)<br>    1 : CAM\_MCLK0<br>    2 : PWM[7]<br>    3 : XGPIOA[19]<br>    4 : UART1\_RTS<br>    5 : AUX0<br>    6 : UART1\_TX<br>    7 : VO\_D[28]<br>    Others : Reserved |  |
| 27 | JTAG\_CPU\_TCK | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK<br>0x0300\_1934 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 | 0x0 | IO JTAG\_CPU\_TCK function select :<br>    0 : CR\_4WTCK (default)<br>    1 : CAM\_MCLK1<br>    2 : PWM[6]<br>    3 : XGPIOA[18]<br>    4 : UART1\_CTS<br>    5 : AUX1<br>    6 : UART1\_RX<br>    7 : VO\_D[29]<br>    Others : Reserved |  |
| 28 | IIC0\_SCL | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_IIC0\_SCL<br>0x0300\_193C | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SCL<br>0x0300\_1070 | 0x0 | IO IIC0\_SCL function select :<br>    0 : CR\_4WTDI (default)<br>    1 : UART1\_TX<br>    2 : UART2\_TX<br>    3 : XGPIOA[28]<br>    5 : WG0\_D0<br>    7 : DBG[10]<br>    Others : Reserved |  |
| 29 | IIC0\_SDA | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_IIC0\_SDA<br>0x0300\_1940 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SDA<br>0x0300\_1074 | 0x0 | IO IIC0\_SDA function select :<br>    0 : CR\_4WTDO (default)<br>    1 : UART1\_RX<br>    2 : UART2\_RX<br>    3 : XGPIOA[29]<br>    5 : WG0\_D1<br>    6 : WG1\_D0<br>    7 : DBG[11]<br>    Others : Reserved |  |
| 30 | AUX0 | 18OD33 IO | G7 | VDDIO\_SD0\_EMMC | IOBLK\_G7\_REG\_AUX0<br>0x0300\_1944 | FMUX\_GPIO\_REG\_IOCTRL\_AUX0<br>0x0300\_1078 | 0x3 | IO AUX0 function select :<br><br>    0 : AUX0<br><br>    3 : XGPIOA[30] (default)<br><br>    4 : IIS1\_MCLK<br><br>    5 : VO\_D[31]<br><br>    6 : WG1\_D1<br><br>    7 : DBG[12]<br><br>    Others : Reserved |  |
| 38 | PWR\_VBAT\_DET | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET<br>0x0502\_7000 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_VBAT\_DET<br>0x0300\_107C | 0x0 | IO PWR\_VBAT\_DET function select :<br><br>    0 : PWR\_VBAT\_DET (default)<br><br>    Others : Reserved |  |
| 39 | PWR\_RSTN | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_RSTN<br>0x0502\_7004 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_RSTN<br>0x0300\_1080 | 0x0 | IO PWR\_RSTN function select :<br><br>    0 : PWR\_RSTN (default)<br><br>    Others : Reserved |  |
| 40 | PWR\_SEQ1 | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_SEQ1<br>0x0502\_7008 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_SEQ1<br>0x0300\_1084 | 0x0 | IO PWR\_SEQ1 function select :<br><br>    0 : PWR\_SEQ1 (default)<br><br>    3 : PWR\_GPIO[3]<br><br>    Others : Reserved |  |
| 41 | PWR\_SEQ2 | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_SEQ2<br>0x0502\_700C | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_SEQ2<br>0x0300\_1088 | 0x0 | IO PWR\_SEQ2 function select :<br><br>    0 : PWR\_SEQ2 (default)<br><br>    3 : PWR\_GPIO[4]<br><br>    Others : Reserved |  |
| 43 | PWR\_WAKEUP0 | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0<br>0x0502\_7018 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 | 0x0 | IO PWR\_WAKEUP0 function select :<br><br>    0 : PWR\_WAKEUP0 (default)<br><br>    1 : PWR\_IR0<br><br>    2 : PWR\_UART0\_TX<br><br>    3 : PWR\_GPIO[6]<br><br>    4 : UART1\_TX<br><br>    5 : IIC4\_SCL<br><br>    6 : EPHY\_LNK\_LED<br><br>    7 : WG2\_D0<br><br>    Others : Reserved |  |
| 44 | PWR\_BUTTON1 | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_BUTTON1<br>0x0502\_7020 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_BUTTON1<br>0x0300\_1098 | 0x0 | IO PWR\_BUTTON1 function select :<br><br>    0 : PWR\_BUTTON1 (default)<br><br>    3 : PWR\_GPIO[8]<br><br>    4 : UART1\_RX<br><br>    5 : IIC4\_SDA<br><br>    6 : EPHY\_SPD\_LED<br><br>    7 : WG2\_D1<br><br>    Others : Reserved |  |
| 45 | XTAL\_XIN | Xtal | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_XTAL\_XIN<br>0x0502\_7028 | FMUX\_GPIO\_REG\_IOCTRL\_XTAL\_XIN<br>0x0300\_10A0 | 0x0 | IO XTAL\_XIN function select :<br><br>    0 : PWR\_XTAL\_CLKIN (default)<br><br>    Others : Reserved |  |
| 47 | PWR\_GPIO0 | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_GPIO0<br>0x0502\_702C | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO0<br>0x0300\_10A4 | 0x0 | IO PWR\_GPIO0 function select :<br><br>    0 : PWR\_GPIO[0] (default)<br><br>    1 : UART2\_TX<br><br>    2 : PWR\_UART0\_RX<br><br>    4 : PWM[8]<br><br>    Others : Reserved |  |
| 48 | PWR\_GPIO1 | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_GPIO1<br>0x0502\_7030 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO1<br>0x0300\_10A8 | 0x0 | IO PWR\_GPIO1 function select :<br>    0 : PWR\_GPIO[1] (default)<br>    1 : UART2\_RX<br>    3 : EPHY\_LNK\_LED<br>    4 : PWM[9]<br>    5 : PWR\_IIC\_SCL<br>    6 : IIC2\_SCL<br>    7 : CR\_SDA0<br>    Others : Reserved |  |
| 49 | PWR\_GPIO2 | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_PWR\_GPIO2<br>0x0502\_7034 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO2<br>0x0300\_10AC | 0x0 | IO PWR\_GPIO2 function select :<br>    0 : PWR\_GPIO[2] (default)<br>    2 : PWR\_SECTICK<br>    3 : EPHY\_SPD\_LED<br>    4 : PWM[10]<br>    5 : PWR\_IIC\_SDA<br>    6 : IIC2\_SDA<br>    7 : CR\_2WTCK<br>    Others : Reserved |  |
| 51 | SD1\_D3 | 18OD33 IO | GRTC | VDDIO\_SD1 | IOBLK\_GRTC\_REG\_SD1\_D3<br>0x0502\_7058 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 | 0x6 | IO SD1\_D3 function select :<br><br>    0 : PWR\_SD1\_D3\_VO32<br><br>    1 : SPI2\_CS\_X<br><br>    2 : IIC1\_SCL<br><br>    3 : PWR\_GPIO[18]<br><br>    4 : CAM\_MCLK0<br><br>    5 : UART3\_CTS<br><br>    6 : PWR\_SPINOR1\_CS\_X (default)<br><br>    7 : PWM[4]<br><br>    Others : Reserved | SD1\_D3 or VO[32]<br>Check 0x0502\_70E4 |
| 52 | SD1\_D2 | 18OD33 IO | GRTC | VDDIO\_SD1 | IOBLK\_GRTC\_REG\_SD1\_D2<br>0x0502\_705C | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 | 0x6 | IO SD1\_D2 function select :<br><br>    0 : PWR\_SD1\_D2\_VO33<br><br>    1 : IIC1\_SCL<br><br>    2 : UART2\_TX<br><br>    3 : PWR\_GPIO[19]<br><br>    4 : CAM\_MCLK0<br><br>    5 : UART3\_TX<br><br>    6 : PWR\_SPINOR1\_HOLD\_X (default)<br><br>    7 : PWM[5]<br><br>    Others : Reserved | SD1\_D2 or VO[33]<br>Check 0x0502\_70E4 |
| 53 | SD1\_D1 | 18OD33 IO | GRTC | VDDIO\_SD1 | IOBLK\_GRTC\_REG\_SD1\_D1<br>0x0502\_7060 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 | 0x6 | IO SD1\_D1 function select :<br><br>    0 : PWR\_SD1\_D1\_VO34<br><br>    1 : IIC1\_SDA<br><br>    2 : UART2\_RX<br><br>    3 : PWR\_GPIO[20]<br><br>    4 : CAM\_MCLK1<br><br>    5 : UART3\_RX<br><br>    6 : PWR\_SPINOR1\_WP\_X (default)<br><br>    7 : PWM[6]<br><br>    Others : Reserved | SD1\_D1 or VO[34]<br>Check 0x0502\_70E4 |
| 54 | SD1\_D0 | 18OD33 IO | GRTC | VDDIO\_SD1 | IOBLK\_GRTC\_REG\_SD1\_D0<br>0x0502\_7064 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC | 0x6 | IO SD1\_D0 function select :<br><br>    0 : PWR\_SD1\_D0\_VO35<br><br>    1 : SPI2\_SDI<br><br>    2 : IIC1\_SDA<br><br>    3 : PWR\_GPIO[21]<br><br>    4 : CAM\_MCLK1<br><br>    5 : UART3\_RTS<br><br>    6 : PWR\_SPINOR1\_MISO (default)<br><br>    7 : PWM[7]<br><br>    Others : Reserved | SD1\_D0 or VO[35]<br>Check 0x0502\_70E4 |
| 55 | SD1\_CMD | 18OD33 IO | GRTC | VDDIO\_SD1 | IOBLK\_GRTC\_REG\_SD1\_CMD<br>0x0502\_7068 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 | 0x6 | IO SD1\_CMD function select :<br><br>    0 : PWR\_SD1\_CMD\_VO36<br><br>    1 : SPI2\_SDO<br><br>    2 : IIC3\_SCL<br><br>    3 : PWR\_GPIO[22]<br><br>    4 : CAM\_VS0<br><br>    5 : EPHY\_LNK\_LED<br><br>    6 : PWR\_SPINOR1\_MOSI (default)<br><br>    7 : PWM[8]<br><br>    Others : Reserved | SD1\_CMD or VO[36]<br>Check 0x0502\_70E4 |
| 56 | SD1\_CLK | 18OD33 IO | GRTC | VDDIO\_SD1 | IOBLK\_GRTC\_REG\_SD1\_CLK<br>0x0502\_706C | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 | 0x6 | IO SD1\_CLK function select :<br><br>    0 : PWR\_SD1\_CLK\_VO37<br><br>    1 : SPI2\_SCK<br><br>    2 : IIC3\_SDA<br><br>    3 : PWR\_GPIO[23]<br><br>    4 : CAM\_HS0<br><br>    5 : EPHY\_SPD\_LED<br><br>    6 : PWR\_SPINOR1\_SCK (default)<br><br>    7 : PWM[9]<br><br>    Others : Reserved | SD1\_CLK or VO[37]<br>Check 0x0502\_70E4 |
| 58 | PWM0\_BUCK | 1.8V GPIO | G1 | VDD18A\_USB\_PLL\_ETH | IOBLK\_G1\_REG\_PWM0\_BUCK<br>0x0300\_1804 | FMUX\_GPIO\_REG\_IOCTRL\_PWM0\_BUCK<br>0x0300\_10EC | 0x3 | IO PWM0\_BUCK function select :<br><br>    0 : PWM[0]<br><br>    3 : XGPIOB[0] (default)<br><br>    Others : Reserved |  |
| 59 | ADC1 | 1.8V GPIO | G1 | VDD18A\_USB\_PLL\_ETH | IOBLK\_G1\_REG\_ADC1<br>0x0300\_1810 | FMUX\_GPIO\_REG\_IOCTRL\_ADC1<br>0x0300\_10F8 | 0x3 | IO ADC1 function select :<br><br>    3 : XGPIOB[3] (default)<br><br>    4 : KEY\_COL2<br><br>    Others : Reserved |  |
| 60 | USB\_VBUS\_DET | 1.8V GPIO | G1 | VDD18A\_USB\_PLL\_ETH | IOBLK\_G1\_REG\_USB\_VBUS\_DET<br>0x0300\_1820 | FMUX\_GPIO\_REG\_IOCTRL\_USB\_VBUS\_DET<br>0x0300\_1108 | 0x0 | IO USB\_VBUS\_DET function select :<br><br>    0 : USB\_VBUS\_DET (default)<br><br>    3 : XGPIOB[6]<br><br>    4 : CAM\_MCLK0<br><br>    5 : CAM\_MCLK1<br><br>    Others : Reserved |  |
| 62 | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | ETH GPIO<br>(1.8V) |  | VDD18A\_USB\_PLL\_ETH |  | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 | 0x3 | IO PAD\_ETH\_TXP function select :<br><br>    1 : UART3\_RX<br><br>    2 : IIC1\_SCL<br><br>    3 : XGPIOB[25] (default)<br><br>    4 : PWM[13]<br><br>    5 : CAM\_MCLK0<br><br>    6 : SPI1\_SDO<br><br>    7 : IIS2\_LRCK<br><br>    Others : Reserved |  |
| 63 | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | ETH GPIO<br>(1.8V) |  | VDD18A\_USB\_PLL\_ETH |  | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 | 0x3 | IO PAD\_ETH\_TXM function select :<br><br>    1 : UART3\_RTS<br><br>    2 : IIC1\_SDA<br><br>    3 : XGPIOB[24] (default)<br><br>    4 : PWM[12]<br><br>    5 : CAM\_MCLK1<br><br>    6 : SPI1\_SDI<br><br>    7 : IIS2\_BCLK<br><br>    Others : Reserved |  |
| 64 | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | ETH GPIO<br>(1.8V) |  | VDD18A\_USB\_PLL\_ETH |  | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C | 0x3 | IO PAD\_ETH\_RXP function select :<br><br>    1 : UART3\_TX<br><br>    2 : CAM\_MCLK1<br><br>    3 : XGPIOB[27] (default)<br><br>    4 : PWM[15]<br><br>    5 : CAM\_HS0<br><br>    6 : SPI1\_SCK<br><br>    7 : IIS2\_DO<br><br>    Others : Reserved |  |
| 65 | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | ETH GPIO<br>(1.8V) |  | VDD18A\_USB\_PLL\_ETH |  | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 | 0x3 | IO PAD\_ETH\_RXM function select :<br><br>    1 : UART3\_CTS<br><br>    2 : CAM\_MCLK0<br><br>    3 : XGPIOB[26] (default)<br><br>    4 : PWM[14]<br><br>    5 : CAM\_VS0<br><br>    6 : SPI1\_CS\_X<br><br>    7 : IIS2\_DI<br><br>    Others : Reserved |  |
| 72 | PAD\_MIPIRX4N | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX4N<br>0x0300\_1C38 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C | 0x0 | IO PAD\_MIPIRX4N function select :<br>    0 : CR\_SCL0 (default)<br>    1 : VI0\_CLK<br>    2 : VI1\_D[13]<br>    3 : XGPIOC[2]<br>    4 : IIC1\_SDA<br>    5 : CAM\_MCLK0<br>    6 : KEY\_ROW0<br>    7 : MUX\_SPI1\_SCK<br>    Others : Reserved |  |
| 73 | PAD\_MIPIRX4P | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX4P<br>0x0300\_1C3C | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 | 0x0 | IO PAD\_MIPIRX4P function select :<br>    0 : CR\_SDA0 (default)<br>    1 : VI0\_D[0]<br>    2 : VI1\_D[14]<br>    3 : XGPIOC[3]<br>    4 : IIC1\_SCL<br>    5 : CAM\_MCLK1<br>    6 : KEY\_ROW1<br>    7 : MUX\_SPI1\_CS<br>    Others : Reserved |  |
| 74 | PAD\_MIPIRX3N | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX3N<br>0x0300\_1C40 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3N<br>0x0300\_1174 | 0x3 | IO PAD\_MIPIRX3N function select :<br>    0 : CR\_2WTMS<br>    1 : VI0\_D[1]<br>    2 : VI1\_D[15]<br>    3 : XGPIOC[4] (default)<br>    4 : CAM\_MCLK0<br>    7 : MUX\_SPI1\_MISO<br>    Others : Reserved |  |
| 75 | PAD\_MIPIRX3P | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX3P<br>0x0300\_1C44 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3P<br>0x0300\_1178 | 0x3 | IO PAD\_MIPIRX3P function select :<br>    0 : CR\_2WTCK<br>    1 : VI0\_D[2]<br>    2 : VI1\_D[16]<br>    3 : XGPIOC[5] (default)<br>    7 : MUX\_SPI1\_MOSI<br>    Others : Reserved |  |
| 76 | PAD\_MIPIRX2N | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX2N<br>0x0300\_1C48 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2N<br>0x0300\_117C | 0x3 | IO PAD\_MIPIRX2N function select :<br><br>    1 : VI0\_D[3]<br><br>    2 : VO\_D[10]<br><br>    3 : XGPIOC[6] (default)<br><br>    4 : VI1\_D[17]<br><br>    5 : IIC4\_SCL<br><br>    7 : DBG[6]<br><br>    Others : Reserved |  |
| 77 | PAD\_MIPIRX2P | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX2P<br>0x0300\_1C4C | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2P<br>0x0300\_1180 | 0x3 | IO PAD\_MIPIRX2P function select :<br><br>    1 : VI0\_D[4]<br><br>    2 : VO\_D[9]<br><br>    3 : XGPIOC[7] (default)<br><br>    4 : VI1\_D[18]<br><br>    5 : IIC4\_SDA<br><br>    7 : DBG[7]<br><br>    Others : Reserved |  |
| 78 | PAD\_MIPIRX1N | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX1N<br>0x0300\_1C50 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1N<br>0x0300\_1184 | 0x3 | IO PAD\_MIPIRX1N function select :<br><br>    1 : VI0\_D[5]<br><br>    2 : VO\_D[8]<br><br>    3 : XGPIOC[8] (default)<br><br>    6 : KEY\_ROW3<br><br>    7 : DBG[8]<br><br>    Others : Reserved |  |
| 79 | PAD\_MIPIRX1P | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX1P<br>0x0300\_1C54 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1P<br>0x0300\_1188 | 0x3 | IO PAD\_MIPIRX1P function select :<br><br>    1 : VI0\_D[6]<br><br>    2 : VO\_D[7]<br><br>    3 : XGPIOC[9] (default)<br><br>    4 : IIC1\_SDA<br><br>    6 : KEY\_ROW2<br><br>    7 : DBG[9]<br><br>    Others : Reserved |  |
| 80 | PAD\_MIPIRX0N | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX0N<br>0x0300\_1C58 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0N<br>0x0300\_118C | 0x3 | IO PAD\_MIPIRX0N function select :<br><br>    1 : VI0\_D[7]<br><br>    2 : VO\_D[6]<br><br>    3 : XGPIOC[10] (default)<br><br>    4 : IIC1\_SCL<br><br>    5 : CAM\_MCLK1<br><br>    7 : DBG[10]<br><br>    Others : Reserved |  |
| 81 | PAD\_MIPIRX0P | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPIRX0P<br>0x0300\_1C5C | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0P<br>0x0300\_1190 | 0x3 | IO PAD\_MIPIRX0P function select :<br><br>    1 : VI0\_D[8]<br><br>    2 : VO\_D[5]<br><br>    3 : XGPIOC[11] (default)<br><br>    4 : CAM\_MCLK0<br><br>    7 : DBG[11]<br><br>    Others : Reserved |  |
| 83 | PAD\_MIPI\_TXM2 | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2<br>0x0300\_1C70 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 | 0x3 | IO PAD\_MIPI\_TXM2 function select :<br>    0 : CR\_SDA0<br>    1 : VI0\_D[13]<br>    2 : VO\_D[0]<br>    3 : XGPIOC[16] (default)<br>    4 : IIC1\_SDA<br>    5 : PWM[8]<br>    6 : SPI0\_SCK<br>    7 : SD1\_D2<br>    Others : Reserved |  |
| 84 | PAD\_MIPI\_TXP2 | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2<br>0x0300\_1C74 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 | 0x3 | IO PAD\_MIPI\_TXP2 function select :<br>    0 : CR\_SCL0<br>    1 : VI0\_D[14]<br>    2 : VO\_CLK0<br>    3 : XGPIOC[17] (default)<br>    4 : IIC1\_SCL<br>    5 : PWM[9]<br>    6 : SPI0\_CS\_X<br>    7 : SD1\_D3<br>    Others : Reserved |  |
| 85 | PAD\_MIPI\_TXM1 | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1<br>0x0300\_1C78 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC | 0x3 | IO PAD\_MIPI\_TXM1 function select :<br>    0 : CR\_2WTMS<br>    1 : VI0\_D[11]<br>    2 : VO\_D[2]<br>    3 : XGPIOC[14] (default)<br>    4 : IIC2\_SDA<br>    5 : PWM[10]<br>    6 : SPI0\_SDO<br>    7 : DBG[14]<br>    Others : Reserved |  |
| 86 | PAD\_MIPI\_TXP1 | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1<br>0x0300\_1C7C | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 | 0x3 | IO PAD\_MIPI\_TXP1 function select :<br>    0 : CR\_2WTCK<br>    1 : VI0\_D[12]<br>    2 : VO\_D[1]<br>    3 : XGPIOC[15] (default)<br>    4 : IIC2\_SCL<br>    5 : PWM[11]<br>    6 : SPI0\_SDI<br>    7 : DBG[15]<br>    Others : Reserved |  |
| 87 | PAD\_MIPI\_TXM0 | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0<br>0x0300\_1C80 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 | 0x3 | IO PAD\_MIPI\_TXM0 function select :<br><br>    1 : VI0\_D[9]<br><br>    2 : VO\_D[4]<br><br>    3 : XGPIOC[12] (default)<br><br>    4 : CAM\_MCLK1<br><br>    5 : PWM[14]<br><br>    6 : CAM\_VS0<br><br>    7 : DBG[12]<br><br>    Others : Reserved |  |
| 88 | PAD\_MIPI\_TXP0 | 1.8V GPIO | G12 | VDD18A\_MIPI | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0<br>0x0300\_1C84 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 | 0x3 | IO PAD\_MIPI\_TXP0 function select :<br><br>    1 : VI0\_D[10]<br><br>    2 : VO\_D[3]<br><br>    3 : XGPIOC[13] (default)<br><br>    4 : CAM\_MCLK0<br><br>    5 : PWM[15]<br><br>    6 : CAM\_HS0<br><br>    7 : DBG[13]<br><br>    Others : Reserved |  |
| 2 | PAD\_AUD\_AINL\_MIC | AUDIO GPIO<br>(1.8V) |  | VDD18A\_MIPI |  | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AINL\_MIC<br>0x0300\_11BC | 0x3 | IO PAD\_AUD\_AINL\_MIC function select :<br><br>    3 : XGPIOC[23] (default)<br><br>    4 : IIS1\_BCLK<br><br>    5 : IIS2\_BCLK<br><br>    Others : Reserved |  |
| 4 | PAD\_AUD\_AOUTR | AUDIO GPIO<br>(1.8V) |  | VDD18A\_MIPI |  | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AOUTR<br>0x0300\_11C8 | 0x3 | IO PAD\_AUD\_AOUTR function select :<br><br>    3 : XGPIOC[24] (default)<br><br>    4 : IIS1\_DI<br><br>    5 : IIS2\_DO<br><br>    6 : IIS1\_DO<br><br>    Others : Reserved |  |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | 1.8V GPIO | G12 | VDD18A\_USB\_PLL\_ETH | IOBLK\_G12\_REG\_GPIO\_RTX<br>0x0300\_1C8C | FMUX\_GPIO\_REG\_IOCTRL\_GPIO\_RTX<br>0x0300\_11CC | 0x3 | IO GPIO\_RTX function select :<br><br>    3 : XGPIOB[23] (default)<br><br>    4 : PWM[1]<br><br>    5 : CAM\_MCLK0<br><br>    Others : Reserved | CPU SEL<br>Low----CA53<br>High---CA906 |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | 1.8V GPIO | GRTC | VDDIO\_RTC | IOBLK\_GRTC\_REG\_GPIO\_ZQ<br>0x0502\_70E0 | FMUX\_GPIO\_REG\_IOCTRL\_GPIO\_ZQ<br>0x0300\_11D0 | 0x3 | IO GPIO\_ZQ function select :<br><br>    3 : PWR\_GPIO[24] (default)<br><br>    4 : PWM[2]<br><br>    Others : Reserved |  |
|  | PKG\_TYPE0 | 1.8V GPIO | G1 | VDD18A\_USB\_PLL\_ETH | IOBLK\_G1\_REG\_PKG\_TYPE0<br>0x0300\_181C | FMUX\_GPIO\_REG\_IOCTRL\_PKG\_TYPE0<br>0x0300\_1104 | 0x0 | IO PKG\_TYPE0 function select :<br><br>    0 : PKG\_TYPE0 (default)<br><br>    Others : Reserved | no pin out |
|  | PKG\_TYPE1 | 1.8V GPIO | G1 | VDD18A\_USB\_PLL\_ETH | IOBLK\_G1\_REG\_PKG\_TYPE1<br>0x0300\_1824 | FMUX\_GPIO\_REG\_IOCTRL\_PKG\_TYPE1<br>0x0300\_110C | 0x0 | IO PKG\_TYPE1 function select :<br><br>    0 : PKG\_TYPE1 (default)<br><br>    Others : Reserved | no pin out |
|  | PKG\_TYPE2 | 1.8V GPIO | G1 | VDD18A\_USB\_PLL\_ETH | IOBLK\_G1\_REG\_PKG\_TYPE2<br>0x0300\_1828 | FMUX\_GPIO\_REG\_IOCTRL\_PKG\_TYPE2<br>0x0300\_1110 | 0x0 | IO PKG\_TYPE2 function select :<br><br>    0 : PKG\_TYPE2 (default)<br><br>    Others : Reserved | no pin out |
|  | MUX\_SPI1\_MISO | NA | NA | NA |  | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MISO<br>0x0300\_1114 | 0x3 | IO MUX\_SPI1\_MISO function select :<br><br>    1 : UART3\_RTS<br><br>    2 : IIC1\_SDA<br><br>    3 : XGPIOB[8] (default)<br><br>    4 : PWM[9]<br><br>    5 : KEY\_COL1<br><br>    6 : SPI1\_SDI<br><br>    7 : DBG[14]<br><br>    Others : Reserved | Result is feed to PAD\_MIPIRX3N func7 |
|  | MUX\_SPI1\_MOSI | NA | NA | NA |  | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MOSI<br>0x0300\_1118 | 0x3 | IO MUX\_SPI1\_MOSI function select :<br><br>    1 : UART3\_RX<br><br>    2 : IIC1\_SCL<br><br>    3 : XGPIOB[7] (default)<br><br>    4 : PWM[8]<br><br>    5 : KEY\_COL0<br><br>    6 : SPI1\_SDO<br><br>    7 : DBG[13]<br><br>    Others : Reserved | Result is feed to PAD\_MIPIRX3P func7 |
|  | MUX\_SPI1\_CS | NA | NA | NA |  | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_CS<br>0x0300\_111C | 0x3 | IO MUX\_SPI1\_CS function select :<br><br>    1 : UART3\_CTS<br><br>    2 : CAM\_MCLK0<br><br>    3 : XGPIOB[10] (default)<br><br>    4 : PWM[11]<br><br>    5 : KEY\_ROW3<br><br>    6 : SPI1\_CS\_X<br><br>    7 : DBG[16]<br><br>    Others : Reserved | Result is feed to PAD\_MIPIRX4P func7 |
|  | MUX\_SPI1\_SCK | NA | NA | NA |  | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_SCK<br>0x0300\_1120 | 0x3 | IO MUX\_SPI1\_SCK function select :<br><br>    1 : UART3\_TX<br><br>    2 : CAM\_MCLK1<br><br>    3 : XGPIOB[9] (default)<br><br>    4 : PWM[10]<br><br>    5 : KEY\_ROW2<br><br>    6 : SPI1\_SCK<br><br>    7 : DBG[15]<br><br>    Others : Reserved | Result is feed to PAD\_MIPIRX4N func7 |

## 2. Functional Signal Table (QFN)
| Interface | Signal Name | Description | Direction | PinName | Function Number | Function select register |
| --- | --- | --- | --- | --- | --- | --- |
| ADC | (ADC1) |  | I | ADC1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_ADC1<br>0x0300\_10F8 |
| No die domain ADC | PWR\_GPIO[2](PWRSAR.VIN1) |  | I | PWR\_GPIO2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO2<br>0x0300\_10AC |
| No die domain ADC | PWR\_GPIO[1](PWRSAR1.VIN2) |  | I | PWR\_GPIO1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO1<br>0x0300\_10A8 |
| No die domain ADC | PWR\_VBAT\_DET(PWRSAR.VIN3) |  | I | PWR\_VBAT\_DET | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_VBAT\_DET<br>0x0300\_107C |
| Audio | (PAD\_AUD\_AINL\_MIC) |  | I | PAD\_AUD\_AINL\_MIC | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AINL\_MIC<br>0x0300\_11BC |
| Audio | (PAD\_AUD\_AOUTR) |  | O | PAD\_AUD\_AOUTR | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AOUTR<br>0x0300\_11C8 |
| Ethernet | (PAD\_ETH\_RXM) |  | I/O | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 |
| Ethernet | (PAD\_ETH\_RXP) |  | I/O | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C |
| Ethernet | (PAD\_ETH\_TXM) |  | I/O | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 |
| Ethernet | (PAD\_ETH\_TXP) |  | I/O | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 |
| Ethernet | EPHY\_LNK\_LED |  | O | SD0\_CLK | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CLK<br>0x0300\_101C |
| Ethernet | EPHY\_LNK\_LED |  | O | PWR\_WAKEUP0 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 |
| Ethernet | EPHY\_LNK\_LED |  | O | SD1\_CMD | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| Ethernet | EPHY\_LNK\_LED |  | O | PWR\_GPIO1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO1<br>0x0300\_10A8 |
| Ethernet | EPHY\_SPD\_LED |  | O | SD0\_CMD | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CMD<br>0x0300\_1020 |
| Ethernet | EPHY\_SPD\_LED |  | O | PWR\_BUTTON1 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_BUTTON1<br>0x0300\_1098 |
| Ethernet | EPHY\_SPD\_LED |  | O | SD1\_CLK | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| Ethernet | EPHY\_SPD\_LED |  | O | PWR\_GPIO2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO2<br>0x0300\_10AC |
| DSI/LVDS | (PAD\_MIPI\_TXM0) |  | I/O | PAD\_MIPI\_TXM0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 |
| DSI/LVDS | (PAD\_MIPI\_TXP0) |  | I/O | PAD\_MIPI\_TXP0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 |
| DSI/LVDS | (PAD\_MIPI\_TXM1) |  | I/O | PAD\_MIPI\_TXM1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| DSI/LVDS | (PAD\_MIPI\_TXP1) |  | I/O | PAD\_MIPI\_TXP1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| DSI/LVDS | (PAD\_MIPI\_TXM2) |  | I/O | PAD\_MIPI\_TXM2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 |
| DSI/LVDS | (PAD\_MIPI\_TXP2) |  | I/O | PAD\_MIPI\_TXP2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX0N) |  | I/O | PAD\_MIPIRX0N | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0N<br>0x0300\_118C |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX0P) |  | I/O | PAD\_MIPIRX0P | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0P<br>0x0300\_1190 |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX1N) |  | I/O | PAD\_MIPIRX1N | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1N<br>0x0300\_1184 |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX1P) |  | I/O | PAD\_MIPIRX1P | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1P<br>0x0300\_1188 |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX2N) |  | I/O | PAD\_MIPIRX2N | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2N<br>0x0300\_117C |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX2P) |  | I/O | PAD\_MIPIRX2P | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2P<br>0x0300\_1180 |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX3N) |  | I/O | PAD\_MIPIRX3N | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3N<br>0x0300\_1174 |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX3P) |  | I/O | PAD\_MIPIRX3P | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3P<br>0x0300\_1178 |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX4N) |  | I/O | PAD\_MIPIRX4N | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
| CSI/sLVDS/HiSPI | (PAD\_MIPIRX4P) |  | I/O | PAD\_MIPIRX4P | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
| Aux clock out | AUX0 |  | O | AUX0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_AUX0<br>0x0300\_1078 |
| Aux clock out | AUX0 |  | O | SD0\_D1 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 |
| Aux clock out | AUX0 |  | O | UART0\_RX | 5 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_RX<br>0x0300\_1044 |
| Aux clock out | AUX0 |  | O | JTAG\_CPU\_TMS | 5 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 |
| Aux clock out | AUX1 |  | O | SD0\_D2 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C |
| Aux clock out | AUX1 |  | O | UART0\_TX | 5 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_TX<br>0x0300\_1040 |
| Aux clock out | AUX1 |  | O | JTAG\_CPU\_TCK | 5 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 |
| Camera Interface | CAM\_HS0 |  | O | SD1\_CLK | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| Camera Interface | CAM\_HS0 |  | O | PAD\_MIPI\_TXP0 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 |
| Camera Interface | CAM\_HS0 |  | O | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C |
| Camera Interface | CAM\_MCLK0 |  | O | PAD\_MIPI\_TXP0 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 |
| Camera Interface | CAM\_MCLK0 |  | O | SD0\_D3 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 |
| Camera Interface | CAM\_MCLK0 |  | O | UART0\_RX | 1 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_RX<br>0x0300\_1044 |
| Camera Interface | CAM\_MCLK0 |  | O | JTAG\_CPU\_TMS | 1 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 |
| Camera Interface | CAM\_MCLK0 |  | O | SD1\_D3 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| Camera Interface | CAM\_MCLK0 |  | O | SD1\_D2 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| Camera Interface | CAM\_MCLK0 |  | O | USB\_VBUS\_DET | 4 | FMUX\_GPIO\_REG\_IOCTRL\_USB\_VBUS\_DET<br>0x0300\_1108 |
| Camera Interface | CAM\_MCLK0 |  | O | PAD\_MIPIRX3N | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3N<br>0x0300\_1174 |
| Camera Interface | CAM\_MCLK0 |  | O | PAD\_MIPIRX0P | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0P<br>0x0300\_1190 |
| Camera Interface | CAM\_MCLK0 |  | O | MUX\_SPI1\_CS | 2 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_CS<br>0x0300\_111C |
| Camera Interface | CAM\_MCLK0 |  | O | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 |
| Camera Interface | CAM\_MCLK0 |  | O | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 |
| Camera Interface | CAM\_MCLK0 |  | O | PAD\_MIPIRX4N | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
| Camera Interface | CAM\_MCLK0 |  | O | GPIO\_RTX\_\_\_EPHY\_RTX | 5 | FMUX\_GPIO\_REG\_IOCTRL\_GPIO\_RTX<br>0x0300\_11CC |
| Camera Interface | CAM\_MCLK1 |  | O | SD1\_D1 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| Camera Interface | CAM\_MCLK1 |  | O | SD1\_D0 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| Camera Interface | CAM\_MCLK1 |  | O | PAD\_MIPI\_TXM0 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 |
| Camera Interface | CAM\_MCLK1 |  | O | MUX\_SPI1\_SCK | 2 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_SCK<br>0x0300\_1120 |
| Camera Interface | CAM\_MCLK1 |  | O | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C |
| Camera Interface | CAM\_MCLK1 |  | O | USB\_VBUS\_DET | 5 | FMUX\_GPIO\_REG\_IOCTRL\_USB\_VBUS\_DET<br>0x0300\_1108 |
| Camera Interface | CAM\_MCLK1 |  | O | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 |
| Camera Interface | CAM\_MCLK1 |  | O | PAD\_MIPIRX4P | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
| Camera Interface | CAM\_MCLK1 |  | O | PAD\_MIPIRX0N | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0N<br>0x0300\_118C |
| Camera Interface | CAM\_MCLK1 |  | O | SD0\_D0 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 |
| Camera Interface | CAM\_MCLK1 |  | O | UART0\_TX | 1 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_TX<br>0x0300\_1040 |
| Camera Interface | CAM\_MCLK1 |  | O | JTAG\_CPU\_TCK | 1 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 |
| Camera Interface | CAM\_VS0 |  | O | SD1\_CMD | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| Camera Interface | CAM\_VS0 |  | O | PAD\_MIPI\_TXM0 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 |
| Camera Interface | CAM\_VS0 |  | O | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 |
| Parallel Video Out | VO\_CLK0 |  | O | PAD\_MIPI\_TXP2 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 |
| Parallel Video Out | VO\_D[0] |  | O | PAD\_MIPI\_TXM2 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 |
| Parallel Video Out | VO\_D[1] |  | O | PAD\_MIPI\_TXP1 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| Parallel Video Out | VO\_D[2] |  | O | PAD\_MIPI\_TXM1 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| Parallel Video Out | VO\_D[3] |  | O | PAD\_MIPI\_TXP0 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 |
| Parallel Video Out | VO\_D[4] |  | O | PAD\_MIPI\_TXM0 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 |
| Parallel Video Out | VO\_D[5] |  | O | PAD\_MIPIRX0P | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0P<br>0x0300\_1190 |
| Parallel Video Out | VO\_D[6] |  | O | PAD\_MIPIRX0N | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0N<br>0x0300\_118C |
| Parallel Video Out | VO\_D[7] |  | O | PAD\_MIPIRX1P | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1P<br>0x0300\_1188 |
| Parallel Video Out | VO\_D[8] |  | O | PAD\_MIPIRX1N | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1N<br>0x0300\_1184 |
| Parallel Video Out | VO\_D[9] |  | O | PAD\_MIPIRX2P | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2P<br>0x0300\_1180 |
| Parallel Video Out | VO\_D[10] |  | O | PAD\_MIPIRX2N | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2N<br>0x0300\_117C |
| Parallel Video Out | VO\_D[28] |  | O | JTAG\_CPU\_TMS | 7 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 |
| Parallel Video Out | VO\_D[29] |  | O | JTAG\_CPU\_TCK | 7 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 |
| Parallel Video Out | VO\_D[31] |  | O | AUX0 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_AUX0<br>0x0300\_1078 |
| Parallel Video Out | VO\_D[32] |  | O | SD1\_D3 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| Parallel Video Out | VO\_D[33] |  | O | SD1\_D2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| Parallel Video Out | VO\_D[34] |  | O | SD1\_D1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| Parallel Video Out | VO\_D[35] |  | O | SD1\_D0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| Parallel Video Out | VO\_D[36] |  | O | SD1\_CMD | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| Parallel Video Out | VO\_D[37] |  | O | SD1\_CLK | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| Parallel Video In | VI0\_CLK |  | I | PAD\_MIPIRX4N | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
| Parallel Video In | VI0\_D[0] |  | I | PAD\_MIPIRX4P | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
| Parallel Video In | VI0\_D[1] |  | I | PAD\_MIPIRX3N | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3N<br>0x0300\_1174 |
| Parallel Video In | VI0\_D[10] |  | I | PAD\_MIPI\_TXP0 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 |
| Parallel Video In | VI0\_D[11] |  | I | PAD\_MIPI\_TXM1 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| Parallel Video In | VI0\_D[12] |  | I | PAD\_MIPI\_TXP1 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| Parallel Video In | VI0\_D[13] |  | I | PAD\_MIPI\_TXM2 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 |
| Parallel Video In | VI0\_D[14] |  | I | PAD\_MIPI\_TXP2 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 |
| Parallel Video In | VI0\_D[2] |  | I | PAD\_MIPIRX3P | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3P<br>0x0300\_1178 |
| Parallel Video In | VI0\_D[3] |  | I | PAD\_MIPIRX2N | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2N<br>0x0300\_117C |
| Parallel Video In | VI0\_D[4] |  | I | PAD\_MIPIRX2P | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2P<br>0x0300\_1180 |
| Parallel Video In | VI0\_D[5] |  | I | PAD\_MIPIRX1N | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1N<br>0x0300\_1184 |
| Parallel Video In | VI0\_D[6] |  | I | PAD\_MIPIRX1P | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1P<br>0x0300\_1188 |
| Parallel Video In | VI0\_D[7] |  | I | PAD\_MIPIRX0N | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0N<br>0x0300\_118C |
| Parallel Video In | VI0\_D[8] |  | I | PAD\_MIPIRX0P | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0P<br>0x0300\_1190 |
| Parallel Video In | VI0\_D[9] |  | I | PAD\_MIPI\_TXM0 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 |
| Parallel Video In | VI1\_D[13] |  | I | PAD\_MIPIRX4N | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
| Parallel Video In | VI1\_D[14] |  | I | PAD\_MIPIRX4P | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
| Parallel Video In | VI1\_D[15] |  | I | PAD\_MIPIRX3N | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3N<br>0x0300\_1174 |
| Parallel Video In | VI1\_D[16] |  | I | PAD\_MIPIRX3P | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3P<br>0x0300\_1178 |
| Parallel Video In | VI1\_D[17] |  | I | PAD\_MIPIRX2N | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2N<br>0x0300\_117C |
| Parallel Video In | VI1\_D[18] |  | I | PAD\_MIPIRX2P | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2P<br>0x0300\_1180 |
| eMMC | EMMC\_CLK |  | O | EMMC\_CLK | 0 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CLK<br>0x0300\_1050 |
| eMMC | EMMC\_CMD |  | I/O | EMMC\_CMD | 0 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CMD<br>0x0300\_105C |
| eMMC | EMMC\_DAT[0] |  | I/O | EMMC\_DAT0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT0<br>0x0300\_1054 |
| eMMC | EMMC\_DAT[1] |  | I/O | EMMC\_DAT1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT1<br>0x0300\_1060 |
| eMMC | EMMC\_DAT[2] |  | I/O | EMMC\_DAT2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT2<br>0x0300\_104C |
| eMMC | EMMC\_DAT[3] |  | I/O | EMMC\_DAT3 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT3<br>0x0300\_1058 |
| SPI\_NAND | SPINAND\_CLK |  | O | EMMC\_CLK | 2 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CLK<br>0x0300\_1050 |
| SPI\_NAND | SPINAND\_CS |  | O | EMMC\_DAT1 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT1<br>0x0300\_1060 |
| SPI\_NAND | SPINAND\_HOLD |  | I/O | EMMC\_DAT2 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT2<br>0x0300\_104C |
| SPI\_NAND | SPINAND\_MISO |  | I/O | EMMC\_CMD | 2 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CMD<br>0x0300\_105C |
| SPI\_NAND | SPINAND\_MOSI |  | I/O | EMMC\_DAT0 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT0<br>0x0300\_1054 |
| SPI\_NAND | SPINAND\_WP |  | I/O | EMMC\_DAT3 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT3<br>0x0300\_1058 |
| SPI\_NOR | SPINOR\_CS\_X |  | O | EMMC\_DAT1 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT1<br>0x0300\_1060 |
| SPI\_NOR | SPINOR\_HOLD\_X |  | I/O | EMMC\_DAT2 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT2<br>0x0300\_104C |
| SPI\_NOR | SPINOR\_MISO |  | I/O | EMMC\_CMD | 1 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CMD<br>0x0300\_105C |
| SPI\_NOR | SPINOR\_MOSI |  | I/O | EMMC\_DAT0 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT0<br>0x0300\_1054 |
| SPI\_NOR | SPINOR\_SCK |  | O | EMMC\_CLK | 1 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CLK<br>0x0300\_1050 |
| SPI\_NOR | SPINOR\_WP\_X |  | I/O | EMMC\_DAT3 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT3<br>0x0300\_1058 |
| I2C | IIC1\_SCL |  | I/O | PAD\_MIPIRX4P | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
| I2C | IIC1\_SCL |  | I/O | PAD\_MIPIRX0N | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0N<br>0x0300\_118C |
| I2C | IIC1\_SCL |  | I/O | SD0\_CMD | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CMD<br>0x0300\_1020 |
| I2C | IIC1\_SCL |  | I/O | SD0\_D2 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C |
| I2C | IIC1\_SCL |  | I/O | PAD\_MIPI\_TXP2 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 |
| I2C | IIC1\_SCL |  | I/O | SD1\_D3 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| I2C | IIC1\_SCL |  | I/O | MUX\_SPI1\_MOSI | 2 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MOSI<br>0x0300\_1118 |
| I2C | IIC1\_SCL |  | I/O | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 |
| I2C | IIC1\_SCL |  | I/O | SD1\_D2 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| I2C | IIC1\_SDA |  | I/O | PAD\_MIPIRX4N | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
| I2C | IIC1\_SDA |  | I/O | PAD\_MIPIRX1P | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1P<br>0x0300\_1188 |
| I2C | IIC1\_SDA |  | I/O | SD0\_CLK | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CLK<br>0x0300\_101C |
| I2C | IIC1\_SDA |  | I/O | SD0\_D1 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 |
| I2C | IIC1\_SDA |  | I/O | PAD\_MIPI\_TXM2 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 |
| I2C | IIC1\_SDA |  | I/O | SD1\_D0 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| I2C | IIC1\_SDA |  | I/O | MUX\_SPI1\_MISO | 2 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MISO<br>0x0300\_1114 |
| I2C | IIC1\_SDA |  | I/O | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 |
| I2C | IIC1\_SDA |  | I/O | SD1\_D1 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| I2C | IIC2\_SCL |  | I/O | PAD\_MIPI\_TXP1 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| I2C | IIC2\_SCL |  | I/O | PWR\_GPIO1 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO1<br>0x0300\_10A8 |
| I2C | IIC2\_SDA |  | I/O | PAD\_MIPI\_TXM1 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| I2C | IIC2\_SDA |  | I/O | PWR\_GPIO2 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO2<br>0x0300\_10AC |
| I2C | IIC3\_SCL |  | I/O | SD1\_CMD | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| I2C | IIC3\_SDA |  | I/O | SD1\_CLK | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| I2C | IIC4\_SCL |  | I/O | PWR\_WAKEUP0 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 |
| I2C | IIC4\_SCL |  | I/O | PAD\_MIPIRX2N | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2N<br>0x0300\_117C |
| I2C | IIC4\_SDA |  | I/O | PWR\_BUTTON1 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_BUTTON1<br>0x0300\_1098 |
| I2C | IIC4\_SDA |  | I/O | PAD\_MIPIRX2P | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2P<br>0x0300\_1180 |
| no die I2C | PWR\_IIC\_SCL |  | I/O | PWR\_GPIO1 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO1<br>0x0300\_10A8 |
| no die I2C | PWR\_IIC\_SDA |  | I/O | PWR\_GPIO2 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO2<br>0x0300\_10AC |
| IIS | IIS1\_BCLK |  | I/O | PAD\_AUD\_AINL\_MIC | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AINL\_MIC<br>0x0300\_11BC |
| IIS | IIS1\_DI |  | I | PAD\_AUD\_AOUTR | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AOUTR<br>0x0300\_11C8 |
| IIS | IIS1\_DO |  | O | PAD\_AUD\_AOUTR | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AOUTR<br>0x0300\_11C8 |
| IIS | IIS1\_MCLK |  | I/O | AUX0 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_AUX0<br>0x0300\_1078 |
| IIS | IIS2\_BCLK |  | I/O | PAD\_AUD\_AINL\_MIC | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AINL\_MIC<br>0x0300\_11BC |
| IIS | IIS2\_BCLK |  | I/O | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 |
| IIS | IIS2\_DI |  | I | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 |
| IIS | IIS2\_DO |  | O | PAD\_AUD\_AOUTR | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AOUTR<br>0x0300\_11C8 |
| IIS | IIS2\_DO |  | O | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C |
| IIS | IIS2\_LRCK |  | I/O | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 |
|  | KEY\_COL0 |  | I/O | MUX\_SPI1\_MOSI | 5 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MOSI<br>0x0300\_1118 |
|  | KEY\_COL1 |  | I/O | MUX\_SPI1\_MISO | 5 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MISO<br>0x0300\_1114 |
|  | KEY\_COL2 |  | I/O | ADC1 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_ADC1<br>0x0300\_10F8 |
|  | KEY\_ROW0 |  | I/O | PAD\_MIPIRX4N | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
|  | KEY\_ROW1 |  | I/O | PAD\_MIPIRX4P | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
|  | KEY\_ROW2 |  | I/O | PAD\_MIPIRX1P | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1P<br>0x0300\_1188 |
|  | KEY\_ROW2 |  | I/O | MUX\_SPI1\_SCK | 5 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_SCK<br>0x0300\_1120 |
|  | KEY\_ROW3 |  | I/O | PAD\_MIPIRX1N | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1N<br>0x0300\_1184 |
|  | KEY\_ROW3 |  | I/O | MUX\_SPI1\_CS | 5 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_CS<br>0x0300\_111C |
| PWM | PWM[0] |  | I/O | PWM0\_BUCK | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWM0\_BUCK<br>0x0300\_10EC |
| PWM | PWM[1] |  | I/O | GPIO\_RTX\_\_\_EPHY\_RTX | 4 | FMUX\_GPIO\_REG\_IOCTRL\_GPIO\_RTX<br>0x0300\_11CC |
| PWM | PWM[2] |  | I/O | GPIO\_ZQ\_\_\_PAD\_ZQ | 4 | FMUX\_GPIO\_REG\_IOCTRL\_GPIO\_ZQ<br>0x0300\_11D0 |
| PWM | PWM[4] |  | I/O | UART0\_TX | 2 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_TX<br>0x0300\_1040 |
| PWM | PWM[4] |  | I/O | SD1\_D3 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| PWM | PWM[5] |  | I/O | UART0\_RX | 2 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_RX<br>0x0300\_1044 |
| PWM | PWM[5] |  | I/O | SD1\_D2 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| PWM | PWM[6] |  | I/O | JTAG\_CPU\_TCK | 2 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 |
| PWM | PWM[6] |  | I/O | SD1\_D1 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| PWM | PWM[7] |  | I/O | JTAG\_CPU\_TMS | 2 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 |
| PWM | PWM[7] |  | I/O | SD1\_D0 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| PWM | PWM[8] |  | I/O | PWR\_GPIO0 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO0<br>0x0300\_10A4 |
| PWM | PWM[8] |  | I/O | MUX\_SPI1\_MOSI | 4 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MOSI<br>0x0300\_1118 |
| PWM | PWM[8] |  | I/O | SD1\_CMD | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| PWM | PWM[8] |  | I/O | PAD\_MIPI\_TXM2 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 |
| PWM | PWM[9] |  | I/O | PWR\_GPIO1 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO1<br>0x0300\_10A8 |
| PWM | PWM[9] |  | I/O | MUX\_SPI1\_MISO | 4 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MISO<br>0x0300\_1114 |
| PWM | PWM[9] |  | I/O | SD1\_CLK | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| PWM | PWM[9] |  | I/O | PAD\_MIPI\_TXP2 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 |
| PWM | PWM[10] |  | I/O | PWR\_GPIO2 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO2<br>0x0300\_10AC |
| PWM | PWM[10] |  | I/O | MUX\_SPI1\_SCK | 4 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_SCK<br>0x0300\_1120 |
| PWM | PWM[10] |  | I/O | SD0\_D3 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 |
| PWM | PWM[10] |  | I/O | PAD\_MIPI\_TXM1 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| PWM | PWM[11] |  | I/O | MUX\_SPI1\_CS | 4 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_CS<br>0x0300\_111C |
| PWM | PWM[11] |  | I/O | SD0\_D2 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C |
| PWM | PWM[11] |  | I/O | PAD\_MIPI\_TXP1 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| PWM | PWM[12] |  | I/O | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 |
| PWM | PWM[12] |  | I/O | SD0\_D1 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 |
| PWM | PWM[13] |  | I/O | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 |
| PWM | PWM[13] |  | I/O | SD0\_D0 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 |
| PWM | PWM[14] |  | I/O | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 |
| PWM | PWM[14] |  | I/O | SD0\_CMD | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CMD<br>0x0300\_1020 |
| PWM | PWM[14] |  | I/O | PAD\_MIPI\_TXM0 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 |
| PWM | PWM[15] |  | I/O | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C |
| PWM | PWM[15] |  | I/O | SD0\_CLK | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CLK<br>0x0300\_101C |
| PWM | PWM[15] |  | I/O | PAD\_MIPI\_TXP0 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_4WTCK |  | I/O | JTAG\_CPU\_TCK | 0 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_4WTMS |  | I/O | JTAG\_CPU\_TMS | 0 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_2WTCK |  | I/O | PAD\_MIPIRX3P | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3P<br>0x0300\_1178 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_2WTCK |  | I/O | PAD\_MIPI\_TXP1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_2WTCK |  | I/O | PWR\_GPIO2 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO2<br>0x0300\_10AC |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_SCL0 |  | I/O | PAD\_MIPIRX4N | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_SCL0 |  | I/O | PAD\_MIPI\_TXP2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_2WTMS |  | I/O | PAD\_MIPIRX3N | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3N<br>0x0300\_1174 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_2WTMS |  | I/O | PAD\_MIPI\_TXM1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_SDA0 |  | I/O | PAD\_MIPIRX4P | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_SDA0 |  | I/O | PAD\_MIPI\_TXM2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_SDA0 |  | I/O | PWR\_GPIO1 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO1<br>0x0300\_10A8 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_4WTDI |  | I/O | IIC0\_SCL | 0 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SCL<br>0x0300\_1070 |
| CA53 JTAG (2W)<br><br>RISCV JTAG (4W)<br><br>I2C0 | CR\_4WTDO |  | I/O | IIC0\_SDA | 0 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SDA<br>0x0300\_1074 |
| System | PWR\_BUTTON1 |  | I | PWR\_BUTTON1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_BUTTON1<br>0x0300\_1098 |
| System | PWR\_RSTN |  | I | PWR\_RSTN | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_RSTN<br>0x0300\_1080 |
| System | PWR\_SEQ1 |  | O | PWR\_SEQ1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_SEQ1<br>0x0300\_1084 |
| System | PWR\_SEQ2 |  | O | PWR\_SEQ2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_SEQ2<br>0x0300\_1088 |
| System | PWR\_WAKEUP0 |  | I | PWR\_WAKEUP0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 |
| System | USB\_VBUS\_DET |  | I | USB\_VBUS\_DET | 0 | FMUX\_GPIO\_REG\_IOCTRL\_USB\_VBUS\_DET<br>0x0300\_1108 |
| no die IR | PWR\_IR0 |  | I | PWR\_WAKEUP0 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 |
| SPI\_NOR1 | PWR\_SPINOR1\_CS\_X |  | O | SD1\_D3 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| SPI\_NOR1 | PWR\_SPINOR1\_HOLD\_X |  | I/O | SD1\_D2 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| SPI\_NOR1 | PWR\_SPINOR1\_MISO |  | I/O | SD1\_D0 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| SPI\_NOR1 | PWR\_SPINOR1\_MOSI |  | I/O | SD1\_CMD | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| SPI\_NOR1 | PWR\_SPINOR1\_SCK |  | O | SD1\_CLK | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| SPI\_NOR1 | PWR\_SPINOR1\_WP\_X |  | I/O | SD1\_D1 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| SD1 | PWR\_SD1\_CLK |  | O | SD1\_CLK | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| SD1 | PWR\_SD1\_CMD |  | I/O | SD1\_CMD | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| SD1 | PWR\_SD1\_D0 |  | I/O | SD1\_D0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| SD1 | PWR\_SD1\_D1 |  | I/O | SD1\_D1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| SD1 | PWR\_SD1\_D2 |  | I/O | SD1\_D2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| SD1 | PWR\_SD1\_D3 |  | I/O | SD1\_D3 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| SD0 | SDIO0\_CD |  | I | SD0\_CD | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CD<br>0x0300\_1034 |
| SD0 | SDIO0\_CLK |  | O | SD0\_CLK | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CLK<br>0x0300\_101C |
| SD0 | SDIO0\_CMD |  | I/O | SD0\_CMD | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CMD<br>0x0300\_1020 |
| SD0 | SDIO0\_D[0] |  | I/O | SD0\_D0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 |
| SD0 | SDIO0\_D[1] |  | I/O | SD0\_D1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 |
| SD0 | SDIO0\_D[2] |  | I/O | SD0\_D2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C |
| SD0 | SDIO0\_D[3] |  | I/O | SD0\_D3 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 |
| SD0 | SDIO0\_PWR\_EN |  | O | SD0\_PWR\_EN | 0 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_PWR\_EN<br>0x0300\_1038 |
| SPI | SPI0\_CS\_X |  | O | SD0\_D3 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 |
| SPI | SPI0\_CS\_X |  | O | PAD\_MIPI\_TXP2 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 |
| SPI | SPI0\_SCK |  | O | SD0\_CLK | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CLK<br>0x0300\_101C |
| SPI | SPI0\_SCK |  | O | PAD\_MIPI\_TXM2 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 |
| SPI | SPI0\_SDI |  | I | SD0\_D0 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 |
| SPI | SPI0\_SDI |  | I | PAD\_MIPI\_TXP1 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| SPI | SPI0\_SDO |  | I/O | SD0\_CMD | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CMD<br>0x0300\_1020 |
| SPI | SPI0\_SDO |  | I/O | PAD\_MIPI\_TXM1 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| SPI | SPI1\_CS\_X |  | O | MUX\_SPI1\_CS | 6 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_CS<br>0x0300\_111C |
| SPI | SPI1\_CS\_X |  | O | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 |
| SPI | SPI1\_SCK |  | O | MUX\_SPI1\_SCK | 6 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_SCK<br>0x0300\_1120 |
| SPI | SPI1\_SCK |  | O | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C |
| SPI | SPI1\_SDI |  | I | MUX\_SPI1\_MISO | 6 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MISO<br>0x0300\_1114 |
| SPI | SPI1\_SDI |  | I | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 |
| SPI | SPI1\_SDO |  | I/O | MUX\_SPI1\_MOSI | 6 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MOSI<br>0x0300\_1118 |
| SPI | SPI1\_SDO |  | I/O | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 6 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 |
| SPI | SPI2\_CS\_X |  | O | SD1\_D3 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| SPI | SPI2\_SCK |  | O | SD1\_CLK | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| SPI | SPI2\_SDI |  | I | SD1\_D0 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| SPI | SPI2\_SDO |  | I/O | SD1\_CMD | 1 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| UART | UART0\_RX |  | I | UART0\_RX | 0 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_RX<br>0x0300\_1044 |
| UART | UART0\_TX |  | O | UART0\_TX | 0 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_TX<br>0x0300\_1040 |
| UART | UART1\_CTS |  | I | JTAG\_CPU\_TCK | 4 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 |
| UART | UART1\_RTS |  | O | JTAG\_CPU\_TMS | 4 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 |
| UART | UART1\_RX |  | I | SD0\_D2 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C |
| UART | UART1\_RX |  | I | UART0\_RX | 4 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_RX<br>0x0300\_1044 |
| UART | UART1\_RX |  | I | PWR\_BUTTON1 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_BUTTON1<br>0x0300\_1098 |
| UART | UART1\_RX |  | I | JTAG\_CPU\_TCK | 6 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 |
| UART | UART1\_RX |  | I | IIC0\_SDA | 1 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SDA<br>0x0300\_1074 |
| UART | UART1\_TX |  | O | SD0\_D1 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 |
| UART | UART1\_TX |  | O | UART0\_TX | 4 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_TX<br>0x0300\_1040 |
| UART | UART1\_TX |  | O | PWR\_WAKEUP0 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 |
| UART | UART1\_TX |  | O | JTAG\_CPU\_TMS | 6 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 |
| UART | UART1\_TX |  | O | IIC0\_SCL | 1 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SCL<br>0x0300\_1070 |
| UART | UART2\_RX |  | I | IIC0\_SDA | 2 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SDA<br>0x0300\_1074 |
| UART | UART2\_RX |  | I | SD1\_D1 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| UART | UART2\_RX |  | I | PWR\_GPIO1 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO1<br>0x0300\_10A8 |
| UART | UART2\_TX |  | O | IIC0\_SCL | 2 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SCL<br>0x0300\_1070 |
| UART | UART2\_TX |  | O | SD1\_D2 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| UART | UART2\_TX |  | O | PWR\_GPIO0 | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO0<br>0x0300\_10A4 |
| UART | UART3\_CTS |  | I | MUX\_SPI1\_CS | 1 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_CS<br>0x0300\_111C |
| UART | UART3\_CTS |  | I | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 |
| UART | UART3\_CTS |  | I | SD1\_D3 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| UART | UART3\_RTS |  | O | MUX\_SPI1\_MISO | 1 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MISO<br>0x0300\_1114 |
| UART | UART3\_RTS |  | O | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 |
| UART | UART3\_RTS |  | O | SD1\_D0 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| UART | UART3\_RX |  | I | SD0\_D3 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 |
| UART | UART3\_RX |  | I | MUX\_SPI1\_MOSI | 1 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MOSI<br>0x0300\_1118 |
| UART | UART3\_RX |  | I | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 |
| UART | UART3\_RX |  | I | SD1\_D1 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| UART | UART3\_TX |  | O | SD0\_D0 | 4 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 |
| UART | UART3\_TX |  | O | MUX\_SPI1\_SCK | 1 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_SCK<br>0x0300\_1120 |
| UART | UART3\_TX |  | O | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | 1 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C |
| UART | UART3\_TX |  | O | SD1\_D2 | 5 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| no die UART | PWR\_UART0\_RX |  | I | PWR\_GPIO0 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO0<br>0x0300\_10A4 |
| no die UART | PWR\_UART0\_TX |  | O | PWR\_WAKEUP0 | 2 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 |
| Wiegand | WG0\_D0 |  | I/O | SD0\_D0 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 |
| Wiegand | WG0\_D0 |  | I/O | IIC0\_SCL | 5 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SCL<br>0x0300\_1070 |
| Wiegand | WG0\_D1 |  | I/O | SD0\_D1 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 |
| Wiegand | WG0\_D1 |  | I/O | IIC0\_SDA | 5 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SDA<br>0x0300\_1074 |
| Wiegand | WG1\_D0 |  | I/O | SD0\_D2 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C |
| Wiegand | WG1\_D0 |  | I/O | IIC0\_SDA | 6 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SDA<br>0x0300\_1074 |
| Wiegand | WG1\_D1 |  | I/O | SD0\_D3 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 |
| Wiegand | WG1\_D1 |  | I/O | AUX0 | 6 | FMUX\_GPIO\_REG\_IOCTRL\_AUX0<br>0x0300\_1078 |
| Wiegand | WG2\_D0 |  | I/O | PWR\_WAKEUP0 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 |
| Wiegand | WG2\_D1 |  | I/O | PWR\_BUTTON1 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_BUTTON1<br>0x0300\_1098 |
| GPIO | XGPIOA[7] |  | I/O | SD0\_CLK | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CLK<br>0x0300\_101C |
| GPIO | XGPIOA[8] |  | I/O | SD0\_CMD | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CMD<br>0x0300\_1020 |
| GPIO | XGPIOA[9] |  | I/O | SD0\_D0 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 |
| GPIO | XGPIOA[10] |  | I/O | SD0\_D1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 |
| GPIO | XGPIOA[11] |  | I/O | SD0\_D2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C |
| GPIO | XGPIOA[12] |  | I/O | SD0\_D3 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 |
| GPIO | XGPIOA[13] |  | I/O | SD0\_CD | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CD<br>0x0300\_1034 |
| GPIO | XGPIOA[14] |  | I/O | SD0\_PWR\_EN | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_PWR\_EN<br>0x0300\_1038 |
| GPIO | XGPIOA[15] |  | I/O | SPK\_EN | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SPK\_EN<br>0x0300\_103C |
| GPIO | XGPIOA[16] |  | I/O | UART0\_TX | 3 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_TX<br>0x0300\_1040 |
| GPIO | XGPIOA[17] |  | I/O | UART0\_RX | 3 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_RX<br>0x0300\_1044 |
| GPIO | XGPIOA[18] |  | I/O | JTAG\_CPU\_TCK | 3 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TCK<br>0x0300\_1068 |
| GPIO | XGPIOA[19] |  | I/O | JTAG\_CPU\_TMS | 3 | FMUX\_GPIO\_REG\_IOCTRL\_JTAG\_CPU\_TMS<br>0x0300\_1064 |
| GPIO | XGPIOA[22] |  | I/O | EMMC\_CLK | 3 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CLK<br>0x0300\_1050 |
| GPIO | XGPIOA[23] |  | I/O | EMMC\_CMD | 3 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_CMD<br>0x0300\_105C |
| GPIO | XGPIOA[24] |  | I/O | EMMC\_DAT1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT1<br>0x0300\_1060 |
| GPIO | XGPIOA[25] |  | I/O | EMMC\_DAT0 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT0<br>0x0300\_1054 |
| GPIO | XGPIOA[26] |  | I/O | EMMC\_DAT2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT2<br>0x0300\_104C |
| GPIO | XGPIOA[27] |  | I/O | EMMC\_DAT3 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_EMMC\_DAT3<br>0x0300\_1058 |
| GPIO | XGPIOA[28] |  | I/O | IIC0\_SCL | 3 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SCL<br>0x0300\_1070 |
| GPIO | XGPIOA[29] |  | I/O | IIC0\_SDA | 3 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SDA<br>0x0300\_1074 |
| GPIO | XGPIOA[30] |  | I/O | AUX0 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_AUX0<br>0x0300\_1078 |
| GPIO | XGPIOB[0] |  | I/O | PWM0\_BUCK | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PWM0\_BUCK<br>0x0300\_10EC |
| GPIO | XGPIOB[10] |  | I/O | MUX\_SPI1\_CS | 3 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_CS<br>0x0300\_111C |
| GPIO | XGPIOB[23] |  | I/O | GPIO\_RTX\_\_\_EPHY\_RTX | 3 | FMUX\_GPIO\_REG\_IOCTRL\_GPIO\_RTX<br>0x0300\_11CC |
| GPIO | XGPIOB[24] |  | I/O | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXM<br>0x0300\_1128 |
| GPIO | XGPIOB[25] |  | I/O | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_TXP<br>0x0300\_1124 |
| GPIO | XGPIOB[26] |  | I/O | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXM<br>0x0300\_1130 |
| GPIO | XGPIOB[27] |  | I/O | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_ETH\_RXP<br>0x0300\_112C |
| GPIO | XGPIOB[3] |  | I/O | ADC1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_ADC1<br>0x0300\_10F8 |
| GPIO | XGPIOB[6] |  | I/O | USB\_VBUS\_DET | 3 | FMUX\_GPIO\_REG\_IOCTRL\_USB\_VBUS\_DET<br>0x0300\_1108 |
| GPIO | XGPIOB[7] |  | I/O | MUX\_SPI1\_MOSI | 3 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MOSI<br>0x0300\_1118 |
| GPIO | XGPIOB[8] |  | I/O | MUX\_SPI1\_MISO | 3 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MISO<br>0x0300\_1114 |
| GPIO | XGPIOB[9] |  | I/O | MUX\_SPI1\_SCK | 3 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_SCK<br>0x0300\_1120 |
| GPIO | XGPIOC[2] |  | I/O | PAD\_MIPIRX4N | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
| GPIO | XGPIOC[3] |  | I/O | PAD\_MIPIRX4P | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
| GPIO | XGPIOC[4] |  | I/O | PAD\_MIPIRX3N | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3N<br>0x0300\_1174 |
| GPIO | XGPIOC[5] |  | I/O | PAD\_MIPIRX3P | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3P<br>0x0300\_1178 |
| GPIO | XGPIOC[6] |  | I/O | PAD\_MIPIRX2N | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2N<br>0x0300\_117C |
| GPIO | XGPIOC[7] |  | I/O | PAD\_MIPIRX2P | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2P<br>0x0300\_1180 |
| GPIO | XGPIOC[8] |  | I/O | PAD\_MIPIRX1N | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1N<br>0x0300\_1184 |
| GPIO | XGPIOC[9] |  | I/O | PAD\_MIPIRX1P | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1P<br>0x0300\_1188 |
| GPIO | XGPIOC[10] |  | I/O | PAD\_MIPIRX0N | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0N<br>0x0300\_118C |
| GPIO | XGPIOC[11] |  | I/O | PAD\_MIPIRX0P | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0P<br>0x0300\_1190 |
| GPIO | XGPIOC[12] |  | I/O | PAD\_MIPI\_TXM0 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 |
| GPIO | XGPIOC[13] |  | I/O | PAD\_MIPI\_TXP0 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 |
| GPIO | XGPIOC[14] |  | I/O | PAD\_MIPI\_TXM1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| GPIO | XGPIOC[15] |  | I/O | PAD\_MIPI\_TXP1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| GPIO | XGPIOC[16] |  | I/O | PAD\_MIPI\_TXM2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM2<br>0x0300\_11A4 |
| GPIO | XGPIOC[17] |  | I/O | PAD\_MIPI\_TXP2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP2<br>0x0300\_11A8 |
| GPIO | XGPIOC[23] |  | I/O | PAD\_AUD\_AINL\_MIC | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AINL\_MIC<br>0x0300\_11BC |
| GPIO | XGPIOC[24] |  | I/O | PAD\_AUD\_AOUTR | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_AUD\_AOUTR<br>0x0300\_11C8 |
| No die domain GPIO | PWR\_GPIO[0] |  | I/O | PWR\_GPIO0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_GPIO0<br>0x0300\_10A4 |
| No die domain GPIO | PWR\_GPIO[3] |  | I/O | PWR\_SEQ1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_SEQ1<br>0x0300\_1084 |
| No die domain GPIO | PWR\_GPIO[4] |  | I/O | PWR\_SEQ2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_SEQ2<br>0x0300\_1088 |
| No die domain GPIO | PWR\_GPIO[6]( |  | I/O | PWR\_WAKEUP0 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_WAKEUP0<br>0x0300\_1090 |
| No die domain GPIO | PWR\_GPIO[8] |  | I/O | PWR\_BUTTON1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_PWR\_BUTTON1<br>0x0300\_1098 |
| No die domain GPIO | PWR\_GPIO[18] |  | I/O | SD1\_D3 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D3<br>0x0300\_10D0 |
| No die domain GPIO | PWR\_GPIO[19] |  | I/O | SD1\_D2 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D2<br>0x0300\_10D4 |
| No die domain GPIO | PWR\_GPIO[20] |  | I/O | SD1\_D1 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D1<br>0x0300\_10D8 |
| No die domain GPIO | PWR\_GPIO[21] |  | I/O | SD1\_D0 | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_D0<br>0x0300\_10DC |
| No die domain GPIO | PWR\_GPIO[22] |  | I/O | SD1\_CMD | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CMD<br>0x0300\_10E0 |
| No die domain GPIO | PWR\_GPIO[23] |  | I/O | SD1\_CLK | 3 | FMUX\_GPIO\_REG\_IOCTRL\_SD1\_CLK<br>0x0300\_10E4 |
| No die domain GPIO | PWR\_GPIO[24] |  | I/O | GPIO\_ZQ\_\_\_PAD\_ZQ | 3 | FMUX\_GPIO\_REG\_IOCTRL\_GPIO\_ZQ<br>0x0300\_11D0 |
| Debug | DBG[0] |  | O | SD0\_CLK | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CLK<br>0x0300\_101C |
| Debug | DBG[1] |  | O | SD0\_CMD | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_CMD<br>0x0300\_1020 |
| Debug | DBG[10] |  | O | IIC0\_SCL | 7 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SCL<br>0x0300\_1070 |
| Debug | DBG[10] |  | O | PAD\_MIPIRX0N | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0N<br>0x0300\_118C |
| Debug | DBG[11] |  | O | IIC0\_SDA | 7 | FMUX\_GPIO\_REG\_IOCTRL\_IIC0\_SDA<br>0x0300\_1074 |
| Debug | DBG[11] |  | O | PAD\_MIPIRX0P | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX0P<br>0x0300\_1190 |
| Debug | DBG[12] |  | O | AUX0 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_AUX0<br>0x0300\_1078 |
| Debug | DBG[12] |  | O | PAD\_MIPI\_TXM0 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM0<br>0x0300\_11B4 |
| Debug | DBG[13] |  | O | MUX\_SPI1\_MOSI | 7 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MOSI<br>0x0300\_1118 |
| Debug | DBG[13] |  | O | PAD\_MIPI\_TXP0 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP0<br>0x0300\_11B8 |
| Debug | DBG[14] |  | O | MUX\_SPI1\_MISO | 7 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_MISO<br>0x0300\_1114 |
| Debug | DBG[14] |  | O | PAD\_MIPI\_TXM1 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXM1<br>0x0300\_11AC |
| Debug | DBG[15] |  | O | MUX\_SPI1\_SCK | 7 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_SCK<br>0x0300\_1120 |
| Debug | DBG[15] |  | O | PAD\_MIPI\_TXP1 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPI\_TXP1<br>0x0300\_11B0 |
| Debug | DBG[16] |  | O | MUX\_SPI1\_CS | 7 | FMUX\_GPIO\_REG\_IOCTRL\_MUX\_SPI1\_CS<br>0x0300\_111C |
| Debug | DBG[2] |  | O | SD0\_D0 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D0<br>0x0300\_1024 |
| Debug | DBG[3] |  | O | SD0\_D1 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D1<br>0x0300\_1028 |
| Debug | DBG[4] |  | O | SD0\_D2 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D2<br>0x0300\_102C |
| Debug | DBG[5] |  | O | SD0\_D3 | 7 | FMUX\_GPIO\_REG\_IOCTRL\_SD0\_D3<br>0x0300\_1030 |
| Debug | DBG[6] |  | O | UART0\_TX | 7 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_TX<br>0x0300\_1040 |
| Debug | DBG[6] |  | O | PAD\_MIPIRX2N | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2N<br>0x0300\_117C |
| Debug | DBG[7] |  | O | UART0\_RX | 7 | FMUX\_GPIO\_REG\_IOCTRL\_UART0\_RX<br>0x0300\_1044 |
| Debug | DBG[7] |  | O | PAD\_MIPIRX2P | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX2P<br>0x0300\_1180 |
| Debug | DBG[8] |  | O | PAD\_MIPIRX1N | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1N<br>0x0300\_1184 |
| Debug | DBG[9] |  | O | PAD\_MIPIRX1P | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX1P<br>0x0300\_1188 |
| Others | MUX\_SPI1\_CS |  | I/O | PAD\_MIPIRX4P | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4P<br>0x0300\_1170 |
| Others | MUX\_SPI1\_MISO |  | I/O | PAD\_MIPIRX3N | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3N<br>0x0300\_1174 |
| Others | MUX\_SPI1\_MOSI |  | I/O | PAD\_MIPIRX3P | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX3P<br>0x0300\_1178 |
| Others | MUX\_SPI1\_SCK |  | I/O | PAD\_MIPIRX4N | 7 | FMUX\_GPIO\_REG\_IOCTRL\_PAD\_MIPIRX4N<br>0x0300\_116C |
| Others | PKG\_TYPE0 |  | I | PKG\_TYPE0 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PKG\_TYPE0<br>0x0300\_1104 |
| Others | PKG\_TYPE1 |  | I | PKG\_TYPE1 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PKG\_TYPE1<br>0x0300\_110C |
| Others | PKG\_TYPE2 |  | I | PKG\_TYPE2 | 0 | FMUX\_GPIO\_REG\_IOCTRL\_PKG\_TYPE2<br>0x0300\_1110 |

## 3. Pin Control Register (QFN)
| Pin Num | Pin Name | Register Name | Address | Field Name | Bit | Default | Field Description (CN) | Field Description (EN) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 58 | PWM0\_BUCK | IOBLK\_G1\_REG\_PWM0\_BUCK | 0x0300\_1804 | IOBLK\_G1\_REG\_PWM0\_BUCK\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 58 | PWM0\_BUCK | IOBLK\_G1\_REG\_PWM0\_BUCK | 0x0300\_1804 | IOBLK\_G1\_REG\_PWM0\_BUCK\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 58 | PWM0\_BUCK | IOBLK\_G1\_REG\_PWM0\_BUCK | 0x0300\_1804 | IOBLK\_G1\_REG\_PWM0\_BUCK\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 58 | PWM0\_BUCK | IOBLK\_G1\_REG\_PWM0\_BUCK | 0x0300\_1804 | IOBLK\_G1\_REG\_PWM0\_BUCK\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 58 | PWM0\_BUCK | IOBLK\_G1\_REG\_PWM0\_BUCK | 0x0300\_1804 | IOBLK\_G1\_REG\_PWM0\_BUCK\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 58 | PWM0\_BUCK | IOBLK\_G1\_REG\_PWM0\_BUCK | 0x0300\_1804 | IOBLK\_G1\_REG\_PWM0\_BUCK\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 58 | PWM0\_BUCK | IOBLK\_G1\_REG\_PWM0\_BUCK | 0x0300\_1804 | IOBLK\_G1\_REG\_PWM0\_BUCK\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 58 | PWM0\_BUCK | IOBLK\_G1\_REG\_PWM0\_BUCK | 0x0300\_1804 | IOBLK\_G1\_REG\_PWM0\_BUCK\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 59 | ADC1 | IOBLK\_G1\_REG\_ADC1 | 0x0300\_1810 | IOBLK\_G1\_REG\_ADC1\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 59 | ADC1 | IOBLK\_G1\_REG\_ADC1 | 0x0300\_1810 | IOBLK\_G1\_REG\_ADC1\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 59 | ADC1 | IOBLK\_G1\_REG\_ADC1 | 0x0300\_1810 | IOBLK\_G1\_REG\_ADC1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 59 | ADC1 | IOBLK\_G1\_REG\_ADC1 | 0x0300\_1810 | IOBLK\_G1\_REG\_ADC1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 59 | ADC1 | IOBLK\_G1\_REG\_ADC1 | 0x0300\_1810 | IOBLK\_G1\_REG\_ADC1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 59 | ADC1 | IOBLK\_G1\_REG\_ADC1 | 0x0300\_1810 | IOBLK\_G1\_REG\_ADC1\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 59 | ADC1 | IOBLK\_G1\_REG\_ADC1 | 0x0300\_1810 | IOBLK\_G1\_REG\_ADC1\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 59 | ADC1 | IOBLK\_G1\_REG\_ADC1 | 0x0300\_1810 | IOBLK\_G1\_REG\_ADC1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
|  | PKG\_TYPE0 | IOBLK\_G1\_REG\_PKG\_TYPE0 | 0x0300\_181C | IOBLK\_G1\_REG\_PKG\_TYPE0\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
|  | PKG\_TYPE0 | IOBLK\_G1\_REG\_PKG\_TYPE0 | 0x0300\_181C | IOBLK\_G1\_REG\_PKG\_TYPE0\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
|  | PKG\_TYPE0 | IOBLK\_G1\_REG\_PKG\_TYPE0 | 0x0300\_181C | IOBLK\_G1\_REG\_PKG\_TYPE0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
|  | PKG\_TYPE0 | IOBLK\_G1\_REG\_PKG\_TYPE0 | 0x0300\_181C | IOBLK\_G1\_REG\_PKG\_TYPE0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
|  | PKG\_TYPE0 | IOBLK\_G1\_REG\_PKG\_TYPE0 | 0x0300\_181C | IOBLK\_G1\_REG\_PKG\_TYPE0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
|  | PKG\_TYPE0 | IOBLK\_G1\_REG\_PKG\_TYPE0 | 0x0300\_181C | IOBLK\_G1\_REG\_PKG\_TYPE0\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
|  | PKG\_TYPE0 | IOBLK\_G1\_REG\_PKG\_TYPE0 | 0x0300\_181C | IOBLK\_G1\_REG\_PKG\_TYPE0\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
|  | PKG\_TYPE0 | IOBLK\_G1\_REG\_PKG\_TYPE0 | 0x0300\_181C | IOBLK\_G1\_REG\_PKG\_TYPE0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 60 | USB\_VBUS\_DET | IOBLK\_G1\_REG\_USB\_VBUS\_DET | 0x0300\_1820 | IOBLK\_G1\_REG\_USB\_VBUS\_DET\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 60 | USB\_VBUS\_DET | IOBLK\_G1\_REG\_USB\_VBUS\_DET | 0x0300\_1820 | IOBLK\_G1\_REG\_USB\_VBUS\_DET\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 60 | USB\_VBUS\_DET | IOBLK\_G1\_REG\_USB\_VBUS\_DET | 0x0300\_1820 | IOBLK\_G1\_REG\_USB\_VBUS\_DET\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 60 | USB\_VBUS\_DET | IOBLK\_G1\_REG\_USB\_VBUS\_DET | 0x0300\_1820 | IOBLK\_G1\_REG\_USB\_VBUS\_DET\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 60 | USB\_VBUS\_DET | IOBLK\_G1\_REG\_USB\_VBUS\_DET | 0x0300\_1820 | IOBLK\_G1\_REG\_USB\_VBUS\_DET\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 60 | USB\_VBUS\_DET | IOBLK\_G1\_REG\_USB\_VBUS\_DET | 0x0300\_1820 | IOBLK\_G1\_REG\_USB\_VBUS\_DET\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 60 | USB\_VBUS\_DET | IOBLK\_G1\_REG\_USB\_VBUS\_DET | 0x0300\_1820 | IOBLK\_G1\_REG\_USB\_VBUS\_DET\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 60 | USB\_VBUS\_DET | IOBLK\_G1\_REG\_USB\_VBUS\_DET | 0x0300\_1820 | IOBLK\_G1\_REG\_USB\_VBUS\_DET\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
|  | PKG\_TYPE1 | IOBLK\_G1\_REG\_PKG\_TYPE1 | 0x0300\_1824 | IOBLK\_G1\_REG\_PKG\_TYPE1\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
|  | PKG\_TYPE1 | IOBLK\_G1\_REG\_PKG\_TYPE1 | 0x0300\_1824 | IOBLK\_G1\_REG\_PKG\_TYPE1\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
|  | PKG\_TYPE1 | IOBLK\_G1\_REG\_PKG\_TYPE1 | 0x0300\_1824 | IOBLK\_G1\_REG\_PKG\_TYPE1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
|  | PKG\_TYPE1 | IOBLK\_G1\_REG\_PKG\_TYPE1 | 0x0300\_1824 | IOBLK\_G1\_REG\_PKG\_TYPE1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
|  | PKG\_TYPE1 | IOBLK\_G1\_REG\_PKG\_TYPE1 | 0x0300\_1824 | IOBLK\_G1\_REG\_PKG\_TYPE1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
|  | PKG\_TYPE1 | IOBLK\_G1\_REG\_PKG\_TYPE1 | 0x0300\_1824 | IOBLK\_G1\_REG\_PKG\_TYPE1\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
|  | PKG\_TYPE1 | IOBLK\_G1\_REG\_PKG\_TYPE1 | 0x0300\_1824 | IOBLK\_G1\_REG\_PKG\_TYPE1\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
|  | PKG\_TYPE1 | IOBLK\_G1\_REG\_PKG\_TYPE1 | 0x0300\_1824 | IOBLK\_G1\_REG\_PKG\_TYPE1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
|  | PKG\_TYPE2 | IOBLK\_G1\_REG\_PKG\_TYPE2 | 0x0300\_1828 | IOBLK\_G1\_REG\_PKG\_TYPE2\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
|  | PKG\_TYPE2 | IOBLK\_G1\_REG\_PKG\_TYPE2 | 0x0300\_1828 | IOBLK\_G1\_REG\_PKG\_TYPE2\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
|  | PKG\_TYPE2 | IOBLK\_G1\_REG\_PKG\_TYPE2 | 0x0300\_1828 | IOBLK\_G1\_REG\_PKG\_TYPE2\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
|  | PKG\_TYPE2 | IOBLK\_G1\_REG\_PKG\_TYPE2 | 0x0300\_1828 | IOBLK\_G1\_REG\_PKG\_TYPE2\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
|  | PKG\_TYPE2 | IOBLK\_G1\_REG\_PKG\_TYPE2 | 0x0300\_1828 | IOBLK\_G1\_REG\_PKG\_TYPE2\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
|  | PKG\_TYPE2 | IOBLK\_G1\_REG\_PKG\_TYPE2 | 0x0300\_1828 | IOBLK\_G1\_REG\_PKG\_TYPE2\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
|  | PKG\_TYPE2 | IOBLK\_G1\_REG\_PKG\_TYPE2 | 0x0300\_1828 | IOBLK\_G1\_REG\_PKG\_TYPE2\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
|  | PKG\_TYPE2 | IOBLK\_G1\_REG\_PKG\_TYPE2 | 0x0300\_1828 | IOBLK\_G1\_REG\_PKG\_TYPE2\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 14 | SD0\_CD | IOBLK\_G7\_REG\_SD0\_CD | 0x0300\_1900 | IOBLK\_G7\_REG\_SD0\_CD\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 14 | SD0\_CD | IOBLK\_G7\_REG\_SD0\_CD | 0x0300\_1900 | IOBLK\_G7\_REG\_SD0\_CD\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 14 | SD0\_CD | IOBLK\_G7\_REG\_SD0\_CD | 0x0300\_1900 | IOBLK\_G7\_REG\_SD0\_CD\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 14 | SD0\_CD | IOBLK\_G7\_REG\_SD0\_CD | 0x0300\_1900 | IOBLK\_G7\_REG\_SD0\_CD\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 14 | SD0\_CD | IOBLK\_G7\_REG\_SD0\_CD | 0x0300\_1900 | IOBLK\_G7\_REG\_SD0\_CD\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 14 | SD0\_CD | IOBLK\_G7\_REG\_SD0\_CD | 0x0300\_1900 | IOBLK\_G7\_REG\_SD0\_CD\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 14 | SD0\_CD | IOBLK\_G7\_REG\_SD0\_CD | 0x0300\_1900 | IOBLK\_G7\_REG\_SD0\_CD\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 15 | SD0\_PWR\_EN | IOBLK\_G7\_REG\_SD0\_PWR\_EN | 0x0300\_1904 | IOBLK\_G7\_REG\_SD0\_PWR\_EN\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 15 | SD0\_PWR\_EN | IOBLK\_G7\_REG\_SD0\_PWR\_EN | 0x0300\_1904 | IOBLK\_G7\_REG\_SD0\_PWR\_EN\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 15 | SD0\_PWR\_EN | IOBLK\_G7\_REG\_SD0\_PWR\_EN | 0x0300\_1904 | IOBLK\_G7\_REG\_SD0\_PWR\_EN\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 15 | SD0\_PWR\_EN | IOBLK\_G7\_REG\_SD0\_PWR\_EN | 0x0300\_1904 | IOBLK\_G7\_REG\_SD0\_PWR\_EN\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 15 | SD0\_PWR\_EN | IOBLK\_G7\_REG\_SD0\_PWR\_EN | 0x0300\_1904 | IOBLK\_G7\_REG\_SD0\_PWR\_EN\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 15 | SD0\_PWR\_EN | IOBLK\_G7\_REG\_SD0\_PWR\_EN | 0x0300\_1904 | IOBLK\_G7\_REG\_SD0\_PWR\_EN\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 15 | SD0\_PWR\_EN | IOBLK\_G7\_REG\_SD0\_PWR\_EN | 0x0300\_1904 | IOBLK\_G7\_REG\_SD0\_PWR\_EN\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 17 | SPK\_EN | IOBLK\_G7\_REG\_SPK\_EN | 0x0300\_1908 | IOBLK\_G7\_REG\_SPK\_EN\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 17 | SPK\_EN | IOBLK\_G7\_REG\_SPK\_EN | 0x0300\_1908 | IOBLK\_G7\_REG\_SPK\_EN\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 17 | SPK\_EN | IOBLK\_G7\_REG\_SPK\_EN | 0x0300\_1908 | IOBLK\_G7\_REG\_SPK\_EN\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 17 | SPK\_EN | IOBLK\_G7\_REG\_SPK\_EN | 0x0300\_1908 | IOBLK\_G7\_REG\_SPK\_EN\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 17 | SPK\_EN | IOBLK\_G7\_REG\_SPK\_EN | 0x0300\_1908 | IOBLK\_G7\_REG\_SPK\_EN\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 17 | SPK\_EN | IOBLK\_G7\_REG\_SPK\_EN | 0x0300\_1908 | IOBLK\_G7\_REG\_SPK\_EN\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 17 | SPK\_EN | IOBLK\_G7\_REG\_SPK\_EN | 0x0300\_1908 | IOBLK\_G7\_REG\_SPK\_EN\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 18 | UART0\_TX | IOBLK\_G7\_REG\_UART0\_TX | 0x0300\_190C | IOBLK\_G7\_REG\_UART0\_TX\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 18 | UART0\_TX | IOBLK\_G7\_REG\_UART0\_TX | 0x0300\_190C | IOBLK\_G7\_REG\_UART0\_TX\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 18 | UART0\_TX | IOBLK\_G7\_REG\_UART0\_TX | 0x0300\_190C | IOBLK\_G7\_REG\_UART0\_TX\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 18 | UART0\_TX | IOBLK\_G7\_REG\_UART0\_TX | 0x0300\_190C | IOBLK\_G7\_REG\_UART0\_TX\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 18 | UART0\_TX | IOBLK\_G7\_REG\_UART0\_TX | 0x0300\_190C | IOBLK\_G7\_REG\_UART0\_TX\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 18 | UART0\_TX | IOBLK\_G7\_REG\_UART0\_TX | 0x0300\_190C | IOBLK\_G7\_REG\_UART0\_TX\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 18 | UART0\_TX | IOBLK\_G7\_REG\_UART0\_TX | 0x0300\_190C | IOBLK\_G7\_REG\_UART0\_TX\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 19 | UART0\_RX | IOBLK\_G7\_REG\_UART0\_RX | 0x0300\_1910 | IOBLK\_G7\_REG\_UART0\_RX\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 19 | UART0\_RX | IOBLK\_G7\_REG\_UART0\_RX | 0x0300\_1910 | IOBLK\_G7\_REG\_UART0\_RX\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 19 | UART0\_RX | IOBLK\_G7\_REG\_UART0\_RX | 0x0300\_1910 | IOBLK\_G7\_REG\_UART0\_RX\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 19 | UART0\_RX | IOBLK\_G7\_REG\_UART0\_RX | 0x0300\_1910 | IOBLK\_G7\_REG\_UART0\_RX\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 19 | UART0\_RX | IOBLK\_G7\_REG\_UART0\_RX | 0x0300\_1910 | IOBLK\_G7\_REG\_UART0\_RX\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 19 | UART0\_RX | IOBLK\_G7\_REG\_UART0\_RX | 0x0300\_1910 | IOBLK\_G7\_REG\_UART0\_RX\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 19 | UART0\_RX | IOBLK\_G7\_REG\_UART0\_RX | 0x0300\_1910 | IOBLK\_G7\_REG\_UART0\_RX\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 20 | EMMC\_DAT2 | IOBLK\_G7\_REG\_EMMC\_DAT2 | 0x0300\_1918 | IOBLK\_G7\_REG\_EMMC\_DAT2\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 20 | EMMC\_DAT2 | IOBLK\_G7\_REG\_EMMC\_DAT2 | 0x0300\_1918 | IOBLK\_G7\_REG\_EMMC\_DAT2\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 20 | EMMC\_DAT2 | IOBLK\_G7\_REG\_EMMC\_DAT2 | 0x0300\_1918 | IOBLK\_G7\_REG\_EMMC\_DAT2\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 20 | EMMC\_DAT2 | IOBLK\_G7\_REG\_EMMC\_DAT2 | 0x0300\_1918 | IOBLK\_G7\_REG\_EMMC\_DAT2\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 20 | EMMC\_DAT2 | IOBLK\_G7\_REG\_EMMC\_DAT2 | 0x0300\_1918 | IOBLK\_G7\_REG\_EMMC\_DAT2\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 20 | EMMC\_DAT2 | IOBLK\_G7\_REG\_EMMC\_DAT2 | 0x0300\_1918 | IOBLK\_G7\_REG\_EMMC\_DAT2\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 20 | EMMC\_DAT2 | IOBLK\_G7\_REG\_EMMC\_DAT2 | 0x0300\_1918 | IOBLK\_G7\_REG\_EMMC\_DAT2\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 21 | EMMC\_CLK | IOBLK\_G7\_REG\_EMMC\_CLK | 0x0300\_191C | IOBLK\_G7\_REG\_EMMC\_CLK\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 21 | EMMC\_CLK | IOBLK\_G7\_REG\_EMMC\_CLK | 0x0300\_191C | IOBLK\_G7\_REG\_EMMC\_CLK\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 21 | EMMC\_CLK | IOBLK\_G7\_REG\_EMMC\_CLK | 0x0300\_191C | IOBLK\_G7\_REG\_EMMC\_CLK\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 21 | EMMC\_CLK | IOBLK\_G7\_REG\_EMMC\_CLK | 0x0300\_191C | IOBLK\_G7\_REG\_EMMC\_CLK\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 21 | EMMC\_CLK | IOBLK\_G7\_REG\_EMMC\_CLK | 0x0300\_191C | IOBLK\_G7\_REG\_EMMC\_CLK\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 21 | EMMC\_CLK | IOBLK\_G7\_REG\_EMMC\_CLK | 0x0300\_191C | IOBLK\_G7\_REG\_EMMC\_CLK\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 21 | EMMC\_CLK | IOBLK\_G7\_REG\_EMMC\_CLK | 0x0300\_191C | IOBLK\_G7\_REG\_EMMC\_CLK\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 22 | EMMC\_DAT0 | IOBLK\_G7\_REG\_EMMC\_DAT0 | 0x0300\_1920 | IOBLK\_G7\_REG\_EMMC\_DAT0\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 22 | EMMC\_DAT0 | IOBLK\_G7\_REG\_EMMC\_DAT0 | 0x0300\_1920 | IOBLK\_G7\_REG\_EMMC\_DAT0\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 22 | EMMC\_DAT0 | IOBLK\_G7\_REG\_EMMC\_DAT0 | 0x0300\_1920 | IOBLK\_G7\_REG\_EMMC\_DAT0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 22 | EMMC\_DAT0 | IOBLK\_G7\_REG\_EMMC\_DAT0 | 0x0300\_1920 | IOBLK\_G7\_REG\_EMMC\_DAT0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 22 | EMMC\_DAT0 | IOBLK\_G7\_REG\_EMMC\_DAT0 | 0x0300\_1920 | IOBLK\_G7\_REG\_EMMC\_DAT0\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 22 | EMMC\_DAT0 | IOBLK\_G7\_REG\_EMMC\_DAT0 | 0x0300\_1920 | IOBLK\_G7\_REG\_EMMC\_DAT0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 22 | EMMC\_DAT0 | IOBLK\_G7\_REG\_EMMC\_DAT0 | 0x0300\_1920 | IOBLK\_G7\_REG\_EMMC\_DAT0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 23 | EMMC\_DAT3 | IOBLK\_G7\_REG\_EMMC\_DAT3 | 0x0300\_1924 | IOBLK\_G7\_REG\_EMMC\_DAT3\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 23 | EMMC\_DAT3 | IOBLK\_G7\_REG\_EMMC\_DAT3 | 0x0300\_1924 | IOBLK\_G7\_REG\_EMMC\_DAT3\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 23 | EMMC\_DAT3 | IOBLK\_G7\_REG\_EMMC\_DAT3 | 0x0300\_1924 | IOBLK\_G7\_REG\_EMMC\_DAT3\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 23 | EMMC\_DAT3 | IOBLK\_G7\_REG\_EMMC\_DAT3 | 0x0300\_1924 | IOBLK\_G7\_REG\_EMMC\_DAT3\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 23 | EMMC\_DAT3 | IOBLK\_G7\_REG\_EMMC\_DAT3 | 0x0300\_1924 | IOBLK\_G7\_REG\_EMMC\_DAT3\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 23 | EMMC\_DAT3 | IOBLK\_G7\_REG\_EMMC\_DAT3 | 0x0300\_1924 | IOBLK\_G7\_REG\_EMMC\_DAT3\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 23 | EMMC\_DAT3 | IOBLK\_G7\_REG\_EMMC\_DAT3 | 0x0300\_1924 | IOBLK\_G7\_REG\_EMMC\_DAT3\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 24 | EMMC\_CMD | IOBLK\_G7\_REG\_EMMC\_CMD | 0x0300\_1928 | IOBLK\_G7\_REG\_EMMC\_CMD\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 24 | EMMC\_CMD | IOBLK\_G7\_REG\_EMMC\_CMD | 0x0300\_1928 | IOBLK\_G7\_REG\_EMMC\_CMD\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 24 | EMMC\_CMD | IOBLK\_G7\_REG\_EMMC\_CMD | 0x0300\_1928 | IOBLK\_G7\_REG\_EMMC\_CMD\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 24 | EMMC\_CMD | IOBLK\_G7\_REG\_EMMC\_CMD | 0x0300\_1928 | IOBLK\_G7\_REG\_EMMC\_CMD\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 24 | EMMC\_CMD | IOBLK\_G7\_REG\_EMMC\_CMD | 0x0300\_1928 | IOBLK\_G7\_REG\_EMMC\_CMD\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 24 | EMMC\_CMD | IOBLK\_G7\_REG\_EMMC\_CMD | 0x0300\_1928 | IOBLK\_G7\_REG\_EMMC\_CMD\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 24 | EMMC\_CMD | IOBLK\_G7\_REG\_EMMC\_CMD | 0x0300\_1928 | IOBLK\_G7\_REG\_EMMC\_CMD\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 25 | EMMC\_DAT1 | IOBLK\_G7\_REG\_EMMC\_DAT1 | 0x0300\_192C | IOBLK\_G7\_REG\_EMMC\_DAT1\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 25 | EMMC\_DAT1 | IOBLK\_G7\_REG\_EMMC\_DAT1 | 0x0300\_192C | IOBLK\_G7\_REG\_EMMC\_DAT1\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 25 | EMMC\_DAT1 | IOBLK\_G7\_REG\_EMMC\_DAT1 | 0x0300\_192C | IOBLK\_G7\_REG\_EMMC\_DAT1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 25 | EMMC\_DAT1 | IOBLK\_G7\_REG\_EMMC\_DAT1 | 0x0300\_192C | IOBLK\_G7\_REG\_EMMC\_DAT1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 25 | EMMC\_DAT1 | IOBLK\_G7\_REG\_EMMC\_DAT1 | 0x0300\_192C | IOBLK\_G7\_REG\_EMMC\_DAT1\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 25 | EMMC\_DAT1 | IOBLK\_G7\_REG\_EMMC\_DAT1 | 0x0300\_192C | IOBLK\_G7\_REG\_EMMC\_DAT1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 25 | EMMC\_DAT1 | IOBLK\_G7\_REG\_EMMC\_DAT1 | 0x0300\_192C | IOBLK\_G7\_REG\_EMMC\_DAT1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 26 | JTAG\_CPU\_TMS | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS | 0x0300\_1930 | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 26 | JTAG\_CPU\_TMS | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS | 0x0300\_1930 | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 26 | JTAG\_CPU\_TMS | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS | 0x0300\_1930 | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 26 | JTAG\_CPU\_TMS | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS | 0x0300\_1930 | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 26 | JTAG\_CPU\_TMS | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS | 0x0300\_1930 | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 26 | JTAG\_CPU\_TMS | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS | 0x0300\_1930 | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 26 | JTAG\_CPU\_TMS | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS | 0x0300\_1930 | IOBLK\_G7\_REG\_JTAG\_CPU\_TMS\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 27 | JTAG\_CPU\_TCK | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK | 0x0300\_1934 | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 27 | JTAG\_CPU\_TCK | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK | 0x0300\_1934 | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 27 | JTAG\_CPU\_TCK | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK | 0x0300\_1934 | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 27 | JTAG\_CPU\_TCK | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK | 0x0300\_1934 | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 27 | JTAG\_CPU\_TCK | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK | 0x0300\_1934 | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 27 | JTAG\_CPU\_TCK | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK | 0x0300\_1934 | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 27 | JTAG\_CPU\_TCK | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK | 0x0300\_1934 | IOBLK\_G7\_REG\_JTAG\_CPU\_TCK\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 28 | IIC0\_SCL | IOBLK\_G7\_REG\_IIC0\_SCL | 0x0300\_193C | IOBLK\_G7\_REG\_IIC0\_SCL\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 28 | IIC0\_SCL | IOBLK\_G7\_REG\_IIC0\_SCL | 0x0300\_193C | IOBLK\_G7\_REG\_IIC0\_SCL\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 28 | IIC0\_SCL | IOBLK\_G7\_REG\_IIC0\_SCL | 0x0300\_193C | IOBLK\_G7\_REG\_IIC0\_SCL\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 28 | IIC0\_SCL | IOBLK\_G7\_REG\_IIC0\_SCL | 0x0300\_193C | IOBLK\_G7\_REG\_IIC0\_SCL\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 28 | IIC0\_SCL | IOBLK\_G7\_REG\_IIC0\_SCL | 0x0300\_193C | IOBLK\_G7\_REG\_IIC0\_SCL\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 28 | IIC0\_SCL | IOBLK\_G7\_REG\_IIC0\_SCL | 0x0300\_193C | IOBLK\_G7\_REG\_IIC0\_SCL\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 28 | IIC0\_SCL | IOBLK\_G7\_REG\_IIC0\_SCL | 0x0300\_193C | IOBLK\_G7\_REG\_IIC0\_SCL\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 29 | IIC0\_SDA | IOBLK\_G7\_REG\_IIC0\_SDA | 0x0300\_1940 | IOBLK\_G7\_REG\_IIC0\_SDA\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 29 | IIC0\_SDA | IOBLK\_G7\_REG\_IIC0\_SDA | 0x0300\_1940 | IOBLK\_G7\_REG\_IIC0\_SDA\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 29 | IIC0\_SDA | IOBLK\_G7\_REG\_IIC0\_SDA | 0x0300\_1940 | IOBLK\_G7\_REG\_IIC0\_SDA\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 29 | IIC0\_SDA | IOBLK\_G7\_REG\_IIC0\_SDA | 0x0300\_1940 | IOBLK\_G7\_REG\_IIC0\_SDA\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 29 | IIC0\_SDA | IOBLK\_G7\_REG\_IIC0\_SDA | 0x0300\_1940 | IOBLK\_G7\_REG\_IIC0\_SDA\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 29 | IIC0\_SDA | IOBLK\_G7\_REG\_IIC0\_SDA | 0x0300\_1940 | IOBLK\_G7\_REG\_IIC0\_SDA\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 29 | IIC0\_SDA | IOBLK\_G7\_REG\_IIC0\_SDA | 0x0300\_1940 | IOBLK\_G7\_REG\_IIC0\_SDA\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 30 | AUX0 | IOBLK\_G7\_REG\_AUX0 | 0x0300\_1944 | IOBLK\_G7\_REG\_AUX0\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 30 | AUX0 | IOBLK\_G7\_REG\_AUX0 | 0x0300\_1944 | IOBLK\_G7\_REG\_AUX0\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 30 | AUX0 | IOBLK\_G7\_REG\_AUX0 | 0x0300\_1944 | IOBLK\_G7\_REG\_AUX0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 30 | AUX0 | IOBLK\_G7\_REG\_AUX0 | 0x0300\_1944 | IOBLK\_G7\_REG\_AUX0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 30 | AUX0 | IOBLK\_G7\_REG\_AUX0 | 0x0300\_1944 | IOBLK\_G7\_REG\_AUX0\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 30 | AUX0 | IOBLK\_G7\_REG\_AUX0 | 0x0300\_1944 | IOBLK\_G7\_REG\_AUX0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 30 | AUX0 | IOBLK\_G7\_REG\_AUX0 | 0x0300\_1944 | IOBLK\_G7\_REG\_AUX0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 6 | SD0\_CLK | IOBLK\_G10\_REG\_SD0\_CLK | 0x0300\_1A00 | IOBLK\_G10\_REG\_SD0\_CLK\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 6 | SD0\_CLK | IOBLK\_G10\_REG\_SD0\_CLK | 0x0300\_1A00 | IOBLK\_G10\_REG\_SD0\_CLK\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 6 | SD0\_CLK | IOBLK\_G10\_REG\_SD0\_CLK | 0x0300\_1A00 | IOBLK\_G10\_REG\_SD0\_CLK\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 6 | SD0\_CLK | IOBLK\_G10\_REG\_SD0\_CLK | 0x0300\_1A00 | IOBLK\_G10\_REG\_SD0\_CLK\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 6 | SD0\_CLK | IOBLK\_G10\_REG\_SD0\_CLK | 0x0300\_1A00 | IOBLK\_G10\_REG\_SD0\_CLK\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 6 | SD0\_CLK | IOBLK\_G10\_REG\_SD0\_CLK | 0x0300\_1A00 | IOBLK\_G10\_REG\_SD0\_CLK\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 6 | SD0\_CLK | IOBLK\_G10\_REG\_SD0\_CLK | 0x0300\_1A00 | IOBLK\_G10\_REG\_SD0\_CLK\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 7 | SD0\_CMD | IOBLK\_G10\_REG\_SD0\_CMD | 0x0300\_1A04 | IOBLK\_G10\_REG\_SD0\_CMD\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 7 | SD0\_CMD | IOBLK\_G10\_REG\_SD0\_CMD | 0x0300\_1A04 | IOBLK\_G10\_REG\_SD0\_CMD\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 7 | SD0\_CMD | IOBLK\_G10\_REG\_SD0\_CMD | 0x0300\_1A04 | IOBLK\_G10\_REG\_SD0\_CMD\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 7 | SD0\_CMD | IOBLK\_G10\_REG\_SD0\_CMD | 0x0300\_1A04 | IOBLK\_G10\_REG\_SD0\_CMD\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 7 | SD0\_CMD | IOBLK\_G10\_REG\_SD0\_CMD | 0x0300\_1A04 | IOBLK\_G10\_REG\_SD0\_CMD\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 7 | SD0\_CMD | IOBLK\_G10\_REG\_SD0\_CMD | 0x0300\_1A04 | IOBLK\_G10\_REG\_SD0\_CMD\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 7 | SD0\_CMD | IOBLK\_G10\_REG\_SD0\_CMD | 0x0300\_1A04 | IOBLK\_G10\_REG\_SD0\_CMD\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 8 | SD0\_D0 | IOBLK\_G10\_REG\_SD0\_D0 | 0x0300\_1A08 | IOBLK\_G10\_REG\_SD0\_D0\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 8 | SD0\_D0 | IOBLK\_G10\_REG\_SD0\_D0 | 0x0300\_1A08 | IOBLK\_G10\_REG\_SD0\_D0\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 8 | SD0\_D0 | IOBLK\_G10\_REG\_SD0\_D0 | 0x0300\_1A08 | IOBLK\_G10\_REG\_SD0\_D0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 8 | SD0\_D0 | IOBLK\_G10\_REG\_SD0\_D0 | 0x0300\_1A08 | IOBLK\_G10\_REG\_SD0\_D0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 8 | SD0\_D0 | IOBLK\_G10\_REG\_SD0\_D0 | 0x0300\_1A08 | IOBLK\_G10\_REG\_SD0\_D0\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 8 | SD0\_D0 | IOBLK\_G10\_REG\_SD0\_D0 | 0x0300\_1A08 | IOBLK\_G10\_REG\_SD0\_D0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 8 | SD0\_D0 | IOBLK\_G10\_REG\_SD0\_D0 | 0x0300\_1A08 | IOBLK\_G10\_REG\_SD0\_D0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 10 | SD0\_D1 | IOBLK\_G10\_REG\_SD0\_D1 | 0x0300\_1A0C | IOBLK\_G10\_REG\_SD0\_D1\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 10 | SD0\_D1 | IOBLK\_G10\_REG\_SD0\_D1 | 0x0300\_1A0C | IOBLK\_G10\_REG\_SD0\_D1\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 10 | SD0\_D1 | IOBLK\_G10\_REG\_SD0\_D1 | 0x0300\_1A0C | IOBLK\_G10\_REG\_SD0\_D1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 10 | SD0\_D1 | IOBLK\_G10\_REG\_SD0\_D1 | 0x0300\_1A0C | IOBLK\_G10\_REG\_SD0\_D1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 10 | SD0\_D1 | IOBLK\_G10\_REG\_SD0\_D1 | 0x0300\_1A0C | IOBLK\_G10\_REG\_SD0\_D1\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 10 | SD0\_D1 | IOBLK\_G10\_REG\_SD0\_D1 | 0x0300\_1A0C | IOBLK\_G10\_REG\_SD0\_D1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 10 | SD0\_D1 | IOBLK\_G10\_REG\_SD0\_D1 | 0x0300\_1A0C | IOBLK\_G10\_REG\_SD0\_D1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 11 | SD0\_D2 | IOBLK\_G10\_REG\_SD0\_D2 | 0x0300\_1A10 | IOBLK\_G10\_REG\_SD0\_D2\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 11 | SD0\_D2 | IOBLK\_G10\_REG\_SD0\_D2 | 0x0300\_1A10 | IOBLK\_G10\_REG\_SD0\_D2\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 11 | SD0\_D2 | IOBLK\_G10\_REG\_SD0\_D2 | 0x0300\_1A10 | IOBLK\_G10\_REG\_SD0\_D2\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 11 | SD0\_D2 | IOBLK\_G10\_REG\_SD0\_D2 | 0x0300\_1A10 | IOBLK\_G10\_REG\_SD0\_D2\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 11 | SD0\_D2 | IOBLK\_G10\_REG\_SD0\_D2 | 0x0300\_1A10 | IOBLK\_G10\_REG\_SD0\_D2\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 11 | SD0\_D2 | IOBLK\_G10\_REG\_SD0\_D2 | 0x0300\_1A10 | IOBLK\_G10\_REG\_SD0\_D2\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 11 | SD0\_D2 | IOBLK\_G10\_REG\_SD0\_D2 | 0x0300\_1A10 | IOBLK\_G10\_REG\_SD0\_D2\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 12 | SD0\_D3 | IOBLK\_G10\_REG\_SD0\_D3 | 0x0300\_1A14 | IOBLK\_G10\_REG\_SD0\_D3\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 12 | SD0\_D3 | IOBLK\_G10\_REG\_SD0\_D3 | 0x0300\_1A14 | IOBLK\_G10\_REG\_SD0\_D3\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 12 | SD0\_D3 | IOBLK\_G10\_REG\_SD0\_D3 | 0x0300\_1A14 | IOBLK\_G10\_REG\_SD0\_D3\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 12 | SD0\_D3 | IOBLK\_G10\_REG\_SD0\_D3 | 0x0300\_1A14 | IOBLK\_G10\_REG\_SD0\_D3\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 12 | SD0\_D3 | IOBLK\_G10\_REG\_SD0\_D3 | 0x0300\_1A14 | IOBLK\_G10\_REG\_SD0\_D3\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 12 | SD0\_D3 | IOBLK\_G10\_REG\_SD0\_D3 | 0x0300\_1A14 | IOBLK\_G10\_REG\_SD0\_D3\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 12 | SD0\_D3 | IOBLK\_G10\_REG\_SD0\_D3 | 0x0300\_1A14 | IOBLK\_G10\_REG\_SD0\_D3\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 72 | PAD\_MIPIRX4N | IOBLK\_G12\_REG\_PAD\_MIPIRX4N | 0x0300\_1C38 | IOBLK\_G12\_REG\_PAD\_MIPIRX4N\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 72 | PAD\_MIPIRX4N | IOBLK\_G12\_REG\_PAD\_MIPIRX4N | 0x0300\_1C38 | IOBLK\_G12\_REG\_PAD\_MIPIRX4N\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 72 | PAD\_MIPIRX4N | IOBLK\_G12\_REG\_PAD\_MIPIRX4N | 0x0300\_1C38 | IOBLK\_G12\_REG\_PAD\_MIPIRX4N\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 72 | PAD\_MIPIRX4N | IOBLK\_G12\_REG\_PAD\_MIPIRX4N | 0x0300\_1C38 | IOBLK\_G12\_REG\_PAD\_MIPIRX4N\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 72 | PAD\_MIPIRX4N | IOBLK\_G12\_REG\_PAD\_MIPIRX4N | 0x0300\_1C38 | IOBLK\_G12\_REG\_PAD\_MIPIRX4N\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 72 | PAD\_MIPIRX4N | IOBLK\_G12\_REG\_PAD\_MIPIRX4N | 0x0300\_1C38 | IOBLK\_G12\_REG\_PAD\_MIPIRX4N\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 72 | PAD\_MIPIRX4N | IOBLK\_G12\_REG\_PAD\_MIPIRX4N | 0x0300\_1C38 | IOBLK\_G12\_REG\_PAD\_MIPIRX4N\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 72 | PAD\_MIPIRX4N | IOBLK\_G12\_REG\_PAD\_MIPIRX4N | 0x0300\_1C38 | IOBLK\_G12\_REG\_PAD\_MIPIRX4N\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 73 | PAD\_MIPIRX4P | IOBLK\_G12\_REG\_PAD\_MIPIRX4P | 0x0300\_1C3C | IOBLK\_G12\_REG\_PAD\_MIPIRX4P\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 73 | PAD\_MIPIRX4P | IOBLK\_G12\_REG\_PAD\_MIPIRX4P | 0x0300\_1C3C | IOBLK\_G12\_REG\_PAD\_MIPIRX4P\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 73 | PAD\_MIPIRX4P | IOBLK\_G12\_REG\_PAD\_MIPIRX4P | 0x0300\_1C3C | IOBLK\_G12\_REG\_PAD\_MIPIRX4P\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 73 | PAD\_MIPIRX4P | IOBLK\_G12\_REG\_PAD\_MIPIRX4P | 0x0300\_1C3C | IOBLK\_G12\_REG\_PAD\_MIPIRX4P\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 73 | PAD\_MIPIRX4P | IOBLK\_G12\_REG\_PAD\_MIPIRX4P | 0x0300\_1C3C | IOBLK\_G12\_REG\_PAD\_MIPIRX4P\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 73 | PAD\_MIPIRX4P | IOBLK\_G12\_REG\_PAD\_MIPIRX4P | 0x0300\_1C3C | IOBLK\_G12\_REG\_PAD\_MIPIRX4P\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 73 | PAD\_MIPIRX4P | IOBLK\_G12\_REG\_PAD\_MIPIRX4P | 0x0300\_1C3C | IOBLK\_G12\_REG\_PAD\_MIPIRX4P\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 73 | PAD\_MIPIRX4P | IOBLK\_G12\_REG\_PAD\_MIPIRX4P | 0x0300\_1C3C | IOBLK\_G12\_REG\_PAD\_MIPIRX4P\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 74 | PAD\_MIPIRX3N | IOBLK\_G12\_REG\_PAD\_MIPIRX3N | 0x0300\_1C40 | IOBLK\_G12\_REG\_PAD\_MIPIRX3N\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 74 | PAD\_MIPIRX3N | IOBLK\_G12\_REG\_PAD\_MIPIRX3N | 0x0300\_1C40 | IOBLK\_G12\_REG\_PAD\_MIPIRX3N\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 74 | PAD\_MIPIRX3N | IOBLK\_G12\_REG\_PAD\_MIPIRX3N | 0x0300\_1C40 | IOBLK\_G12\_REG\_PAD\_MIPIRX3N\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 74 | PAD\_MIPIRX3N | IOBLK\_G12\_REG\_PAD\_MIPIRX3N | 0x0300\_1C40 | IOBLK\_G12\_REG\_PAD\_MIPIRX3N\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 74 | PAD\_MIPIRX3N | IOBLK\_G12\_REG\_PAD\_MIPIRX3N | 0x0300\_1C40 | IOBLK\_G12\_REG\_PAD\_MIPIRX3N\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 74 | PAD\_MIPIRX3N | IOBLK\_G12\_REG\_PAD\_MIPIRX3N | 0x0300\_1C40 | IOBLK\_G12\_REG\_PAD\_MIPIRX3N\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 74 | PAD\_MIPIRX3N | IOBLK\_G12\_REG\_PAD\_MIPIRX3N | 0x0300\_1C40 | IOBLK\_G12\_REG\_PAD\_MIPIRX3N\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 74 | PAD\_MIPIRX3N | IOBLK\_G12\_REG\_PAD\_MIPIRX3N | 0x0300\_1C40 | IOBLK\_G12\_REG\_PAD\_MIPIRX3N\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 75 | PAD\_MIPIRX3P | IOBLK\_G12\_REG\_PAD\_MIPIRX3P | 0x0300\_1C44 | IOBLK\_G12\_REG\_PAD\_MIPIRX3P\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 75 | PAD\_MIPIRX3P | IOBLK\_G12\_REG\_PAD\_MIPIRX3P | 0x0300\_1C44 | IOBLK\_G12\_REG\_PAD\_MIPIRX3P\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 75 | PAD\_MIPIRX3P | IOBLK\_G12\_REG\_PAD\_MIPIRX3P | 0x0300\_1C44 | IOBLK\_G12\_REG\_PAD\_MIPIRX3P\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 75 | PAD\_MIPIRX3P | IOBLK\_G12\_REG\_PAD\_MIPIRX3P | 0x0300\_1C44 | IOBLK\_G12\_REG\_PAD\_MIPIRX3P\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 75 | PAD\_MIPIRX3P | IOBLK\_G12\_REG\_PAD\_MIPIRX3P | 0x0300\_1C44 | IOBLK\_G12\_REG\_PAD\_MIPIRX3P\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 75 | PAD\_MIPIRX3P | IOBLK\_G12\_REG\_PAD\_MIPIRX3P | 0x0300\_1C44 | IOBLK\_G12\_REG\_PAD\_MIPIRX3P\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 75 | PAD\_MIPIRX3P | IOBLK\_G12\_REG\_PAD\_MIPIRX3P | 0x0300\_1C44 | IOBLK\_G12\_REG\_PAD\_MIPIRX3P\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 75 | PAD\_MIPIRX3P | IOBLK\_G12\_REG\_PAD\_MIPIRX3P | 0x0300\_1C44 | IOBLK\_G12\_REG\_PAD\_MIPIRX3P\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 76 | PAD\_MIPIRX2N | IOBLK\_G12\_REG\_PAD\_MIPIRX2N | 0x0300\_1C48 | IOBLK\_G12\_REG\_PAD\_MIPIRX2N\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 76 | PAD\_MIPIRX2N | IOBLK\_G12\_REG\_PAD\_MIPIRX2N | 0x0300\_1C48 | IOBLK\_G12\_REG\_PAD\_MIPIRX2N\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 76 | PAD\_MIPIRX2N | IOBLK\_G12\_REG\_PAD\_MIPIRX2N | 0x0300\_1C48 | IOBLK\_G12\_REG\_PAD\_MIPIRX2N\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 76 | PAD\_MIPIRX2N | IOBLK\_G12\_REG\_PAD\_MIPIRX2N | 0x0300\_1C48 | IOBLK\_G12\_REG\_PAD\_MIPIRX2N\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 76 | PAD\_MIPIRX2N | IOBLK\_G12\_REG\_PAD\_MIPIRX2N | 0x0300\_1C48 | IOBLK\_G12\_REG\_PAD\_MIPIRX2N\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 76 | PAD\_MIPIRX2N | IOBLK\_G12\_REG\_PAD\_MIPIRX2N | 0x0300\_1C48 | IOBLK\_G12\_REG\_PAD\_MIPIRX2N\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 76 | PAD\_MIPIRX2N | IOBLK\_G12\_REG\_PAD\_MIPIRX2N | 0x0300\_1C48 | IOBLK\_G12\_REG\_PAD\_MIPIRX2N\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 76 | PAD\_MIPIRX2N | IOBLK\_G12\_REG\_PAD\_MIPIRX2N | 0x0300\_1C48 | IOBLK\_G12\_REG\_PAD\_MIPIRX2N\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 77 | PAD\_MIPIRX2P | IOBLK\_G12\_REG\_PAD\_MIPIRX2P | 0x0300\_1C4C | IOBLK\_G12\_REG\_PAD\_MIPIRX2P\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 77 | PAD\_MIPIRX2P | IOBLK\_G12\_REG\_PAD\_MIPIRX2P | 0x0300\_1C4C | IOBLK\_G12\_REG\_PAD\_MIPIRX2P\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 77 | PAD\_MIPIRX2P | IOBLK\_G12\_REG\_PAD\_MIPIRX2P | 0x0300\_1C4C | IOBLK\_G12\_REG\_PAD\_MIPIRX2P\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 77 | PAD\_MIPIRX2P | IOBLK\_G12\_REG\_PAD\_MIPIRX2P | 0x0300\_1C4C | IOBLK\_G12\_REG\_PAD\_MIPIRX2P\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 77 | PAD\_MIPIRX2P | IOBLK\_G12\_REG\_PAD\_MIPIRX2P | 0x0300\_1C4C | IOBLK\_G12\_REG\_PAD\_MIPIRX2P\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 77 | PAD\_MIPIRX2P | IOBLK\_G12\_REG\_PAD\_MIPIRX2P | 0x0300\_1C4C | IOBLK\_G12\_REG\_PAD\_MIPIRX2P\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 77 | PAD\_MIPIRX2P | IOBLK\_G12\_REG\_PAD\_MIPIRX2P | 0x0300\_1C4C | IOBLK\_G12\_REG\_PAD\_MIPIRX2P\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 77 | PAD\_MIPIRX2P | IOBLK\_G12\_REG\_PAD\_MIPIRX2P | 0x0300\_1C4C | IOBLK\_G12\_REG\_PAD\_MIPIRX2P\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 78 | PAD\_MIPIRX1N | IOBLK\_G12\_REG\_PAD\_MIPIRX1N | 0x0300\_1C50 | IOBLK\_G12\_REG\_PAD\_MIPIRX1N\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 78 | PAD\_MIPIRX1N | IOBLK\_G12\_REG\_PAD\_MIPIRX1N | 0x0300\_1C50 | IOBLK\_G12\_REG\_PAD\_MIPIRX1N\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 78 | PAD\_MIPIRX1N | IOBLK\_G12\_REG\_PAD\_MIPIRX1N | 0x0300\_1C50 | IOBLK\_G12\_REG\_PAD\_MIPIRX1N\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 78 | PAD\_MIPIRX1N | IOBLK\_G12\_REG\_PAD\_MIPIRX1N | 0x0300\_1C50 | IOBLK\_G12\_REG\_PAD\_MIPIRX1N\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 78 | PAD\_MIPIRX1N | IOBLK\_G12\_REG\_PAD\_MIPIRX1N | 0x0300\_1C50 | IOBLK\_G12\_REG\_PAD\_MIPIRX1N\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 78 | PAD\_MIPIRX1N | IOBLK\_G12\_REG\_PAD\_MIPIRX1N | 0x0300\_1C50 | IOBLK\_G12\_REG\_PAD\_MIPIRX1N\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 78 | PAD\_MIPIRX1N | IOBLK\_G12\_REG\_PAD\_MIPIRX1N | 0x0300\_1C50 | IOBLK\_G12\_REG\_PAD\_MIPIRX1N\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 78 | PAD\_MIPIRX1N | IOBLK\_G12\_REG\_PAD\_MIPIRX1N | 0x0300\_1C50 | IOBLK\_G12\_REG\_PAD\_MIPIRX1N\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 79 | PAD\_MIPIRX1P | IOBLK\_G12\_REG\_PAD\_MIPIRX1P | 0x0300\_1C54 | IOBLK\_G12\_REG\_PAD\_MIPIRX1P\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 79 | PAD\_MIPIRX1P | IOBLK\_G12\_REG\_PAD\_MIPIRX1P | 0x0300\_1C54 | IOBLK\_G12\_REG\_PAD\_MIPIRX1P\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 79 | PAD\_MIPIRX1P | IOBLK\_G12\_REG\_PAD\_MIPIRX1P | 0x0300\_1C54 | IOBLK\_G12\_REG\_PAD\_MIPIRX1P\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 79 | PAD\_MIPIRX1P | IOBLK\_G12\_REG\_PAD\_MIPIRX1P | 0x0300\_1C54 | IOBLK\_G12\_REG\_PAD\_MIPIRX1P\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 79 | PAD\_MIPIRX1P | IOBLK\_G12\_REG\_PAD\_MIPIRX1P | 0x0300\_1C54 | IOBLK\_G12\_REG\_PAD\_MIPIRX1P\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 79 | PAD\_MIPIRX1P | IOBLK\_G12\_REG\_PAD\_MIPIRX1P | 0x0300\_1C54 | IOBLK\_G12\_REG\_PAD\_MIPIRX1P\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 79 | PAD\_MIPIRX1P | IOBLK\_G12\_REG\_PAD\_MIPIRX1P | 0x0300\_1C54 | IOBLK\_G12\_REG\_PAD\_MIPIRX1P\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 79 | PAD\_MIPIRX1P | IOBLK\_G12\_REG\_PAD\_MIPIRX1P | 0x0300\_1C54 | IOBLK\_G12\_REG\_PAD\_MIPIRX1P\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 80 | PAD\_MIPIRX0N | IOBLK\_G12\_REG\_PAD\_MIPIRX0N | 0x0300\_1C58 | IOBLK\_G12\_REG\_PAD\_MIPIRX0N\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 80 | PAD\_MIPIRX0N | IOBLK\_G12\_REG\_PAD\_MIPIRX0N | 0x0300\_1C58 | IOBLK\_G12\_REG\_PAD\_MIPIRX0N\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 80 | PAD\_MIPIRX0N | IOBLK\_G12\_REG\_PAD\_MIPIRX0N | 0x0300\_1C58 | IOBLK\_G12\_REG\_PAD\_MIPIRX0N\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 80 | PAD\_MIPIRX0N | IOBLK\_G12\_REG\_PAD\_MIPIRX0N | 0x0300\_1C58 | IOBLK\_G12\_REG\_PAD\_MIPIRX0N\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 80 | PAD\_MIPIRX0N | IOBLK\_G12\_REG\_PAD\_MIPIRX0N | 0x0300\_1C58 | IOBLK\_G12\_REG\_PAD\_MIPIRX0N\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 80 | PAD\_MIPIRX0N | IOBLK\_G12\_REG\_PAD\_MIPIRX0N | 0x0300\_1C58 | IOBLK\_G12\_REG\_PAD\_MIPIRX0N\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 80 | PAD\_MIPIRX0N | IOBLK\_G12\_REG\_PAD\_MIPIRX0N | 0x0300\_1C58 | IOBLK\_G12\_REG\_PAD\_MIPIRX0N\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 80 | PAD\_MIPIRX0N | IOBLK\_G12\_REG\_PAD\_MIPIRX0N | 0x0300\_1C58 | IOBLK\_G12\_REG\_PAD\_MIPIRX0N\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 81 | PAD\_MIPIRX0P | IOBLK\_G12\_REG\_PAD\_MIPIRX0P | 0x0300\_1C5C | IOBLK\_G12\_REG\_PAD\_MIPIRX0P\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 81 | PAD\_MIPIRX0P | IOBLK\_G12\_REG\_PAD\_MIPIRX0P | 0x0300\_1C5C | IOBLK\_G12\_REG\_PAD\_MIPIRX0P\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 81 | PAD\_MIPIRX0P | IOBLK\_G12\_REG\_PAD\_MIPIRX0P | 0x0300\_1C5C | IOBLK\_G12\_REG\_PAD\_MIPIRX0P\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 81 | PAD\_MIPIRX0P | IOBLK\_G12\_REG\_PAD\_MIPIRX0P | 0x0300\_1C5C | IOBLK\_G12\_REG\_PAD\_MIPIRX0P\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 81 | PAD\_MIPIRX0P | IOBLK\_G12\_REG\_PAD\_MIPIRX0P | 0x0300\_1C5C | IOBLK\_G12\_REG\_PAD\_MIPIRX0P\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 81 | PAD\_MIPIRX0P | IOBLK\_G12\_REG\_PAD\_MIPIRX0P | 0x0300\_1C5C | IOBLK\_G12\_REG\_PAD\_MIPIRX0P\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 81 | PAD\_MIPIRX0P | IOBLK\_G12\_REG\_PAD\_MIPIRX0P | 0x0300\_1C5C | IOBLK\_G12\_REG\_PAD\_MIPIRX0P\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 81 | PAD\_MIPIRX0P | IOBLK\_G12\_REG\_PAD\_MIPIRX0P | 0x0300\_1C5C | IOBLK\_G12\_REG\_PAD\_MIPIRX0P\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 83 | PAD\_MIPI\_TXM2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2 | 0x0300\_1C70 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 83 | PAD\_MIPI\_TXM2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2 | 0x0300\_1C70 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 83 | PAD\_MIPI\_TXM2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2 | 0x0300\_1C70 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 83 | PAD\_MIPI\_TXM2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2 | 0x0300\_1C70 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 83 | PAD\_MIPI\_TXM2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2 | 0x0300\_1C70 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 83 | PAD\_MIPI\_TXM2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2 | 0x0300\_1C70 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 83 | PAD\_MIPI\_TXM2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2 | 0x0300\_1C70 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 83 | PAD\_MIPI\_TXM2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2 | 0x0300\_1C70 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM2\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 84 | PAD\_MIPI\_TXP2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2 | 0x0300\_1C74 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 84 | PAD\_MIPI\_TXP2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2 | 0x0300\_1C74 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 84 | PAD\_MIPI\_TXP2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2 | 0x0300\_1C74 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 84 | PAD\_MIPI\_TXP2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2 | 0x0300\_1C74 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 84 | PAD\_MIPI\_TXP2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2 | 0x0300\_1C74 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 84 | PAD\_MIPI\_TXP2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2 | 0x0300\_1C74 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 84 | PAD\_MIPI\_TXP2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2 | 0x0300\_1C74 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 84 | PAD\_MIPI\_TXP2 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2 | 0x0300\_1C74 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP2\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 85 | PAD\_MIPI\_TXM1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1 | 0x0300\_1C78 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 85 | PAD\_MIPI\_TXM1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1 | 0x0300\_1C78 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 85 | PAD\_MIPI\_TXM1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1 | 0x0300\_1C78 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 85 | PAD\_MIPI\_TXM1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1 | 0x0300\_1C78 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 85 | PAD\_MIPI\_TXM1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1 | 0x0300\_1C78 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 85 | PAD\_MIPI\_TXM1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1 | 0x0300\_1C78 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 85 | PAD\_MIPI\_TXM1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1 | 0x0300\_1C78 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 85 | PAD\_MIPI\_TXM1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1 | 0x0300\_1C78 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 86 | PAD\_MIPI\_TXP1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1 | 0x0300\_1C7C | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 86 | PAD\_MIPI\_TXP1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1 | 0x0300\_1C7C | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 86 | PAD\_MIPI\_TXP1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1 | 0x0300\_1C7C | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 86 | PAD\_MIPI\_TXP1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1 | 0x0300\_1C7C | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 86 | PAD\_MIPI\_TXP1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1 | 0x0300\_1C7C | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 86 | PAD\_MIPI\_TXP1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1 | 0x0300\_1C7C | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 86 | PAD\_MIPI\_TXP1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1 | 0x0300\_1C7C | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 86 | PAD\_MIPI\_TXP1 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1 | 0x0300\_1C7C | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 87 | PAD\_MIPI\_TXM0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0 | 0x0300\_1C80 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 87 | PAD\_MIPI\_TXM0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0 | 0x0300\_1C80 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 87 | PAD\_MIPI\_TXM0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0 | 0x0300\_1C80 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 87 | PAD\_MIPI\_TXM0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0 | 0x0300\_1C80 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 87 | PAD\_MIPI\_TXM0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0 | 0x0300\_1C80 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 87 | PAD\_MIPI\_TXM0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0 | 0x0300\_1C80 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 87 | PAD\_MIPI\_TXM0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0 | 0x0300\_1C80 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 87 | PAD\_MIPI\_TXM0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0 | 0x0300\_1C80 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXM0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 88 | PAD\_MIPI\_TXP0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0 | 0x0300\_1C84 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 88 | PAD\_MIPI\_TXP0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0 | 0x0300\_1C84 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 88 | PAD\_MIPI\_TXP0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0 | 0x0300\_1C84 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 88 | PAD\_MIPI\_TXP0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0 | 0x0300\_1C84 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 88 | PAD\_MIPI\_TXP0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0 | 0x0300\_1C84 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 88 | PAD\_MIPI\_TXP0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0 | 0x0300\_1C84 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 88 | PAD\_MIPI\_TXP0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0 | 0x0300\_1C84 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 88 | PAD\_MIPI\_TXP0 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0 | 0x0300\_1C84 | IOBLK\_G12\_REG\_PAD\_MIPI\_TXP0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | IOBLK\_G12\_REG\_GPIO\_RTX | 0x0300\_1C8C | IOBLK\_G12\_REG\_GPIO\_RTX\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | IOBLK\_G12\_REG\_GPIO\_RTX | 0x0300\_1C8C | IOBLK\_G12\_REG\_GPIO\_RTX\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | IOBLK\_G12\_REG\_GPIO\_RTX | 0x0300\_1C8C | IOBLK\_G12\_REG\_GPIO\_RTX\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | IOBLK\_G12\_REG\_GPIO\_RTX | 0x0300\_1C8C | IOBLK\_G12\_REG\_GPIO\_RTX\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | IOBLK\_G12\_REG\_GPIO\_RTX | 0x0300\_1C8C | IOBLK\_G12\_REG\_GPIO\_RTX\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | IOBLK\_G12\_REG\_GPIO\_RTX | 0x0300\_1C8C | IOBLK\_G12\_REG\_GPIO\_RTX\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | IOBLK\_G12\_REG\_GPIO\_RTX | 0x0300\_1C8C | IOBLK\_G12\_REG\_GPIO\_RTX\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | IOBLK\_G12\_REG\_GPIO\_RTX | 0x0300\_1C8C | IOBLK\_G12\_REG\_GPIO\_RTX\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 38 | PWR\_VBAT\_DET | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET | 0x0502\_7000 | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 38 | PWR\_VBAT\_DET | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET | 0x0502\_7000 | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 38 | PWR\_VBAT\_DET | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET | 0x0502\_7000 | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 38 | PWR\_VBAT\_DET | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET | 0x0502\_7000 | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 38 | PWR\_VBAT\_DET | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET | 0x0502\_7000 | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 38 | PWR\_VBAT\_DET | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET | 0x0502\_7000 | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 38 | PWR\_VBAT\_DET | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET | 0x0502\_7000 | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 38 | PWR\_VBAT\_DET | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET | 0x0502\_7000 | IOBLK\_GRTC\_REG\_PWR\_VBAT\_DET\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 39 | PWR\_RSTN | IOBLK\_GRTC\_REG\_PWR\_RSTN | 0x0502\_7004 | IOBLK\_GRTC\_REG\_PWR\_RSTN\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 39 | PWR\_RSTN | IOBLK\_GRTC\_REG\_PWR\_RSTN | 0x0502\_7004 | IOBLK\_GRTC\_REG\_PWR\_RSTN\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 39 | PWR\_RSTN | IOBLK\_GRTC\_REG\_PWR\_RSTN | 0x0502\_7004 | IOBLK\_GRTC\_REG\_PWR\_RSTN\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 39 | PWR\_RSTN | IOBLK\_GRTC\_REG\_PWR\_RSTN | 0x0502\_7004 | IOBLK\_GRTC\_REG\_PWR\_RSTN\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 39 | PWR\_RSTN | IOBLK\_GRTC\_REG\_PWR\_RSTN | 0x0502\_7004 | IOBLK\_GRTC\_REG\_PWR\_RSTN\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 39 | PWR\_RSTN | IOBLK\_GRTC\_REG\_PWR\_RSTN | 0x0502\_7004 | IOBLK\_GRTC\_REG\_PWR\_RSTN\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 39 | PWR\_RSTN | IOBLK\_GRTC\_REG\_PWR\_RSTN | 0x0502\_7004 | IOBLK\_GRTC\_REG\_PWR\_RSTN\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 39 | PWR\_RSTN | IOBLK\_GRTC\_REG\_PWR\_RSTN | 0x0502\_7004 | IOBLK\_GRTC\_REG\_PWR\_RSTN\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 40 | PWR\_SEQ1 | IOBLK\_GRTC\_REG\_PWR\_SEQ1 | 0x0502\_7008 | IOBLK\_GRTC\_REG\_PWR\_SEQ1\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 40 | PWR\_SEQ1 | IOBLK\_GRTC\_REG\_PWR\_SEQ1 | 0x0502\_7008 | IOBLK\_GRTC\_REG\_PWR\_SEQ1\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 40 | PWR\_SEQ1 | IOBLK\_GRTC\_REG\_PWR\_SEQ1 | 0x0502\_7008 | IOBLK\_GRTC\_REG\_PWR\_SEQ1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 40 | PWR\_SEQ1 | IOBLK\_GRTC\_REG\_PWR\_SEQ1 | 0x0502\_7008 | IOBLK\_GRTC\_REG\_PWR\_SEQ1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 40 | PWR\_SEQ1 | IOBLK\_GRTC\_REG\_PWR\_SEQ1 | 0x0502\_7008 | IOBLK\_GRTC\_REG\_PWR\_SEQ1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 40 | PWR\_SEQ1 | IOBLK\_GRTC\_REG\_PWR\_SEQ1 | 0x0502\_7008 | IOBLK\_GRTC\_REG\_PWR\_SEQ1\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 40 | PWR\_SEQ1 | IOBLK\_GRTC\_REG\_PWR\_SEQ1 | 0x0502\_7008 | IOBLK\_GRTC\_REG\_PWR\_SEQ1\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 40 | PWR\_SEQ1 | IOBLK\_GRTC\_REG\_PWR\_SEQ1 | 0x0502\_7008 | IOBLK\_GRTC\_REG\_PWR\_SEQ1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 41 | PWR\_SEQ2 | IOBLK\_GRTC\_REG\_PWR\_SEQ2 | 0x0502\_700C | IOBLK\_GRTC\_REG\_PWR\_SEQ2\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 41 | PWR\_SEQ2 | IOBLK\_GRTC\_REG\_PWR\_SEQ2 | 0x0502\_700C | IOBLK\_GRTC\_REG\_PWR\_SEQ2\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 41 | PWR\_SEQ2 | IOBLK\_GRTC\_REG\_PWR\_SEQ2 | 0x0502\_700C | IOBLK\_GRTC\_REG\_PWR\_SEQ2\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 41 | PWR\_SEQ2 | IOBLK\_GRTC\_REG\_PWR\_SEQ2 | 0x0502\_700C | IOBLK\_GRTC\_REG\_PWR\_SEQ2\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 41 | PWR\_SEQ2 | IOBLK\_GRTC\_REG\_PWR\_SEQ2 | 0x0502\_700C | IOBLK\_GRTC\_REG\_PWR\_SEQ2\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 41 | PWR\_SEQ2 | IOBLK\_GRTC\_REG\_PWR\_SEQ2 | 0x0502\_700C | IOBLK\_GRTC\_REG\_PWR\_SEQ2\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 41 | PWR\_SEQ2 | IOBLK\_GRTC\_REG\_PWR\_SEQ2 | 0x0502\_700C | IOBLK\_GRTC\_REG\_PWR\_SEQ2\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 41 | PWR\_SEQ2 | IOBLK\_GRTC\_REG\_PWR\_SEQ2 | 0x0502\_700C | IOBLK\_GRTC\_REG\_PWR\_SEQ2\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 42 | PTEST | IOBLK\_GRTC\_REG\_PTEST | 0x0502\_7014 | IOBLK\_GRTC\_REG\_PTEST\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 42 | PTEST | IOBLK\_GRTC\_REG\_PTEST | 0x0502\_7014 | IOBLK\_GRTC\_REG\_PTEST\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 42 | PTEST | IOBLK\_GRTC\_REG\_PTEST | 0x0502\_7014 | IOBLK\_GRTC\_REG\_PTEST\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 42 | PTEST | IOBLK\_GRTC\_REG\_PTEST | 0x0502\_7014 | IOBLK\_GRTC\_REG\_PTEST\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 42 | PTEST | IOBLK\_GRTC\_REG\_PTEST | 0x0502\_7014 | IOBLK\_GRTC\_REG\_PTEST\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 42 | PTEST | IOBLK\_GRTC\_REG\_PTEST | 0x0502\_7014 | IOBLK\_GRTC\_REG\_PTEST\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 42 | PTEST | IOBLK\_GRTC\_REG\_PTEST | 0x0502\_7014 | IOBLK\_GRTC\_REG\_PTEST\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 42 | PTEST | IOBLK\_GRTC\_REG\_PTEST | 0x0502\_7014 | IOBLK\_GRTC\_REG\_PTEST\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 43 | PWR\_WAKEUP0 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0 | 0x0502\_7018 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 43 | PWR\_WAKEUP0 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0 | 0x0502\_7018 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 43 | PWR\_WAKEUP0 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0 | 0x0502\_7018 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 43 | PWR\_WAKEUP0 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0 | 0x0502\_7018 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 43 | PWR\_WAKEUP0 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0 | 0x0502\_7018 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 43 | PWR\_WAKEUP0 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0 | 0x0502\_7018 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 43 | PWR\_WAKEUP0 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0 | 0x0502\_7018 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 43 | PWR\_WAKEUP0 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0 | 0x0502\_7018 | IOBLK\_GRTC\_REG\_PWR\_WAKEUP0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 44 | PWR\_BUTTON1 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1 | 0x0502\_7020 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1\_PU | 2 | 0x1 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 44 | PWR\_BUTTON1 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1 | 0x0502\_7020 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1\_PD | 3 | 0x0 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 44 | PWR\_BUTTON1 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1 | 0x0502\_7020 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 44 | PWR\_BUTTON1 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1 | 0x0502\_7020 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 44 | PWR\_BUTTON1 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1 | 0x0502\_7020 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 44 | PWR\_BUTTON1 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1 | 0x0502\_7020 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 44 | PWR\_BUTTON1 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1 | 0x0502\_7020 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 44 | PWR\_BUTTON1 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1 | 0x0502\_7020 | IOBLK\_GRTC\_REG\_PWR\_BUTTON1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 45 | XTAL\_XIN | IOBLK\_GRTC\_REG\_XTAL\_XIN | 0x0502\_7028 | IOBLK\_GRTC\_REG\_XTAL\_XIN\_XDS0 | 14 | 0x0 | 晶振输出驱动能力档位 bit 0 | Xtal buffer Driving Strength bit 0 |
| 45 | XTAL\_XIN | IOBLK\_GRTC\_REG\_XTAL\_XIN | 0x0502\_7028 | IOBLK\_GRTC\_REG\_XTAL\_XIN\_XDS1 | 15 | 0x0 | 晶振输出驱动能力档位 bit 1 | Xtal buffer Driving Strength bit 1 |
| 45 | XTAL\_XIN | IOBLK\_GRTC\_REG\_XTAL\_XIN | 0x0502\_7028 | IOBLK\_GRTC\_REG\_XTAL\_XIN\_XDS2 | 16 | 0x1 | 晶振输出驱动能力档位 bit 2 | Xtal buffer Driving Strength bit 2 |
| 47 | PWR\_GPIO0 | IOBLK\_GRTC\_REG\_PWR\_GPIO0 | 0x0502\_702C | IOBLK\_GRTC\_REG\_PWR\_GPIO0\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 47 | PWR\_GPIO0 | IOBLK\_GRTC\_REG\_PWR\_GPIO0 | 0x0502\_702C | IOBLK\_GRTC\_REG\_PWR\_GPIO0\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 47 | PWR\_GPIO0 | IOBLK\_GRTC\_REG\_PWR\_GPIO0 | 0x0502\_702C | IOBLK\_GRTC\_REG\_PWR\_GPIO0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 47 | PWR\_GPIO0 | IOBLK\_GRTC\_REG\_PWR\_GPIO0 | 0x0502\_702C | IOBLK\_GRTC\_REG\_PWR\_GPIO0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 47 | PWR\_GPIO0 | IOBLK\_GRTC\_REG\_PWR\_GPIO0 | 0x0502\_702C | IOBLK\_GRTC\_REG\_PWR\_GPIO0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 47 | PWR\_GPIO0 | IOBLK\_GRTC\_REG\_PWR\_GPIO0 | 0x0502\_702C | IOBLK\_GRTC\_REG\_PWR\_GPIO0\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 47 | PWR\_GPIO0 | IOBLK\_GRTC\_REG\_PWR\_GPIO0 | 0x0502\_702C | IOBLK\_GRTC\_REG\_PWR\_GPIO0\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 47 | PWR\_GPIO0 | IOBLK\_GRTC\_REG\_PWR\_GPIO0 | 0x0502\_702C | IOBLK\_GRTC\_REG\_PWR\_GPIO0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 48 | PWR\_GPIO1 | IOBLK\_GRTC\_REG\_PWR\_GPIO1 | 0x0502\_7030 | IOBLK\_GRTC\_REG\_PWR\_GPIO1\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 48 | PWR\_GPIO1 | IOBLK\_GRTC\_REG\_PWR\_GPIO1 | 0x0502\_7030 | IOBLK\_GRTC\_REG\_PWR\_GPIO1\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 48 | PWR\_GPIO1 | IOBLK\_GRTC\_REG\_PWR\_GPIO1 | 0x0502\_7030 | IOBLK\_GRTC\_REG\_PWR\_GPIO1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 48 | PWR\_GPIO1 | IOBLK\_GRTC\_REG\_PWR\_GPIO1 | 0x0502\_7030 | IOBLK\_GRTC\_REG\_PWR\_GPIO1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 48 | PWR\_GPIO1 | IOBLK\_GRTC\_REG\_PWR\_GPIO1 | 0x0502\_7030 | IOBLK\_GRTC\_REG\_PWR\_GPIO1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 48 | PWR\_GPIO1 | IOBLK\_GRTC\_REG\_PWR\_GPIO1 | 0x0502\_7030 | IOBLK\_GRTC\_REG\_PWR\_GPIO1\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 48 | PWR\_GPIO1 | IOBLK\_GRTC\_REG\_PWR\_GPIO1 | 0x0502\_7030 | IOBLK\_GRTC\_REG\_PWR\_GPIO1\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 48 | PWR\_GPIO1 | IOBLK\_GRTC\_REG\_PWR\_GPIO1 | 0x0502\_7030 | IOBLK\_GRTC\_REG\_PWR\_GPIO1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 49 | PWR\_GPIO2 | IOBLK\_GRTC\_REG\_PWR\_GPIO2 | 0x0502\_7034 | IOBLK\_GRTC\_REG\_PWR\_GPIO2\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 49 | PWR\_GPIO2 | IOBLK\_GRTC\_REG\_PWR\_GPIO2 | 0x0502\_7034 | IOBLK\_GRTC\_REG\_PWR\_GPIO2\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 49 | PWR\_GPIO2 | IOBLK\_GRTC\_REG\_PWR\_GPIO2 | 0x0502\_7034 | IOBLK\_GRTC\_REG\_PWR\_GPIO2\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 49 | PWR\_GPIO2 | IOBLK\_GRTC\_REG\_PWR\_GPIO2 | 0x0502\_7034 | IOBLK\_GRTC\_REG\_PWR\_GPIO2\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 49 | PWR\_GPIO2 | IOBLK\_GRTC\_REG\_PWR\_GPIO2 | 0x0502\_7034 | IOBLK\_GRTC\_REG\_PWR\_GPIO2\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 49 | PWR\_GPIO2 | IOBLK\_GRTC\_REG\_PWR\_GPIO2 | 0x0502\_7034 | IOBLK\_GRTC\_REG\_PWR\_GPIO2\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 49 | PWR\_GPIO2 | IOBLK\_GRTC\_REG\_PWR\_GPIO2 | 0x0502\_7034 | IOBLK\_GRTC\_REG\_PWR\_GPIO2\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 49 | PWR\_GPIO2 | IOBLK\_GRTC\_REG\_PWR\_GPIO2 | 0x0502\_7034 | IOBLK\_GRTC\_REG\_PWR\_GPIO2\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 51 | SD1\_D3 | IOBLK\_GRTC\_REG\_SD1\_D3 | 0x0502\_7058 | IOBLK\_GRTC\_REG\_SD1\_D3\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 51 | SD1\_D3 | IOBLK\_GRTC\_REG\_SD1\_D3 | 0x0502\_7058 | IOBLK\_GRTC\_REG\_SD1\_D3\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 51 | SD1\_D3 | IOBLK\_GRTC\_REG\_SD1\_D3 | 0x0502\_7058 | IOBLK\_GRTC\_REG\_SD1\_D3\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 51 | SD1\_D3 | IOBLK\_GRTC\_REG\_SD1\_D3 | 0x0502\_7058 | IOBLK\_GRTC\_REG\_SD1\_D3\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 51 | SD1\_D3 | IOBLK\_GRTC\_REG\_SD1\_D3 | 0x0502\_7058 | IOBLK\_GRTC\_REG\_SD1\_D3\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 51 | SD1\_D3 | IOBLK\_GRTC\_REG\_SD1\_D3 | 0x0502\_7058 | IOBLK\_GRTC\_REG\_SD1\_D3\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 51 | SD1\_D3 | IOBLK\_GRTC\_REG\_SD1\_D3 | 0x0502\_7058 | IOBLK\_GRTC\_REG\_SD1\_D3\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 52 | SD1\_D2 | IOBLK\_GRTC\_REG\_SD1\_D2 | 0x0502\_705C | IOBLK\_GRTC\_REG\_SD1\_D2\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 52 | SD1\_D2 | IOBLK\_GRTC\_REG\_SD1\_D2 | 0x0502\_705C | IOBLK\_GRTC\_REG\_SD1\_D2\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 52 | SD1\_D2 | IOBLK\_GRTC\_REG\_SD1\_D2 | 0x0502\_705C | IOBLK\_GRTC\_REG\_SD1\_D2\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 52 | SD1\_D2 | IOBLK\_GRTC\_REG\_SD1\_D2 | 0x0502\_705C | IOBLK\_GRTC\_REG\_SD1\_D2\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 52 | SD1\_D2 | IOBLK\_GRTC\_REG\_SD1\_D2 | 0x0502\_705C | IOBLK\_GRTC\_REG\_SD1\_D2\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 52 | SD1\_D2 | IOBLK\_GRTC\_REG\_SD1\_D2 | 0x0502\_705C | IOBLK\_GRTC\_REG\_SD1\_D2\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 52 | SD1\_D2 | IOBLK\_GRTC\_REG\_SD1\_D2 | 0x0502\_705C | IOBLK\_GRTC\_REG\_SD1\_D2\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 53 | SD1\_D1 | IOBLK\_GRTC\_REG\_SD1\_D1 | 0x0502\_7060 | IOBLK\_GRTC\_REG\_SD1\_D1\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 53 | SD1\_D1 | IOBLK\_GRTC\_REG\_SD1\_D1 | 0x0502\_7060 | IOBLK\_GRTC\_REG\_SD1\_D1\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 53 | SD1\_D1 | IOBLK\_GRTC\_REG\_SD1\_D1 | 0x0502\_7060 | IOBLK\_GRTC\_REG\_SD1\_D1\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 53 | SD1\_D1 | IOBLK\_GRTC\_REG\_SD1\_D1 | 0x0502\_7060 | IOBLK\_GRTC\_REG\_SD1\_D1\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 53 | SD1\_D1 | IOBLK\_GRTC\_REG\_SD1\_D1 | 0x0502\_7060 | IOBLK\_GRTC\_REG\_SD1\_D1\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 53 | SD1\_D1 | IOBLK\_GRTC\_REG\_SD1\_D1 | 0x0502\_7060 | IOBLK\_GRTC\_REG\_SD1\_D1\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 53 | SD1\_D1 | IOBLK\_GRTC\_REG\_SD1\_D1 | 0x0502\_7060 | IOBLK\_GRTC\_REG\_SD1\_D1\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 54 | SD1\_D0 | IOBLK\_GRTC\_REG\_SD1\_D0 | 0x0502\_7064 | IOBLK\_GRTC\_REG\_SD1\_D0\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 54 | SD1\_D0 | IOBLK\_GRTC\_REG\_SD1\_D0 | 0x0502\_7064 | IOBLK\_GRTC\_REG\_SD1\_D0\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 54 | SD1\_D0 | IOBLK\_GRTC\_REG\_SD1\_D0 | 0x0502\_7064 | IOBLK\_GRTC\_REG\_SD1\_D0\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 54 | SD1\_D0 | IOBLK\_GRTC\_REG\_SD1\_D0 | 0x0502\_7064 | IOBLK\_GRTC\_REG\_SD1\_D0\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 54 | SD1\_D0 | IOBLK\_GRTC\_REG\_SD1\_D0 | 0x0502\_7064 | IOBLK\_GRTC\_REG\_SD1\_D0\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 54 | SD1\_D0 | IOBLK\_GRTC\_REG\_SD1\_D0 | 0x0502\_7064 | IOBLK\_GRTC\_REG\_SD1\_D0\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 54 | SD1\_D0 | IOBLK\_GRTC\_REG\_SD1\_D0 | 0x0502\_7064 | IOBLK\_GRTC\_REG\_SD1\_D0\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 55 | SD1\_CMD | IOBLK\_GRTC\_REG\_SD1\_CMD | 0x0502\_7068 | IOBLK\_GRTC\_REG\_SD1\_CMD\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 55 | SD1\_CMD | IOBLK\_GRTC\_REG\_SD1\_CMD | 0x0502\_7068 | IOBLK\_GRTC\_REG\_SD1\_CMD\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 55 | SD1\_CMD | IOBLK\_GRTC\_REG\_SD1\_CMD | 0x0502\_7068 | IOBLK\_GRTC\_REG\_SD1\_CMD\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 55 | SD1\_CMD | IOBLK\_GRTC\_REG\_SD1\_CMD | 0x0502\_7068 | IOBLK\_GRTC\_REG\_SD1\_CMD\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 55 | SD1\_CMD | IOBLK\_GRTC\_REG\_SD1\_CMD | 0x0502\_7068 | IOBLK\_GRTC\_REG\_SD1\_CMD\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 55 | SD1\_CMD | IOBLK\_GRTC\_REG\_SD1\_CMD | 0x0502\_7068 | IOBLK\_GRTC\_REG\_SD1\_CMD\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 55 | SD1\_CMD | IOBLK\_GRTC\_REG\_SD1\_CMD | 0x0502\_7068 | IOBLK\_GRTC\_REG\_SD1\_CMD\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 56 | SD1\_CLK | IOBLK\_GRTC\_REG\_SD1\_CLK | 0x0502\_706C | IOBLK\_GRTC\_REG\_SD1\_CLK\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 56 | SD1\_CLK | IOBLK\_GRTC\_REG\_SD1\_CLK | 0x0502\_706C | IOBLK\_GRTC\_REG\_SD1\_CLK\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 56 | SD1\_CLK | IOBLK\_GRTC\_REG\_SD1\_CLK | 0x0502\_706C | IOBLK\_GRTC\_REG\_SD1\_CLK\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 56 | SD1\_CLK | IOBLK\_GRTC\_REG\_SD1\_CLK | 0x0502\_706C | IOBLK\_GRTC\_REG\_SD1\_CLK\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 56 | SD1\_CLK | IOBLK\_GRTC\_REG\_SD1\_CLK | 0x0502\_706C | IOBLK\_GRTC\_REG\_SD1\_CLK\_DS2 | 7 | 0x0 | 输出驱动能力档位 bit 2 | Output buffer Driving Strength bit 2 |
| 56 | SD1\_CLK | IOBLK\_GRTC\_REG\_SD1\_CLK | 0x0502\_706C | IOBLK\_GRTC\_REG\_SD1\_CLK\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 56 | SD1\_CLK | IOBLK\_GRTC\_REG\_SD1\_CLK | 0x0502\_706C | IOBLK\_GRTC\_REG\_SD1\_CLK\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | IOBLK\_GRTC\_REG\_GPIO\_ZQ | 0x0502\_70E0 | IOBLK\_GRTC\_REG\_GPIO\_ZQ\_PU | 2 | 0x0 | 上拉电阻使能. 0=无效; 1=有效 | Pull up enable 0=disable; 1=enable |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | IOBLK\_GRTC\_REG\_GPIO\_ZQ | 0x0502\_70E0 | IOBLK\_GRTC\_REG\_GPIO\_ZQ\_PD | 3 | 0x1 | 下拉电阻使能. 0=无效; 1=有效 | Pull down enable 0=disable; 1=enable |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | IOBLK\_GRTC\_REG\_GPIO\_ZQ | 0x0502\_70E0 | IOBLK\_GRTC\_REG\_GPIO\_ZQ\_DS0 | 5 | 0x0 | 输出驱动能力档位 bit 0 | Output buffer Driving Strength bit 0 |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | IOBLK\_GRTC\_REG\_GPIO\_ZQ | 0x0502\_70E0 | IOBLK\_GRTC\_REG\_GPIO\_ZQ\_DS1 | 6 | 0x1 | 输出驱动能力档位 bit 1 | Output buffer Driving Strength bit 1 |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | IOBLK\_GRTC\_REG\_GPIO\_ZQ | 0x0502\_70E0 | IOBLK\_GRTC\_REG\_GPIO\_ZQ\_ST0 | 8 | 0x0 | 输入施密特触发器强度控制. bit 0 | Input buffer Schmitt trigger level bit 0 |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | IOBLK\_GRTC\_REG\_GPIO\_ZQ | 0x0502\_70E0 | IOBLK\_GRTC\_REG\_GPIO\_ZQ\_ST1 | 9 | 0x0 | 输入施密特触发器强度控制. bit 1 | Input buffer Schmitt trigger level bit 1 |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | IOBLK\_GRTC\_REG\_GPIO\_ZQ | 0x0502\_70E0 | IOBLK\_GRTC\_REG\_GPIO\_ZQ\_HE | 10 | 0x0 | 弱电平维持器(Bus holder) 使能 . 0=无效 ; 1=有效 | Bus holder enable 0=disable; 1=enable |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | IOBLK\_GRTC\_REG\_GPIO\_ZQ | 0x0502\_70E0 | IOBLK\_GRTC\_REG\_GPIO\_ZQ\_SL | 11 | 0x0 | 输出电平转换速率限制. 0=无效(较快) ; 1=有效(较慢) | Output Buffer Slew Rate Limit 0=disable; 1=enable |

## 4. Default Pin State (QFN)
| PinNum | PinName | Default Function | Default<br>Function Select | Default Pull<br>(PU/PD/NO PULL) | Default Function Direction (During reset) | IO TRAP<br>(become input<br>during reset) | IO Type | PowerDomain | IOBLK\_<br>Group | Fail-safe |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 6 | SD0\_CLK | SDIO0\_CLK | 0 | PD (61Kohm)->DRV\_LO + PD | Output (Low) |  | 18OD33 | VDDIO\_SD0\_EMMC | G10 |  |
| 7 | SD0\_CMD | SDIO0\_CMD | 0 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G10 |  |
| 8 | SD0\_D0 | SDIO0\_D[0] | 0 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G10 |  |
| 10 | SD0\_D1 | SDIO0\_D[1] | 0 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G10 |  |
| 11 | SD0\_D2 | SDIO0\_D[2] | 0 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G10 |  |
| 12 | SD0\_D3 | SDIO0\_D[3] | 0 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G10 |  |
| 14 | SD0\_CD | SDIO0\_CD | 0 | PU (60Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 15 | SD0\_PWR\_EN | XGPIOA[14] | 3 | PD (61Kohm) -> DRV\_HI/LO + PD | Input (Trap) -> Output | V | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 17 | SPK\_EN | XGPIOA[15] | 3 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 18 | UART0\_TX | UART0\_TX | 0 | PU (60Kohm) -> DRV\_HI + PU | Input (Trap) -> Output | V | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 19 | UART0\_RX | UART0\_RX | 0 | PU (60Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 20 | EMMC\_DAT2 | SPINOR\_HOLD\_X | 1 | PD (61Kohm) -> DRV\_HI + PD | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 21 | EMMC\_CLK | SPINOR\_SCK | 1 | DRV\_LO + PD (61Kohm) | Output (Low) |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 22 | EMMC\_DAT0 | SPINOR\_MOSI | 1 | PU (60Kohm) | Input (Trap) | V | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 23 | EMMC\_DAT3 | SPINOR\_WP\_X | 1 | PU (60Kohm) -> DRV\_HI + PU | Input (Trap) | V | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 24 | EMMC\_CMD | SPINOR\_MISO | 1 | PU (60Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 25 | EMMC\_DAT1 | SPINOR\_CS\_X | 1 | DRV\_HI + PD (61Kohm) | Output (High) |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 26 | JTAG\_CPU\_TMS | CR\_4WTMS | 0 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 27 | JTAG\_CPU\_TCK | CR\_4WTCK | 0 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 28 | IIC0\_SCL | CR\_4WTDI | 0 | PU (60Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 29 | IIC0\_SDA | CR\_4WTDO | 0 | PU (60Kohm) | Output (CR) |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 30 | AUX0 | XGPIOA[30] | 3 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD0\_EMMC | G7 |  |
| 38 | PWR\_VBAT\_DET | PWR\_VBAT\_DET | 0 | NO PULL | Input |  | 1.8V\_IO | VDDIO\_RTC | GRTC |  |
| 39 | PWR\_RSTN | PWR\_RSTN | 0 | PU (60Kohm) | Input |  | 1.8V\_IO | VDDIO\_RTC | GRTC | Fail-safe |
| 40 | PWR\_SEQ1 | PWR\_SEQ1 | 0 | PD (87Kohm) -> DRV + PD | Output (L->H) |  | 1.8V\_IO | VDDIO\_RTC | GRTC | Fail-safe |
| 41 | PWR\_SEQ2 | PWR\_SEQ2 | 0 | PD (87Kohm) -> DRV + PD | Output (L->H) |  | 1.8V\_IO | VDDIO\_RTC | GRTC |  |
| 43 | PWR\_WAKEUP0 | PWR\_WAKEUP0 | 0 | PD (87Kohm) | Input |  | 1.8V\_IO | VDDIO\_RTC | GRTC | Fail-safe |
| 44 | PWR\_BUTTON1 | PWR\_BUTTON1 | 0 | PU (60Kohm) | Input |  | 1.8V\_IO | VDDIO\_RTC | GRTC | Fail-safe |
| 47 | PWR\_GPIO0 | PWR\_GPIO[0] | 0 | PD (87Kohm) | Input |  | 1.8V\_IO | VDDIO\_RTC | GRTC | Fail-safe |
| 48 | PWR\_GPIO1 | PWR\_GPIO[1] | 0 | PD (87Kohm) | Input |  | 1.8V\_IO | VDDIO\_RTC | GRTC |  |
| 49 | PWR\_GPIO2 | PWR\_GPIO[2] | 0 | PD (87Kohm) | Input |  | 1.8V\_IO | VDDIO\_RTC | GRTC |  |
| 51 | SD1\_D3 | PWR\_SPINOR1\_CS\_X | 6 | DRV\_HI + PD (61Kohm) | Output (High) |  | 18OD33 | VDDIO\_SD1 | GRTC |  |
| 52 | SD1\_D2 | PWR\_SPINOR1\_HOLD\_X | 6 | PD (61Kohm) -> DRV\_HI + PD | Input |  | 18OD33 | VDDIO\_SD1 | GRTC |  |
| 53 | SD1\_D1 | PWR\_SPINOR1\_WP\_X | 6 | PD (61Kohm) -> DRV\_HI + PD | Input |  | 18OD33 | VDDIO\_SD1 | GRTC |  |
| 54 | SD1\_D0 | PWR\_SPINOR1\_MISO | 6 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD1 | GRTC |  |
| 55 | SD1\_CMD | PWR\_SPINOR1\_MOSI | 6 | PD (61Kohm) | Input |  | 18OD33 | VDDIO\_SD1 | GRTC |  |
| 56 | SD1\_CLK | PWR\_SPINOR1\_SCK | 6 | DRV\_LO + PD (61Kohm) | Output (Low) |  | 18OD33 | VDDIO\_SD1 | GRTC |  |
| 58 | PWM0\_BUCK | XGPIOB[0] | 3 | NO PULL | Input |  | 1.8V\_IO | VDD18A\_USB\_PLL\_ETH | G1 | Fail-safe |
| 59 | ADC1 | XGPIOB[3] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_USB\_PLL\_ETH | G1 | Fail-safe |
| 60 | USB\_VBUS\_DET | USB\_VBUS\_DET | 0 | NO PULL | Input |  | 1.8V\_IO | VDD18A\_USB\_PLL\_ETH | G1 |  |
| 62 | PAD\_ETH\_TXP\_\_\_EPHY\_RXN | XGPIOB[25] | 3 | NO PULL |  |  | ETH\_GPIO | VDD18A\_USB\_PLL\_ETH | G12 |  |
| 63 | PAD\_ETH\_TXM\_\_\_EPHY\_RXP | XGPIOB[24] | 3 | NO PULL |  |  | ETH\_GPIO | VDD18A\_USB\_PLL\_ETH | G12 |  |
| 64 | PAD\_ETH\_RXP\_\_\_EPHY\_TXN | XGPIOB[27] | 3 | NO PULL |  |  | ETH\_GPIO | VDD18A\_USB\_PLL\_ETH | G12 |  |
| 65 | PAD\_ETH\_RXM\_\_\_EPHY\_TXP | XGPIOB[26] | 3 | NO PULL |  |  | ETH\_GPIO | VDD18A\_USB\_PLL\_ETH | G12 |  |
| 72 | PAD\_MIPIRX4N | CR\_SCL0 | 0 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 73 | PAD\_MIPIRX4P | CR\_SDA0 | 0 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 74 | PAD\_MIPIRX3N | XGPIOC[4] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 75 | PAD\_MIPIRX3P | XGPIOC[5] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 76 | PAD\_MIPIRX2N | XGPIOC[6] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 77 | PAD\_MIPIRX2P | XGPIOC[7] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 78 | PAD\_MIPIRX1N | XGPIOC[8] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 79 | PAD\_MIPIRX1P | XGPIOC[9] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 80 | PAD\_MIPIRX0N | XGPIOC[10] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 81 | PAD\_MIPIRX0P | XGPIOC[11] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 83 | PAD\_MIPI\_TXM2 | XGPIOC[16] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 84 | PAD\_MIPI\_TXP2 | XGPIOC[17] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 85 | PAD\_MIPI\_TXM1 | XGPIOC[14] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 86 | PAD\_MIPI\_TXP1 | XGPIOC[15] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 87 | PAD\_MIPI\_TXM0 | XGPIOC[12] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 88 | PAD\_MIPI\_TXP0 | XGPIOC[13] | 3 | PD (87Kohm) | Input |  | 1.8V\_IO | VDD18A\_MIPI | G12 |  |
| 2 | PAD\_AUD\_AINL\_MIC | XGPIOC[23] | 3 | NO PULL | Input |  | AUDIO\_GPIO | VDD18A\_MIPI | G11 |  |
| 4 | PAD\_AUD\_AOUTR | XGPIOC[24] | 3 | NO PULL | Output |  | AUDIO\_GPIO | VDD18A\_MIPI | G11 |  |
| 67 | GPIO\_RTX\_\_\_EPHY\_RTX | XGPIOB[23] | 3 | PD (87Kohm) | Input | V | 1.8V\_IO | VDD18A\_USB\_PLL\_ETH | G12 | Fail-safe |
| 35 | GPIO\_ZQ\_\_\_PAD\_ZQ | PWR\_GPIO[24] | 3 | PD (87Kohm) | Input | V | 1.8V\_IO | VDDIO\_RTC | GRTC | Fail-safe |

## 5. Pin Configuration (QFN)
> Ghi chú: bảng này là **sơ đồ bố trí chân vật lý (footprint) của gói QFN 88 chân**, không phải bảng dữ liệu thông thường. Các số ở giữa bảng là số thứ tự pin theo vị trí vật lý xung quanh 4 cạnh chip; tên ở 2 cột ngoài cùng bên trái/phải và các hàng trên/dưới là tên tín hiệu tại vị trí pin tương ứng. Nên đối chiếu thêm datasheet gốc (hình ảnh) khi cần xác định vị trí vật lý chính xác, vì bảng markdown chỉ giữ lại giá trị chứ không giữ layout không gian.

|  |  | PAD\_MIPI\_TXP0 | PAD\_MIPI\_TXM0 | PAD\_MIPI\_TXP1 | PAD\_MIPI\_TXM1 | PAD\_MIPI\_TXP2 | PAD\_MIPI\_TXM2 | VDD18A\_MIPI | PAD\_MIPIRX0P | PAD\_MIPIRX0N | PAD\_MIPIRX1P | PAD\_MIPIRX1N | PAD\_MIPIRX2P | PAD\_MIPIRX2N | PAD\_MIPIRX3P | PAD\_MIPIRX3N | PAD\_MIPIRX4P | PAD\_MIPIRX4N | VDDC | USB\_DM | USB\_DP | VDD18A\_USB\_PLL\_ETH | GPIO\_RTX\_\_\_EPHY\_RTX |  |  |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|  |  | 88 | 87 | 86 | 85 | 84 | 83 | 82 | 81 | 80 | 79 | 78 | 77 | 76 | 75 | 74 | 73 | 72 | 71 | 70 | 69 | 68 | 67 |  |  |
| VSS18A\_AUD | 1 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 66 | VDD33A\_ETH\_USB |
| PAD\_AUD\_AINL\_MIC | 2 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 65 | PAD\_ETH\_RXM\_\_\_EPHY\_TXP |
| PAD\_AUD\_AVREF | 3 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 64 | PAD\_ETH\_RXP\_\_\_EPHY\_TXN |
| PAD\_AUD\_AOUTR | 4 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 63 | PAD\_ETH\_TXM\_\_\_EPHY\_RXP |
| VDD18A\_AUD | 5 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 62 | PAD\_ETH\_TXP\_\_\_EPHY\_RXN |
| SD0\_CLK | 6 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 61 | VDDC |
| SD0\_CMD | 7 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 60 | USB\_VBUS\_DET |
| SD0\_D0 | 8 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 59 | ADC1 |
| VDDC | 9 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 58 | PWM0\_BUCK |
| SD0\_D1 | 10 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 57 | VDDIO\_SD1 |
| SD0\_D2 | 11 |  |  |  |  |  |  |  |  |  |  | VSSC (EPAD) |  |  |  |  |  |  |  |  |  |  |  | 56 | SD1\_CLK |
| SD0\_D3 | 12 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 55 | SD1\_CMD |
| VDDIO\_SD0\_EMMC | 13 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 54 | SD1\_D0 |
| SD0\_CD | 14 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 53 | SD1\_D1 |
| SD0\_PWR\_EN | 15 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 52 | SD1\_D2 |
| VDDC | 16 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 51 | SD1\_D3 |
| SPK\_EN | 17 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 50 | VDDC |
| UART0\_TX | 18 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 49 | PWR\_GPIO2 |
| UART0\_RX | 19 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 48 | PWR\_GPIO1 |
| EMMC\_DAT2 | 20 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 47 | PWR\_GPIO0 |
| EMMC\_CLK | 21 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 46 | XTAL\_XOUT |
| EMMC\_DAT0 | 22 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | 45 | XTAL\_XIN |
|  |  | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 |  |  |
|  |  | EMMC\_DAT3 | EMMC\_CMD | EMMC\_DAT1 | JTAG\_CPU\_TMS | JTAG\_CPU\_TCK | IIC0\_SCL | IIC0\_SDA | AUX0 | VDDQ\_DRAM | VDDQ | VDDQ | VDDQ\_DRAM | GPIO\_ZQ\_\_\_PAD\_ZQ | VDDIO\_RTC | VDDC\_RTC | PWR\_VBAT\_DET | PWR\_RSTN | PWR\_SEQ1 | PWR\_SEQ2 | PTEST | PWR\_WAKEUP0 | PWR\_BUTTON1 |  |  |
| 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 |
| 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 | 88 LD QFN EPAD 9mmx9mmx0.9mm<br>管脚间距 0.35mm, 总管脚数 88 |

## 6. How to switch MIPI Audio_ETH
| MIPI 如何設成 GPIO |  |
| --- | --- |
| PAD\_MIPI\_TXM0 | 0x0A0D1064 bit[0] (reg\_pd\_lptrx[0])<br>  1 = GPIO (default) ; 0 = MIPI<br>0x0A0D1064 bit[8] (reg\_pd\_txdrv\_ldo[0]<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPI\_TXP0 | 0x0A0D1064 bit[0] (reg\_pd\_lptrx[0])<br>  1 = GPIO (default) ; 0 = MIPI<br>0x0A0D1064 bit[8] (reg\_pd\_txdrv\_ldo[0]<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPI\_TXM1 | 0x0A0D1064 bit[1] (reg\_pd\_lptrx[1])<br>  1 = GPIO (default) ; 0 = MIPI<br>0x0A0D1064 bit[9] (reg\_pd\_txdrv\_ldo[1]<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPI\_TXP1 | 0x0A0D1064 bit[1] (reg\_pd\_lptrx[1])<br>  1 = GPIO (default) ; 0 = MIPI<br>0x0A0D1064 bit[9] (reg\_pd\_txdrv\_ldo[1]<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPI\_TXM2 | 0x0A0D1064 bit[2] (reg\_pd\_lptrx[2])<br>  1 = GPIO (default) ; 0 = MIPI<br>0x0A0D1064 bit[10] (reg\_pd\_txdrv\_ldo[2]<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPI\_TXP2 | 0x0A0D1064 bit[2] (reg\_pd\_lptrx[2])<br>  1 = GPIO (default) ; 0 = MIPI<br>0x0A0D1064 bit[10] (reg\_pd\_txdrv\_ldo[2]<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX0N | 0x0A0D0000 bit[16] (reg\_mipirx\_pd\_rxlp[0])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX0P | 0x0A0D0000 bit[16] (reg\_mipirx\_pd\_rxlp[0])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX1N | 0x0A0D0000 bit[17] (reg\_mipirx\_pd\_rxlp[1])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX1P | 0x0A0D0000 bit[17] (reg\_mipirx\_pd\_rxlp[1])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX2N | 0x0A0D0000 bit[18] (reg\_mipirx\_pd\_rxlp[2])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX2P | 0x0A0D0000 bit[18] (reg\_mipirx\_pd\_rxlp[2])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX3N | 0x0A0D0000 bit[19] (reg\_mipirx\_pd\_rxlp[3])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX3P | 0x0A0D0000 bit[19] (reg\_mipirx\_pd\_rxlp[3])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX4N | 0x0A0D0000 bit[20] (reg\_mipirx\_pd\_rxlp[4])<br>  1 = GPIO (default) ; 0 = MIPI |
| PAD\_MIPIRX4P | 0x0A0D0000 bit[20] (reg\_mipirx\_pd\_rxlp[4])<br>  1 = GPIO (default) ; 0 = MIPI |
| Audio 如何設成 GPIO |  |
| PAD\_AUD\_AINL\_MIC | 0x03022004[23:22]<br>  2'b00 = analog mode (default) ; other = gpio mode<br>0x0300A12c[3:2]<br>  2'b00 = analog mode (default) ; other = gpio mode |
| PAD\_AUD\_AOUTR | 0x03022004[25:24]<br>  2'b00 = analog mode (default) ; other = gpio mode<br>0x0300A12c[1:0]<br>  2'b00 = analog mode (default) ; other = gpio mode |
| ETH 如何設成 GPIO |  |
| PAD\_ETH\_RXM\_\_\_EPHY\_TXP<br>PAD\_ETH\_RXP\_\_\_EPHY\_TXN<br>PAD\_ETH\_TXM\_\_\_EPHY\_RXP<br>PAD\_ETH\_TXP\_\_\_EPHY\_RXN | 0x03009804[0] = 1'b1 (rg\_ephy\_apb\_rw\_sel=1, use apb interface)<br>0x03009808[4:0] = 5'b00001 (rg\_ephy\_pll\_stable\_cnt[4:0] = 5'd1 (10us)<br>0x03009800[2] = 0x0905 (rg\_ephy\_dig\_rst\_n=1, reset release, other keep default)<br>wait 10us<br>0x0300907C[12:8]= 5'b00101 (page\_sel\_mode0 = page 5)<br>0x03009078[11:0] = 0xF00 (set to gpio from top)<br>0x03009074[10:9 2:1]= 0x606 (set ephy rxp&rxm input&output enable)<br>0x03009070[10:9 2:1]= 0x606 (set ephy rxp&rxm input&output enable) |
