/*
 * Tầng 3 (T3 - Tiny1C Inter-Processor Communication): Quản lý bộ nhớ chia sẻ
 * DDR (0x8FE80000), đồng bộ D-Cache 64-byte của XuanTie C906 và giao tiếp Mailbox.
 */

#include "ipc.h"
#include "core.h"
#include <stdio.h>
#include <string.h>

#if defined(__riscv) && defined(__has_include) && __has_include("rtos_cmdqu.h")
#define TINY1C_HAS_CVI_MAILBOX 1
#ifndef CONFIG_64BIT
#define CONFIG_64BIT
#endif
#ifndef FREERTOS_BSP
#define FREERTOS_BSP
#endif
#if __has_include("FreeRTOS.h")
#define THERMAL_HAS_FREERTOS_HDR 1
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#if __has_include("comm.h")
#include "comm.h"
#define THERMAL_HAS_COMM_HDR 1
#endif
#endif
#include "mmio.h"
#include "arch_helpers.h"
#include "rtos_cmdqu.h"
#include "cvi_mailbox.h"
#include "top_reg.h"

#if defined(THERMAL_HAS_COMM_HDR)
__attribute__((weak)) QueueHandle_t main_GetMODHandle(QUEUE_HANDLE_E handle_idx)
{
    (void)handle_idx;
    return NULL;
}
#endif

static int cvi_rtos_send_cmdqu(cmdqu_t *cmdq)
{
    if (cmdq == NULL) {
        return -1;
    }

#if defined(THERMAL_HAS_COMM_HDR)
    QueueHandle_t cmdqu_q = main_GetMODHandle(E_QUEUE_CMDQU);
    if (cmdqu_q != NULL) {
        if (xQueueSend(cmdqu_q, cmdq, 0U) == pdPASS) {
            return 0;
        }
        return -2;
    }
#endif

    volatile struct mailbox_set_register *mbox_reg =
        (volatile struct mailbox_set_register *)MAILBOX_REG_BASE;
    volatile cmdqu_t *mbox_buf = (volatile cmdqu_t *)MAILBOX_REG_BUFF;
    const int send_to_cpu = 1; /* SEND_TO_CPU1: C906B (Linux) */

    for (int valid = 0; valid < MAILBOX_MAX_NUM; valid++) {
        if ((mbox_buf[valid].resv.valid.linux_valid == 0) &&
            (mbox_buf[valid].resv.valid.rtos_valid == 0)) {
            volatile int *ptr = (volatile int *)&mbox_buf[valid];
            cmdq->resv.valid.rtos_valid = 1;
            *ptr = ((cmdq->ip_id << 0) | (cmdq->cmd_id << 8) | (cmdq->block << 15) |
                    (cmdq->resv.valid.linux_valid << 16) |
                    (cmdq->resv.valid.rtos_valid << 24));
            mbox_buf[valid].param_ptr = cmdq->param_ptr;

            mbox_reg->cpu_mbox_set[send_to_cpu].cpu_mbox_int_clr.mbox_int_clr = (char)(1 << valid);
            mbox_reg->cpu_mbox_en[send_to_cpu].mbox_info |= (char)(1 << valid);
            mbox_reg->mbox_set.mbox_set = (char)(1 << valid);
            return 0;
        }
    }
    return -3;
}

#else
/* Chế độ kiểm thử độc lập trên máy chủ (Host Standalone Test) */
#ifndef IP_CAMERA
#define IP_CAMERA 7
#endif
typedef struct {
    unsigned char ip_id;
    unsigned char cmd_id : 7;
    unsigned char block : 1;
    union {
        struct {
            unsigned char linux_valid;
            unsigned char rtos_valid;
        } valid;
        unsigned short mstime;
    } resv;
    unsigned int param_ptr;
} __attribute__((packed)) cmdqu_t;

static uint8_t s_host_shared_backing[sizeof(shared_data_t)] __attribute__((aligned(64)));

static inline int cvi_rtos_send_cmdqu(cmdqu_t *cmdq)
{
    (void)cmdq;
    return 0;
}
#endif

static volatile shared_data_t *s_shared_data = NULL;
static uintptr_t s_shared_addr = SHARED_MEM_BASE;

