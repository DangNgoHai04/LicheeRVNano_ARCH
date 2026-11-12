/*
 * Tầng 3 (T3 - Tiny1C Inter-Processor Communication): Cầu nối giao tiếp hai chiều
 * giữa nhân RTOS (C906_1) và nhân Linux (C906_0) qua Mailbox & Shared DDR RAM (0x8FE80000).
 */

#ifndef TINY1C_IPC_H
#define TINY1C_IPC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "core.h"
#include "vospi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IPC_MAGIC                       (0x54494E59UL) /* 'TINY' */
#define SHARED_MEM_BASE                 (0x8FE80000UL) /* Vùng nhớ DDR chia sẻ cho Mailbox IPC */

typedef enum {
    /* Sự kiện thông báo từ RTOS -> CPU chính (Linux) */
    CMD_THERMAL_INIT                    = 0x10,
    CMD_THERMAL_FRAME_READY             = 0x11,
    CMD_THERMAL_CMD_DONE                = 0x12,

    /* Lệnh điều khiển luồng & đo nhiệt từ CPU chính (Linux) -> RTOS */
    CMD_THERMAL_SHUTTER_REQ             = 0x14,
    CMD_THERMAL_SET_MODE                = 0x15,
    CMD_THERMAL_QUERY_EXTREMES          = 0x16,
    CMD_THERMAL_QUERY_POINT             = 0x17,
    CMD_THERMAL_QUERY_LINE              = 0x18,
    CMD_THERMAL_QUERY_RECT              = 0x19,

    /* Nhóm Tính năng 5: Xử lý hình ảnh nâng cao & ISP */
    CMD_THERMAL_SET_PALETTE             = 0x20,
    CMD_THERMAL_SET_ZOOM                = 0x21,
    CMD_THERMAL_SET_IMAGE_PARAM         = 0x22,
    CMD_THERMAL_SET_MIRROR_FLIP         = 0x23,

    /* Nhóm Tính năng 6: Căn chỉnh & Hiệu chuẩn thiết bị */
    CMD_THERMAL_DPC_AUTO                = 0x30,
    CMD_THERMAL_DPC_ADD_POINT           = 0x31,
    CMD_THERMAL_DPC_REMOVE_POINT        = 0x32,
    CMD_THERMAL_RMCOVER_CALC            = 0x33,
    CMD_THERMAL_RMCOVER_ENABLE          = 0x34,
    CMD_THERMAL_RECAL_1POINT            = 0x35,
    CMD_THERMAL_RECAL_2POINT_P1         = 0x36,
    CMD_THERMAL_RECAL_2POINT_P2         = 0x37,
    CMD_THERMAL_SET_TPD_PARAM           = 0x38,
    CMD_THERMAL_SET_ENV_PARAMS          = 0x39,
    CMD_THERMAL_SET_GAIN_MODE           = 0x3A,
    CMD_THERMAL_SAVE_CONFIG             = 0x3B,
    CMD_THERMAL_RESTORE_DEFAULT         = 0x3C
} ipc_cmd_t;

typedef enum {
    IPC_ZOOM_CENTER_UP                  = 0,
    IPC_ZOOM_CENTER_DOWN                = 1,
    IPC_ZOOM_POSITION_UP                = 2,
    IPC_ZOOM_POSITION_DOWN              = 3
} ipc_zoom_mode_t;

typedef struct {
    uint16_t x0;
    uint16_t y0;
    uint16_t x1;
    uint16_t y1;
    uint8_t  zoom_mode;
    uint8_t  zoom_step;
    uint16_t param_id;
    uint16_t param_val;
    uint16_t reserved;
    float    emissivity;
    float    distance_m;
    float    refl_temp_c;
    float    atm_temp_c;
    float    transmittance;
} __attribute__((packed)) ipc_ctrl_params_t;

typedef struct {
    uint32_t             magic;
    uint32_t             raw_frame_addr;
    uint32_t             frame_bytes;
    uint32_t             frame_seq;
    uint32_t             timestamp_ms;
    uint16_t             frame_width;
    uint16_t             frame_height;
    uint8_t              frame_mode;
    uint8_t              palette_id;
    uint8_t              last_cmd_id;
    uint8_t              reserved0;
    int32_t              last_cmd_status;
    uint16_t             sensor_vtemp;
    uint16_t             reserved1;
    vospi_frame_header_t vospi_hdr;
    extremes_t           extremes;
    region_temp_t        region_result;
    ipc_ctrl_params_t    ctrl_params;
} __attribute__((packed)) shared_data_t;

int ipc_init(uintptr_t shared_mem_addr);
shared_data_t *ipc_get_shared_data(void);
void ipc_flush_shared_cache(uintptr_t buf_addr, size_t buf_size);
void ipc_inv_shared_cache(uintptr_t buf_addr, size_t buf_size);
int ipc_notify_frame_ready(void);
int ipc_notify_cmd_done(uint8_t cmd_id, int32_t status);
void ipc_process_linux_cmds(void);

#ifdef __cplusplus
}
#endif

#endif /* TINY1C_IPC_H */
