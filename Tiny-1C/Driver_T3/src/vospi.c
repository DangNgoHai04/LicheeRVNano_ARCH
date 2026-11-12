/*
 * Tầng 3 (T3 - Tiny1C VoSPI Frame Acquisition): Thu nhận khung hình nhiệt độ /
 * hình ảnh qua Tầng 2 SPI0 (DMA tự động chia cụm 4KB + giữ CS GPIOC17 LOW).
 */

#include "vospi.h"
#include "data_rw.h"
#include <stdio.h>
#include <string.h>

static uint8_t s_spi_id = 0U;
static int s_cs_gpio_pin = SPI0_DSI_CS_PIN_DEFAULT;
static uint16_t s_baud_div = SPI0_BAUDR_DIV_DEFAULT; /* 187.5MHz / 8 = 23.4375 MHz */
static bool s_use_25m_osc = false;

/* Bộ đệm trao đổi gói Header 512-byte căn chỉnh 64-byte theo dòng Cache C906 */
static uint8_t s_tx_dummy[VOSPI_TX_DUMMY_LEN] __attribute__((aligned(64)));
static uint8_t s_rx_dummy[VOSPI_TX_DUMMY_LEN] __attribute__((aligned(64)));
static vospi_frame_header_t s_last_vospi_hdr;

void cvi_gpio_set_value(int pin, int value)
{
    spi0_dsi_cs_set(pin, value != 0);
}

int cvi_spi_transfer(uint8_t spi_id, const uint8_t *tx_buf, uint8_t *rx_buf, uint32_t len)
{
    (void)spi_id;
    return spi0_dsi_transfer_burst(tx_buf, rx_buf, len);
}

void spi_stream_set_clock_config(uint16_t baud_div, bool use_25m_osc)
{
    s_baud_div    = baud_div;
    s_use_25m_osc = use_25m_osc;
    spi0_dsi_configure_vospi(s_baud_div, s_use_25m_osc);
}

int spi_stream_init(uint8_t spi_id, int cs_gpio_pin)
{
    s_spi_id      = spi_id;
    s_cs_gpio_pin = (cs_gpio_pin >= 0) ? cs_gpio_pin : SPI0_DSI_CS_PIN_DEFAULT;

    memset(s_tx_dummy, 0, sizeof(s_tx_dummy));
    memset(s_rx_dummy, 0, sizeof(s_rx_dummy));
    memset(&s_last_vospi_hdr, 0, sizeof(s_last_vospi_hdr));
    s_last_vospi_hdr.frame_valid   = 1U;
    s_last_vospi_hdr.shutter_state = 1U;
    s_last_vospi_hdr.gain_state    = 1U;

    /* Khởi tạo Bus SPI0 và Thiết bị Tiny1C thông qua Tầng 2 */
    spi0_dsi_hw_init();
    spi0_dsi_configure_vospi(s_baud_div, s_use_25m_osc);

    printf("[Thermal VOSPI] Initialized on SPI%u via T2 Driver, CS Pin: 0x%04X\n",
           (unsigned int)s_spi_id, (unsigned int)s_cs_gpio_pin);
    return 0;
}

void spi_cs_set(bool level)
{
    cvi_gpio_set_value(s_cs_gpio_pin, level ? 1 : 0);
}

const vospi_frame_header_t *spi_get_last_header(void)
{
    return &s_last_vospi_hdr;
}

/*
 * Thu nhận 1 khung đơn 96 KB (256x192x2 = 98,304 byte) qua Tầng 2:
 *   1. Gửi gói Header 512 byte (byte [0] = header_type: 0xAA hoặc 0xCC) trong một phiên CS LOW.
 *   2. Bóc tách 32-byte vospi_frame_header_t tại offset 64.
 *   3. Chờ 1ms theo đặc tả ASIC-2121W để DMA nội bộ của cảm biến chuẩn bị khung.
 *   4. Gọi hal_spi_transfer_frame(NULL, dst_96kb, FRAME_BYTES, 0x0055, timeout):
 *      Tầng 2/Tầng 1 tự động giữ CS GPIOC17 LOW, phát byte điền 0x55 mà không cần
 *      bộ đệm TX 512B trong vòng lặp 192 hàng, chia cụm DMA 4096B và invalidate D-Cache.
 */
static int vospi_read_single_frame(uint8_t *dst_96kb, uint8_t header_type)
{
    memset(s_tx_dummy, 0, sizeof(s_tx_dummy));
    s_tx_dummy[0] = header_type;

    int ret = hal_spi_transfer_frame(s_tx_dummy,
                                     s_rx_dummy,
                                     VOSPI_TX_DUMMY_LEN,
                                     0x0000U,
                                     THERMAL_SPI_TIMEOUT_MS);
    if (ret != 0) {
        (void)hal_spi_recover_bus();
        return -2;
    }

    memcpy(&s_last_vospi_hdr, &s_rx_dummy[64], sizeof(vospi_frame_header_t));

    delay_ms(1U);

    ret = hal_spi_transfer_frame(NULL,
                                 dst_96kb,
                                 (size_t)FRAME_BYTES,
                                 0x0055U,
                                 THERMAL_SPI_TIMEOUT_MS);
    if (ret != 0) {
        (void)hal_spi_recover_bus();
        return -3;
    }

    return 0;
}

int spi_get_frame(uint8_t *out_buffer, uint32_t buf_len, uint8_t header_type)
{
    if ((out_buffer == NULL) || (buf_len < (uint32_t)FRAME_BYTES)) {
        return -1;
    }

    if (buf_len >= (uint32_t)DUAL_FRAME_BYTES) {
        /*
         * Chế độ Kép (MODE_DUAL - 192 KB):
         * Theo chuẩn SDK hãng (image_and_temp_frame_interval_get), thu liên tiếp:
         *   - Nửa đầu 96 KB: Khung ảnh YUV422 (VOSPI_HEADER_IMAGE = 0xAA)
         *   - Nửa sau 96 KB: Ma trận nhiệt độ RAW16 (VOSPI_HEADER_TEMP = 0xCC)
         */
        int ret = vospi_read_single_frame(out_buffer, VOSPI_HEADER_IMAGE);
        if (ret != 0) {
            return ret;
        }
        return vospi_read_single_frame(out_buffer + FRAME_BYTES, VOSPI_HEADER_TEMP);
    }

    return vospi_read_single_frame(out_buffer, header_type);
}
