/*
 * Tầng 3 (T3 - Tiny1C I2C Control Transport Adapter): Giao tiếp đọc/ghi thanh ghi 16-bit
 * của chip ASIC-2121W thông qua API Tầng 2 (soft_i2c_bus_t) và Tầng 0 (bb_gpio_platform_init).
 */

#include "data_rw.h"
#include <stdio.h>
#include <string.h>

#if defined(TINY1C_HAS_T2_SOFT_I2C) && defined(__has_include) && __has_include("hal_i2c_gpio_platform.h")
#include "hal_i2c_gpio_platform.h"
#define TINY1C_HAS_T0_I2C_PLATFORM 1
#endif

#if defined(__riscv) && defined(__has_include) && __has_include("FreeRTOS.h")
#define TINY1C_HAS_FREERTOS 1
#include "FreeRTOS.h"
#include "task.h"
#endif

static uint8_t s_i2c_bus = 5U;              /* Mặc định Bus 5 (Soft-I2C trên GPIOA15/GPIOA27) */
static uint16_t s_slave_id = I2C_SLAVE_ID;  /* Mặc định 0x78 (8-bit) -> 0x3C (7-bit) */
static bool s_i2c_hw_ready = false;

#if defined(TINY1C_HAS_T2_SOFT_I2C)
static soft_i2c_bus_t s_default_soft_i2c_bus;
static soft_i2c_bus_t *s_bound_i2c_bus = NULL;
static bb_io_t s_default_i2c_io;
#endif

void data_rw_set_i2c_bus(uint8_t i2c_bus_id)
{
    s_i2c_bus = i2c_bus_id;
}

uint8_t data_rw_get_i2c_bus(void)
{
    return s_i2c_bus;
}

void data_rw_set_slave_addr(uint16_t slave_id)
{
    if (slave_id != 0U) {
        s_slave_id = slave_id;
    }
}

uint16_t data_rw_get_slave_addr(void)
{
    return s_slave_id;
}

/*
 * Chuẩn hóa địa chỉ Slave về 7-bit (0x3C) theo đúng hợp đồng Tầng 1 (hal_i2c_bitbang.c).
 * Lưu ý quan trọng: 0x78 (I2C_SLAVE_ID trong SDK gốc) là địa chỉ 8-bit (0x3C << 1),
 * dù nhỏ hơn 0x7F nhưng bắt buộc phải dịch phải 1 bit về 0x3C (I2C_SLAVE_7BIT_ADDR)
 * để Tầng 1 không dịch trái thêm lần nữa thành 0xF0 trên đường truyền vật lý!
 */
static inline uint16_t get_7bit_addr(uint16_t slave_id)
{
    if ((slave_id == I2C_SLAVE_ID) || (slave_id == (I2C_SLAVE_ID | 1U)) || (slave_id > 0x7FU)) {
        return (uint16_t)(slave_id >> 1U);
    }
    return slave_id;
}

void delay_ms(uint32_t ms)
{
    if (ms == 0U) {
        return;
    }
#if defined(TINY1C_HAS_FREERTOS)
    TickType_t ticks = pdMS_TO_TICKS(ms);
    vTaskDelay((ticks > 0U) ? ticks : 1U);
#else
    (void)ms;
#endif
}

#if defined(TINY1C_HAS_T2_SOFT_I2C)
ir_error_t data_rw_bind_soft_i2c_bus(soft_i2c_bus_t *bus)
{
    if ((bus == NULL) || (!bus->ready)) {
        return IR_ERROR_PARAM;
    }
    s_bound_i2c_bus = bus;
    s_i2c_hw_ready  = true;
    return IR_SUCCESS;
}

soft_i2c_bus_t *data_rw_get_soft_i2c_bus(void)
{
    return s_bound_i2c_bus;
}
#endif