void ipc_flush_shared_cache(uintptr_t buf_addr, size_t buf_size)
{
    if ((buf_addr == 0U) || (buf_size == 0U)) {
        return;
    }
#if defined(TINY1C_HAS_CVI_MAILBOX)
    uintptr_t aligned_addr = buf_addr & ~(uintptr_t)63UL;
    size_t aligned_size    = ((buf_addr + buf_size - aligned_addr) + 63UL) & ~(size_t)63UL;
    flush_dcache_range(aligned_addr, aligned_size);
    __asm__ volatile("fence rw, rw" ::: "memory");
#endif
}

void ipc_inv_shared_cache(uintptr_t buf_addr, size_t buf_size)
{
    if ((buf_addr == 0U) || (buf_size == 0U)) {
        return;
    }
#if defined(TINY1C_HAS_CVI_MAILBOX)
    uintptr_t aligned_addr = buf_addr & ~(uintptr_t)63UL;
    size_t aligned_size    = ((buf_addr + buf_size - aligned_addr) + 63UL) & ~(size_t)63UL;
    inv_dcache_range(aligned_addr, aligned_size);
    __asm__ volatile("fence rw, rw" ::: "memory");
#endif
}

int ipc_init(uintptr_t shared_mem_addr)
{
    if (shared_mem_addr != 0U) {
        s_shared_addr = shared_mem_addr;
    }

#if defined(TINY1C_HAS_CVI_MAILBOX)
    s_shared_data = (volatile shared_data_t *)s_shared_addr;
#else
    if (s_shared_addr == SHARED_MEM_BASE) {
        s_shared_data = (volatile shared_data_t *)s_host_shared_backing;
    } else {
        s_shared_data = (volatile shared_data_t *)s_shared_addr;
    }
#endif

    memset((void *)s_shared_data, 0, sizeof(shared_data_t));

    s_shared_data->magic        = IPC_MAGIC;
    s_shared_data->frame_width  = IMG_W;
    s_shared_data->frame_height = IMG_H;
    s_shared_data->frame_bytes  = FRAME_BYTES;
    s_shared_data->frame_mode   = (uint8_t)MODE_TEMP;
    s_shared_data->palette_id   = (uint8_t)PSEUDO_COLOR_MODE_1;

    ipc_flush_shared_cache((uintptr_t)s_shared_data, sizeof(shared_data_t));
    return 0;
}

shared_data_t *ipc_get_shared_data(void)
{
    return (shared_data_t *)s_shared_data;
}

int ipc_notify_frame_ready(void)
{
    if (s_shared_data == NULL) {
        return -1;
    }

    if ((s_shared_data->raw_frame_addr != 0U) && (s_shared_data->frame_bytes > 0U)) {
        ipc_flush_shared_cache((uintptr_t)s_shared_data->raw_frame_addr,
                               (size_t)s_shared_data->frame_bytes);
    }
    ipc_flush_shared_cache((uintptr_t)s_shared_data, sizeof(shared_data_t));

    cmdqu_t cmdq;
    memset(&cmdq, 0, sizeof(cmdqu_t));
    cmdq.ip_id     = IP_CAMERA;
    cmdq.cmd_id    = CMD_THERMAL_FRAME_READY;
    cmdq.block     = 0;
    cmdq.param_ptr = (unsigned int)s_shared_addr;

    return cvi_rtos_send_cmdqu(&cmdq);
}

int ipc_notify_cmd_done(uint8_t cmd_id, int32_t status)
{
    if (s_shared_data == NULL) {
        return -1;
    }

    s_shared_data->last_cmd_id     = cmd_id;
    s_shared_data->last_cmd_status = status;
    ipc_flush_shared_cache((uintptr_t)s_shared_data, sizeof(shared_data_t));

    cmdqu_t cmdq;
    memset(&cmdq, 0, sizeof(cmdqu_t));
    cmdq.ip_id     = IP_CAMERA;
    cmdq.cmd_id    = CMD_THERMAL_CMD_DONE;
    cmdq.block     = 0;
    cmdq.param_ptr = (unsigned int)((cmd_id & 0xFFU) | (((uint32_t)(status & 0xFFFFFF)) << 8U));

    return cvi_rtos_send_cmdqu(&cmdq);
}

