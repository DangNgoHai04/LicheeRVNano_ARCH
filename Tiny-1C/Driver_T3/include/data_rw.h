/*
 * Tầng 3 (T3 - Tiny1C I2C Control Transport Adapter): Giao tiếp đọc/ghi thanh ghi 16-bit
 * của chip ASIC-2121W thông qua API Tầng 2 (soft_i2c_bus_t) và Tầng 0 (bb_gpio_platform_init).
 *
 * Nguyên tắc phân tầng:
 * - Tầng 3 KHÔNG truy cập trực tiếp thanh ghi GPIO hay I2C MMIO của SoC.
 * - Mọi thao tác đọc/ghi thanh ghi 16-bit đều ủy quyền xuống Tầng 2:
 *   soft_i2c_read_reg16(), soft_i2c_write_reg16(), soft_i2c_transfer(), soft_i2c_recover().
 * - Địa chỉ Slave truyền xuống Tầng 2 luôn được chuẩn hóa về dạng 7-bit (0x3C),
 *   tuân thủ nghiêm ngặt ràng buộc của Tầng 1 (hal_i2c_bitbang.h).
 */

#ifndef TINY1C_DATA_RW_H
#define TINY1C_DATA_RW_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "all_config.h"

#if defined(__has_include) && __has_include("soft_i2c.h") && __has_include("FreeRTOS.h")
#include "soft_i2c.h"
#define TINY1C_HAS_T2_SOFT_I2C 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * 1. Địa chỉ Slave & Thanh ghi giao thức VDCMD của ASIC-2121W
 * -------------------------------------------------------------------------- */

/* Địa chỉ I2C của Tiny1C: 0x78 (dạng 8-bit SDK gốc) = 0x3C (dạng 7-bit chuẩn T1/T2) */
#define I2C_SLAVE_ID                    0x78U
#define I2C_SLAVE_7BIT_ADDR             0x3CU

/* Các thanh ghi bộ đệm lệnh Vendor Command (VDCMD) bên trong ASIC-2121W */
#define I2C_VD_BUFFER_RW                0x1D00U /* Ghi vào đây sẽ kích hoạt thực thi lệnh */
#define I2C_VD_BUFFER_HLD               0x9D00U /* Chỉ nạp dữ liệu vào bộ đệm giữ, chưa thực thi */
#define I2C_VD_CHECK_ACCESS             0x8000U /* Bit 15 = 1: Không cần poll trạng thái 0x0200 */
#define I2C_VD_BUFFER_DATA_LEN          256U    /* Độ dài tối đa đọc/ghi mỗi lượt VDCMD */
#define I2C_OUT_BUFFER_MAX              64U     /* Kích thước tối đa một gói ghi I2C đơn lẻ */

/* Thanh ghi trạng thái thực thi lệnh VDCMD (0x0200):
 * - Bits [7:2]: Mã lỗi thực thi (VCMD_ERR_STS_*)
 * - Bit  [1]  : Kết quả (0 = PASS, 1 = FAIL)
 * - Bit  [0]  : Cờ bận (0 = IDLE, 1 = BUSY)
 */
#define I2C_VD_BUFFER_STATUS            0x0200U
#define VCMD_BUSY_STS_BIT               0x01U
#define VCMD_RST_STS_BIT                0x02U
#define VCMD_ERR_STS_BIT                0xFCU

#define VCMD_BUSY_STS_IDLE              0x00U
#define VCMD_BUSY_STS_BUSY              0x01U

#define VCMD_RST_STS_PASS               0x00U
#define VCMD_RST_STS_FAIL               0x02U

#define VCMD_ERR_STS_SUCCESS            0x00U
#define VCMD_ERR_STS_LEN_ERR            0x04U
#define VCMD_ERR_STS_UNKNOWN_CMD_ERR    0x08U
#define VCMD_ERR_STS_HW_ERR             0x0CU
#define VCMD_ERR_STS_UNKNOWN_SUBCMD_ERR 0x10U
#define VCMD_ERR_STS_PARAM_ERR          0x14U

