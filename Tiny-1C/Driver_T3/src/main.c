/*
 * Tầng 3 (T3 - Tiny1C Application Task): Tiến trình thời gian thực thu nhận
 * khung hình nhiệt VoSPI và xử lý lệnh điều khiển I2C5 trên nhân RTOS (C906_1).
 */

#include "core.h"
#include "vospi.h"
#include "ipc.h"
#include "data_rw.h"
#include <stdio.h>
#include <string.h>

#if defined(__riscv) && defined(__has_include) && __has_include("FreeRTOS.h")
#define THERMAL_HAS_FREERTOS_HDR 1
#include "FreeRTOS.h"
#include "task.h"
#else
static inline void vTaskDelay(uint32_t ticks)
{
    (void)ticks;
}
#define pdMS_TO_TICKS(ms) (ms)
#define configMAX_PRIORITIES (5)
#endif

#ifndef THERMAL_I2C_BUS_DEFAULT
/* Mặc định dùng Bus 5 (Soft-I2C Tầng 2 trên GPIOA15 SCL / GPIOA27 SDA) */
#define THERMAL_I2C_BUS_DEFAULT     (5U)
#endif

#define THERMAL_TASK_STACK_SIZE     (4096U)
#define THERMAL_TASK_PRIORITY       (configMAX_PRIORITIES - 2)

/* Bộ đệm khung hình 192 KB căn chỉnh 64-byte Cache Line C906 */
static uint8_t s_frame_buffer[DUAL_FRAME_BYTES] __attribute__((aligned(64)));

void thermal_camera_task(void *pvParameters)
{
    (void)pvParameters;
    printf("=== Thermal Camera Tiny1C T3 Driver Task (T2 SPI0 + T2 Soft-I2C5) ===\n");

    /* 1. Khởi tạo Shared RAM cho cầu nối IPC giữa RTOS và CPU chính */
    (void)ipc_init(SHARED_MEM_BASE);
    shared_data_t *shared = ipc_get_shared_data();

    /* 2. Khởi tạo điều khiển I2C5 qua Tầng 2 (soft_i2c_init, Slave 7-bit 0x3C) */
    if (core_init(THERMAL_I2C_BUS_DEFAULT, I2C_SLAVE_ID) != IR_SUCCESS) {
        printf("[Bridge] Error: Failed to initialize Thermal Sensor Soft-I2C5!\n");
    }

    /* 3. Khởi tạo kênh truyền dữ liệu VoSPI qua Tầng 2 (spi_bus_init + spi_device_init) */
    if (spi_stream_init(0U, SPI0_DSI_CS_PIN_DEFAULT) != 0) {
        printf("[Bridge] Error: Failed to initialize SPI0 stream!\n");
    }

    /* 4. Kích hoạt camera ở chế độ ma trận nhiệt độ RAW16 @ 25 FPS */
    if (core_start(MODE_TEMP, 25U) != IR_SUCCESS) {
        printf("[Bridge] Error: Failed to start Thermal Camera preview!\n");
    }

    /* 5. Bật bảo vệ màn trập tự động khi rơi (Fall Protection) */
    (void)core_set_fall_protection(true, 1000U, 1000U);

    uint32_t frame_count = 0U;

    for (;;) {
        /* A. Xử lý các mệnh lệnh điều khiển do CPU chính gửi xuống qua Mailbox IPC */
        ipc_process_linux_cmds();

        /* B. Xác định chế độ hiện tại và kích thước Frame cần thu nhận qua VoSPI */
        camera_mode_t cur_mode = core_get_mode();
        uint8_t vospi_hdr      = (cur_mode == MODE_IMAGE) ? VOSPI_HEADER_IMAGE : VOSPI_HEADER_TEMP;
        uint32_t target_bytes  = (cur_mode == MODE_DUAL) ? DUAL_FRAME_BYTES : FRAME_BYTES;
        uint16_t target_height = (cur_mode == MODE_DUAL) ? DUAL_HEIGHT : IMG_H;

        /* C. Thu nhận 1 Frame từ camera qua Tầng 2 SPI0 (DMA 4KB auto-chunking) */
        int ret = spi_get_frame(s_frame_buffer, target_bytes, vospi_hdr);
        if (ret == 0) {
            frame_count++;

#if defined(THERMAL_HAS_FREERTOS_HDR)
            uint32_t current_ms = (uint32_t)xTaskGetTickCount() * portTICK_PERIOD_MS;
#else
            uint32_t current_ms = frame_count * 40U;
#endif

            const vospi_frame_header_t *hdr = spi_get_last_header();
            if (shared != NULL) {
                shared->raw_frame_addr = (uint32_t)(uintptr_t)s_frame_buffer;
                shared->frame_bytes    = target_bytes;
                shared->frame_seq      = frame_count;
                shared->timestamp_ms   = current_ms;
                shared->frame_width    = IMG_W;
                shared->frame_height   = target_height;
                shared->frame_mode     = (uint8_t)cur_mode;
                if (hdr != NULL) {
                    memcpy((void *)&shared->vospi_hdr, hdr, sizeof(vospi_frame_header_t));
                    shared->sensor_vtemp = hdr->vtemp;
                }
            }

            if ((frame_count % 50U) == 0U) {
                extremes_t ext;
                if (query_extremes(&ext) == IR_SUCCESS) {
                    if (shared != NULL) {
                        memcpy((void *)&shared->extremes, &ext, sizeof(extremes_t));
                    }
                }
            }

            (void)ipc_notify_frame_ready();

            if ((frame_count % 1500U) == 0U) {
                (void)trigger_shutter();
            }

            vTaskDelay(pdMS_TO_TICKS(1U));
        } else {
            vTaskDelay(pdMS_TO_TICKS(5U));
        }

#if !defined(__riscv)
        if (frame_count >= 1U) {
            break;
        }
#endif
    }
}

void select_output_mode(camera_mode_t mode)
{
    if ((mode == MODE_IMAGE) || (mode == MODE_TEMP) || (mode == MODE_DUAL)) {
        (void)core_start(mode, 25U);
    }
}

void start_thermal_camera_task(void)
{
#if defined(THERMAL_HAS_FREERTOS_HDR)
    (void)xTaskCreate(thermal_camera_task, "thermal_task",
                      THERMAL_TASK_STACK_SIZE, NULL,
                      THERMAL_TASK_PRIORITY, NULL);
#else
    thermal_camera_task(NULL);
#endif
}

int thermal_app_main(void)
{
    start_thermal_camera_task();
    return 0;
}

#if !defined(__riscv)
int main(void)
{
    return thermal_app_main();
}
#endif