static ir_error_t ipc_dispatch_command(uint8_t cmd_id, uint32_t param)
{
    if (s_shared_data == NULL) {
        return IR_ERROR_PARAM;
    }

    ipc_inv_shared_cache((uintptr_t)&s_shared_data->ctrl_params, sizeof(ipc_ctrl_params_t));
    const volatile ipc_ctrl_params_t *cp = &s_shared_data->ctrl_params;

    ir_error_t ret = IR_SUCCESS;

    switch (cmd_id) {
    case CMD_THERMAL_SHUTTER_REQ:
        ret = trigger_shutter();
        break;

    case CMD_THERMAL_SET_MODE:
        if (param <= (uint32_t)MODE_DUAL) {
            ret = core_start((camera_mode_t)param, 25U);
            if (ret == IR_SUCCESS) {
                s_shared_data->frame_mode   = (uint8_t)param;
                s_shared_data->frame_height = (param == (uint32_t)MODE_DUAL) ? DUAL_HEIGHT : IMG_H;
                s_shared_data->frame_bytes  = (param == (uint32_t)MODE_DUAL) ? DUAL_FRAME_BYTES : FRAME_BYTES;
            }
        } else {
            ret = IR_ERROR_PARAM;
        }
        break;

    case CMD_THERMAL_QUERY_EXTREMES: {
        extremes_t ext;
        ret = query_extremes(&ext);
        if (ret == IR_SUCCESS) {
            memcpy((void *)&s_shared_data->extremes, &ext, sizeof(extremes_t));
        }
        break;
    }

    case CMD_THERMAL_QUERY_POINT: {
        float pt_temp = 0.0f;
        uint16_t x = (param != 0U) ? (uint16_t)(param & 0xFFFFU) : cp->x0;
        uint16_t y = (param != 0U) ? (uint16_t)((param >> 16U) & 0xFFFFU) : cp->y0;
        ret = query_point_temp(x, y, &pt_temp);
        if (ret == IR_SUCCESS) {
            s_shared_data->region_result.avg_temp = pt_temp;
            s_shared_data->region_result.max_temp = pt_temp;
            s_shared_data->region_result.min_temp = pt_temp;
            s_shared_data->region_result.max_x    = x;
            s_shared_data->region_result.max_y    = y;
            s_shared_data->region_result.min_x    = x;
            s_shared_data->region_result.min_y    = y;
        }
        break;
    }

    case CMD_THERMAL_QUERY_LINE: {
        region_temp_t rinfo;
        ret = query_line_temp(cp->x0, cp->y0, cp->x1, cp->y1, &rinfo);
        if (ret == IR_SUCCESS) {
            memcpy((void *)&s_shared_data->region_result, &rinfo, sizeof(region_temp_t));
        }
        break;
    }

    case CMD_THERMAL_QUERY_RECT: {
        region_temp_t rinfo;
        ret = query_rect_temp(cp->x0, cp->y0, cp->x1, cp->y1, &rinfo);
        if (ret == IR_SUCCESS) {
            memcpy((void *)&s_shared_data->region_result, &rinfo, sizeof(region_temp_t));
        }
        break;
    }

    case CMD_THERMAL_SET_PALETTE:
        ret = set_palette((uint8_t)(param & 0xFFU));
        if (ret == IR_SUCCESS) {
            s_shared_data->palette_id = (uint8_t)(param & 0xFFU);
        }
        break;

    case CMD_THERMAL_SET_ZOOM: {
        uint8_t zmode = cp->zoom_mode;
        enum zoom_scale_step zstep = ((cp->zoom_step >= ZOOM_STEP1) && (cp->zoom_step <= ZOOM_STEP4))
                                         ? (enum zoom_scale_step)cp->zoom_step
                                         : ZOOM_STEP1;
        switch (zmode) {
        case IPC_ZOOM_CENTER_UP:
            ret = core_zoom_center_up(zstep);
            break;
        case IPC_ZOOM_CENTER_DOWN:
            ret = core_zoom_center_down(zstep);
            break;
        case IPC_ZOOM_POSITION_UP:
            ret = core_zoom_position_up(zstep, cp->x0, cp->y0);
            break;
        case IPC_ZOOM_POSITION_DOWN:
            ret = core_zoom_position_down(zstep, cp->x0, cp->y0);
            break;
        default:
            ret = IR_ERROR_PARAM;
            break;
        }
        break;
    }

    case CMD_THERMAL_SET_IMAGE_PARAM: {
        enum prop_image_params iparam = (param != 0U)
                                            ? (enum prop_image_params)((param >> 16U) & 0xFFFFU)
                                            : (enum prop_image_params)cp->param_id;
        uint16_t ival = (param != 0U) ? (uint16_t)(param & 0xFFFFU) : cp->param_val;
        ret = core_set_image_param(iparam, ival);
        break;
    }

    case CMD_THERMAL_SET_MIRROR_FLIP:
        ret = core_set_mirror_flip((enum mirror_flip_types)(param & 0xFFU));
        break;

    case CMD_THERMAL_DPC_AUTO:
        ret = core_dpc_auto_calibrate((uint16_t)(param & 0xFFFFU));
        break;

    case CMD_THERMAL_DPC_ADD_POINT: {
        uint16_t x = (param != 0U) ? (uint16_t)(param & 0xFFFFU) : cp->x0;
        uint16_t y = (param != 0U) ? (uint16_t)((param >> 16U) & 0xFFFFU) : cp->y0;
        ret = core_dpc_add_point(x, y);
        break;
    }

    case CMD_THERMAL_DPC_REMOVE_POINT: {
        uint16_t x = (param != 0U) ? (uint16_t)(param & 0xFFFFU) : cp->x0;
        uint16_t y = (param != 0U) ? (uint16_t)((param >> 16U) & 0xFFFFU) : cp->y0;
        ret = core_dpc_remove_point(x, y);
        break;
    }

    case CMD_THERMAL_RMCOVER_CALC:
        ret = core_rmcover_auto_calc((uint8_t)(param & 0xFFU));
        break;

    case CMD_THERMAL_RMCOVER_ENABLE:
        ret = core_rmcover_enable(param != 0U);
        break;

    case CMD_THERMAL_RECAL_1POINT:
        ret = core_recal_1point_kelvin((uint16_t)(param & 0xFFFFU));
        break;

    case CMD_THERMAL_RECAL_2POINT_P1:
        ret = core_recal_2point_kelvin(TPD_KTBT_RECAL_P1, (uint16_t)(param & 0xFFFFU));
        break;

    case CMD_THERMAL_RECAL_2POINT_P2:
        ret = core_recal_2point_kelvin(TPD_KTBT_RECAL_P2, (uint16_t)(param & 0xFFFFU));
        break;

    case CMD_THERMAL_SET_TPD_PARAM: {
        enum prop_tpd_params tparam = (param != 0U)
                                          ? (enum prop_tpd_params)((param >> 16U) & 0xFFFFU)
                                          : (enum prop_tpd_params)cp->param_id;
        uint16_t tval = (param != 0U) ? (uint16_t)(param & 0xFFFFU) : cp->param_val;
        ret = core_set_tpd_param(tparam, tval);
        break;
    }

    case CMD_THERMAL_SET_ENV_PARAMS:
        ret = core_set_env_params(cp->emissivity, cp->distance_m,
                                  cp->refl_temp_c, cp->atm_temp_c,
                                  cp->transmittance);
        break;

    case CMD_THERMAL_SET_GAIN_MODE:
        ret = core_set_gain_mode(param != 0U);
        break;

    case CMD_THERMAL_SAVE_CONFIG:
        ret = core_save_config((enum prop_spi_cfg_module)(param & 0xFFU));
        break;

    case CMD_THERMAL_RESTORE_DEFAULT:
        ret = core_restore_default_cfg((enum prop_default_cfg)(param & 0xFFU));
        break;

    default:
        ret = IR_UNSUPPORTED_CMD;
        break;
    }

    return ret;
}

void ipc_process_linux_cmds(void)
{
#if defined(TINY1C_HAS_CVI_MAILBOX) && defined(THERMAL_HAS_COMM_HDR)
    QueueHandle_t cam_q = main_GetMODHandle(E_QUEUE_CAMERA);
    if (cam_q == NULL) {
        return;
    }

    cmdqu_t cmdq;
    while (xQueueReceive(cam_q, &cmdq, 0U) == pdPASS) {
        if (cmdq.ip_id != IP_CAMERA) {
            continue;
        }
        ir_error_t status = ipc_dispatch_command(cmdq.cmd_id, cmdq.param_ptr);
        (void)ipc_notify_cmd_done(cmdq.cmd_id, (int32_t)status);
    }
#else
    (void)ipc_dispatch_command;
#endif
}