ir_error_t data_rw_i2c_hw_init(uint8_t i2c_bus_id)
{
    s_i2c_bus = i2c_bus_id;

    if (s_i2c_hw_ready) {
        return IR_SUCCESS;
    }

#if defined(TINY1C_HAS_T2_SOFT_I2C) && defined(TINY1C_HAS_T0_I2C_PLATFORM) && defined(__riscv)
    /*
     * Khởi tạo Tầng 0 (GPIOA15 SCL, GPIOA27 SDA + Pinmux + bộ đếm rdtime 25 MHz)
     * và Tầng 2 (soft_i2c_init ở 100 kHz Standard-Mode, clock stretch 20ms).
     */
    if (bb_gpio_platform_init(&s_default_i2c_io, THERMAL_RDTIME_HZ, true) != BB_OK) {
        return IR_I2C_DEVICE_OPEN_FAIL;
    }

    bb_status_t st = soft_i2c_init(&s_default_soft_i2c_bus,
                                   &s_default_i2c_io,
                                   THERMAL_I2C_FREQ_HZ,
                                   THERMAL_I2C_STRETCH_US);
    if (st != BB_OK) {
        return IR_I2C_DEVICE_OPEN_FAIL;
    }

    s_bound_i2c_bus = &s_default_soft_i2c_bus;
#endif

    s_i2c_hw_ready = true;
    return IR_SUCCESS;
}

ir_error_t data_rw_i2c_recover(void)
{
#if defined(TINY1C_HAS_T2_SOFT_I2C) && defined(__riscv)
    if (s_bound_i2c_bus == NULL) {
        return IR_I2C_DEVICE_OPEN_FAIL;
    }
    bb_status_t st = soft_i2c_recover(s_bound_i2c_bus, THERMAL_I2C_TIMEOUT_MS);
    return (st == BB_OK) ? IR_SUCCESS : IR_I2C_TRANSFER_FAIL;
#else
    return IR_SUCCESS;
#endif
}

#if !defined(TINY1C_HAS_T2_SOFT_I2C) || !defined(__riscv)
/* Bộ nhớ giả lập cho chế độ kiểm thử độc lập khi chưa liên kết Tầng 2 */
__attribute__((weak)) int i2c_read(uint8_t i2c_id, uint8_t dev, uint16_t addr,
                                   uint16_t alen, uint8_t *buffer, uint16_t len)
{
    (void)i2c_id;
    (void)dev;
    (void)alen;
    if ((buffer != NULL) && (len > 0U)) {
        memset(buffer, 0, len);
        if (addr == I2C_VD_BUFFER_STATUS) {
            buffer[0] = VCMD_BUSY_STS_IDLE | VCMD_RST_STS_PASS;
        }
    }
    return 0;
}

__attribute__((weak)) int i2c_write(uint8_t i2c_id, uint8_t dev, uint16_t addr,
                                    uint16_t alen, uint8_t *buffer, uint16_t len)
{
    (void)i2c_id;
    (void)dev;
    (void)addr;
    (void)alen;
    (void)buffer;
    (void)len;
    return 0;
}
#endif

static int t3_i2c_read_reg16(uint16_t dev_7bit, uint16_t reg_addr, uint8_t *buf, uint16_t len)
{
    if (!s_i2c_hw_ready) {
        if (data_rw_i2c_hw_init(s_i2c_bus) != IR_SUCCESS) {
            return -1;
        }
    }

#if defined(TINY1C_HAS_T2_SOFT_I2C) && defined(__riscv)
    if (s_bound_i2c_bus == NULL) {
        return -1;
    }

    for (uint32_t attempt = 0U; attempt < 3U; ++attempt) {
        bb_status_t st = soft_i2c_read_reg16(s_bound_i2c_bus,
                                             dev_7bit,
                                             reg_addr,
                                             buf,
                                             (size_t)len,
                                             THERMAL_I2C_TIMEOUT_MS);
        if (st == BB_OK) {
            return 0;
        }
        (void)soft_i2c_recover(s_bound_i2c_bus, THERMAL_I2C_TIMEOUT_MS);
        delay_ms(1U << attempt);
    }
    return -1;
#else
    return i2c_read(s_i2c_bus, (uint8_t)dev_7bit, reg_addr, 2U, buf, len);
#endif
}

