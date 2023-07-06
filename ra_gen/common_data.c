/* generated common source file - do not edit */
#include "common_data.h"
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_ctrl = &g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };
SemaphoreHandle_t g_usb_read_cpt_sem;
#if 1
StaticSemaphore_t g_usb_read_cpt_sem_memory;
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
SemaphoreHandle_t g_usb_write_cpt_sem;
#if 1
StaticSemaphore_t g_usb_write_cpt_sem_memory;
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_hcdc_queue;
#if 1
StaticQueue_t g_hcdc_queue_memory;
uint8_t g_hcdc_queue_queue_memory[4 * 20];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t usb_cmd_queue;
#if 1
StaticQueue_t usb_cmd_queue_memory;
uint8_t usb_cmd_queue_queue_memory[512 * 20];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
void g_common_init(void)
{
    g_usb_read_cpt_sem =
#if 1
            xSemaphoreCreateBinaryStatic (&g_usb_read_cpt_sem_memory);
#else
                xSemaphoreCreateBinary();
                #endif
    if (NULL == g_usb_read_cpt_sem)
    {
        rtos_startup_err_callback (g_usb_read_cpt_sem, 0);
    }
    g_usb_write_cpt_sem =
#if 1
            xSemaphoreCreateBinaryStatic (&g_usb_write_cpt_sem_memory);
#else
                xSemaphoreCreateBinary();
                #endif
    if (NULL == g_usb_write_cpt_sem)
    {
        rtos_startup_err_callback (g_usb_write_cpt_sem, 0);
    }
    g_hcdc_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                20,
                                4
#if 1
                                ,
                                &g_hcdc_queue_queue_memory[0], &g_hcdc_queue_memory
#endif
                                );
    if (NULL == g_hcdc_queue)
    {
        rtos_startup_err_callback (g_hcdc_queue, 0);
    }
    usb_cmd_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                20,
                                512
#if 1
                                ,
                                &usb_cmd_queue_queue_memory[0], &usb_cmd_queue_memory
#endif
                                );
    if (NULL == usb_cmd_queue)
    {
        rtos_startup_err_callback (usb_cmd_queue, 0);
    }
}