/* --------------------------------------------------------------------------
 * 2. Cấu trúc Header của 3 loại gói tin VDCMD (Short, Standard, Long)
 * -------------------------------------------------------------------------- */

typedef struct {
    uint8_t byCmdType;
    uint8_t bySubCmd;
    uint8_t byPara;
    uint8_t byAddr_h;
    uint8_t byAddr_l;
    uint8_t byAddr_ll;
    uint8_t byLen_h;
    uint8_t byLen_l;
} vdcmd_std_header_t;

typedef struct {
    uint8_t byCmdType;
    uint8_t bySubCmd;
    uint8_t byParam_h;
    uint8_t byParam_l;
    uint8_t byAddr1_hh;
    uint8_t byAddr1_h;
    uint8_t byAddr1_l;
    uint8_t byAddr1_ll;
    uint8_t byAddr2_hh;
    uint8_t byAddr2_h;
    uint8_t byAddr2_l;
    uint8_t byAddr2_ll;
    uint8_t byLen_hh;
    uint8_t byLen_h;
    uint8_t byLen_l;
    uint8_t byLen_ll;
} vdcmd_long_header_t;

typedef struct {
    uint8_t byCmd;
    uint8_t byLen;
    uint8_t byAddr_h;
    uint8_t byAddr_l;
} vdcmd_short_header_t;

/* --------------------------------------------------------------------------
 * 3. Các hàm khởi tạo, liên kết Bus Tầng 2 và Đọc/Ghi thanh ghi 16-bit
 * -------------------------------------------------------------------------- */

/* Tác dụng: Trì hoãn thực thi theo mili-giây (nhường CPU qua vTaskDelay khi Scheduler chạy) */
void delay_ms(uint32_t ms);
#define HAL_Delay(ms) delay_ms(ms)

#if defined(TINY1C_HAS_T2_SOFT_I2C)
/* Tác dụng: Liên kết trực tiếp một đối tượng soft_i2c_bus_t đã được khởi tạo ở Tầng 2 */
ir_error_t data_rw_bind_soft_i2c_bus(soft_i2c_bus_t *bus);

/* Tác dụng: Lấy con trỏ tới đối tượng soft_i2c_bus_t Tầng 2 đang sử dụng */
soft_i2c_bus_t *data_rw_get_soft_i2c_bus(void);
#endif

/* Tác dụng: Khởi tạo Tầng 0 (bb_gpio_platform_init) và Tầng 2 (soft_i2c_init) cho Bus I2C */
ir_error_t data_rw_i2c_hw_init(uint8_t i2c_bus_id);

/* Tác dụng: Chủ động phát xung giải cứu Bus I2C thông qua hàm soft_i2c_recover() của Tầng 2 */
ir_error_t data_rw_i2c_recover(void);

/* Tác dụng: Đọc dữ liệu từ thanh ghi 16-bit của ASIC-2121W qua soft_i2c_read_reg16() (Tầng 2) */
ir_error_t i2c_data_read(uint16_t byI2CSlaveID,
                         uint16_t wI2CRegAddr,
                         uint16_t wLen,
                         uint8_t *pbyData);

/* Tác dụng: Ghi dữ liệu vào thanh ghi 16-bit của ASIC-2121W qua soft_i2c_write_reg16() (Tầng 2)
 *           và tự động chờ hoàn tất tại 0x0200 nếu ghi vào vùng thực thi 0x1D00. */
ir_error_t i2c_data_write(uint16_t byI2CSlaveID,
                          uint16_t wI2CRegAddr,
                          uint16_t wLen,
                          uint8_t *pbyData);

/* Tác dụng: Cấu hình và truy vấn mã định danh Bus I2C cùng địa chỉ Slave */
void data_rw_set_i2c_bus(uint8_t i2c_bus_id);
uint8_t data_rw_get_i2c_bus(void);
void data_rw_set_slave_addr(uint16_t slave_id);
uint16_t data_rw_get_slave_addr(void);

#ifdef __cplusplus
}
#endif

#endif /* TINY1C_DATA_RW_H */