static int t3_i2c_write_reg16(uint16_t dev_7bit, uint16_t reg_addr, const uint8_t *buf, uint16_t len)
{
    if (!s_i2c_hw_ready) {
        if (data_rw_i2c_hw_init(s_i2c_bus) != IR_SUCCESS) {
            return -1;
        }
    }

#if defined(TINY1C_HAS_T2_SOFT_I2C) && defined(__riscv)
    if (s_bound_i2c_bus == NULL) {
        return -1;
    }

    for (uint32_t attempt = 0U; attempt < 3U; ++attempt) {
        bb_status_t st;
        if ((len > 0U) && (len <= 256U) && (buf != NULL)) {
            st = soft_i2c_write_reg16(s_bound_i2c_bus,
                                      (uint8_t)dev_7bit,
                                      reg_addr,
                                      buf,
                                      (size_t)len,
                                      THERMAL_I2C_TIMEOUT_MS);
        } else {
            uint8_t frame[2U + I2C_OUT_BUFFER_MAX];
            frame[0] = (uint8_t)((reg_addr >> 8U) & 0xFFU);
            frame[1] = (uint8_t)(reg_addr & 0xFFU);
            if ((len > 0U) && (buf != NULL)) {
                memcpy(&frame[2], buf, len);
            }
            bb_msg_t msg = {
                .addr = (uint8_t)dev_7bit,
                .read = false,
                .data = frame,
                .len  = (size_t)(2U + len)
            };
            st = soft_i2c_transfer(s_bound_i2c_bus, &msg, 1U, THERMAL_I2C_TIMEOUT_MS);
        }

        if (st == BB_OK) {
            return 0;
        }
        (void)soft_i2c_recover(s_bound_i2c_bus, THERMAL_I2C_TIMEOUT_MS);
        delay_ms(1U << attempt);
    }
    return -1;
#else
    return i2c_write(s_i2c_bus, (uint8_t)dev_7bit, reg_addr, 2U, (uint8_t *)buf, len);
#endif
}

ir_error_t i2c_data_read(uint16_t byI2CSlaveID, uint16_t wI2CRegAddr, uint16_t wLen, uint8_t *pbyData)
{
    if ((pbyData == NULL) || (wLen == 0U) || (wLen > I2C_VD_BUFFER_DATA_LEN)) {
        return IR_ERROR_PARAM;
    }

    uint16_t raw_id  = (byI2CSlaveID != 0U) ? byI2CSlaveID : s_slave_id;
    uint16_t addr_7b = get_7bit_addr(raw_id);

    if (t3_i2c_read_reg16(addr_7b, wI2CRegAddr, pbyData, wLen) != 0) {
        return IR_I2C_GET_REGISTER_FAIL;
    }
    return IR_SUCCESS;
}

static ir_error_t i2c_check_access_done(uint16_t slave_id)
{
    uint8_t status = 0xFFU;
    uint16_t wait_ms = (uint16_t)I2C_TRANSFER_WAIT_TIME_MS;
    uint16_t raw_id  = (slave_id != 0U) ? slave_id : s_slave_id;
    uint16_t addr_7b = get_7bit_addr(raw_id);

    do {
        if (t3_i2c_read_reg16(addr_7b, I2C_VD_BUFFER_STATUS, &status, 1U) != 0) {
            return IR_I2C_GET_REGISTER_FAIL;
        }

        if ((status & VCMD_ERR_STS_BIT) != VCMD_ERR_STS_SUCCESS) {
            return IR_CHECK_DONE_FAIL;
        }

        if ((status & (VCMD_RST_STS_BIT | VCMD_BUSY_STS_BIT)) ==
            (VCMD_BUSY_STS_IDLE | VCMD_RST_STS_PASS)) {
            return IR_SUCCESS;
        }

        delay_ms(5U);
        if (wait_ms >= 5U) {
            wait_ms -= 5U;
        } else {
            break;
        }
    } while (wait_ms > 0U);

    return IR_CHECK_DONE_FAIL;
}

ir_error_t i2c_data_write(uint16_t byI2CSlaveID, uint16_t wI2CRegAddr, uint16_t wLen, uint8_t *pbyData)
{
    if ((wLen > I2C_OUT_BUFFER_MAX) || ((wLen > 0U) && (pbyData == NULL))) {
        return IR_ERROR_PARAM;
    }

    uint16_t raw_id  = (byI2CSlaveID != 0U) ? byI2CSlaveID : s_slave_id;
    uint16_t addr_7b = get_7bit_addr(raw_id);

    if (t3_i2c_write_reg16(addr_7b, wI2CRegAddr, pbyData, wLen) != 0) {
        return IR_I2C_SET_REGISTER_FAIL;
    }

    /* Nếu bit 15 (0x8000 - I2C_VD_BUFFER_HLD) bật, chỉ nạp dữ liệu giữ, chưa cần poll 0x0200 */
    if ((wI2CRegAddr & I2C_VD_CHECK_ACCESS) != 0U) {
        return IR_SUCCESS;
    }
    return i2c_check_access_done(raw_id);
}
