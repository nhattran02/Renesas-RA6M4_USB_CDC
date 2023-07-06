/* generated thread source file - do not edit */
#include "usb_thread.h"

#if 1
static StaticTask_t usb_thread_memory;
#if defined(__ARMCC_VERSION)           /* AC6 compiler */
                static uint8_t usb_thread_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
                #else
static uint8_t usb_thread_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.usb_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif
#endif
TaskHandle_t usb_thread;
void usb_thread_create(void);
static void usb_thread_func(void *pvParameters);
void rtos_startup_err_callback(void *p_instance, void *p_data);
void rtos_startup_common_init(void);
usb_instance_ctrl_t g_basic1_ctrl;

#if !defined(NULL)
extern usb_descriptor_t NULL;
#endif
#define RA_NOT_DEFINED (1)
const usb_cfg_t g_basic1_cfg =
{ .usb_mode = USB_MODE_HOST,
  .usb_speed = USB_SPEED_FS,
  .module_number = 0,
  .type = USB_CLASS_HCDC,
#if defined(NULL)
                .p_usb_reg = NULL,
#else
  .p_usb_reg = &NULL,
#endif
  .usb_complience_cb = NULL,
#if defined(VECTOR_NUMBER_USBFS_INT)
                .irq       = VECTOR_NUMBER_USBFS_INT,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_RESUME)
                .irq_r     = VECTOR_NUMBER_USBFS_RESUME,
#else
  .irq_r = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_FIFO_0)
                .irq_d0    = VECTOR_NUMBER_USBFS_FIFO_0,
#else
  .irq_d0 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_FIFO_1)
                .irq_d1    = VECTOR_NUMBER_USBFS_FIFO_1,
#else
  .irq_d1 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_USB_INT_RESUME)
                .hsirq     = VECTOR_NUMBER_USBHS_USB_INT_RESUME,
#else
  .hsirq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_FIFO_0)
                .hsirq_d0  = VECTOR_NUMBER_USBHS_FIFO_0,
#else
  .hsirq_d0 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_FIFO_1)
                .hsirq_d1  = VECTOR_NUMBER_USBHS_FIFO_1,
#else
  .hsirq_d1 = FSP_INVALID_VECTOR,
#endif
  .ipl = (12),
  .ipl_r = (12),
  .ipl_d0 = (12),
  .ipl_d1 = (12),
  .hsipl = (BSP_IRQ_DISABLED),
  .hsipl_d0 = (BSP_IRQ_DISABLED),
  .hsipl_d1 = (BSP_IRQ_DISABLED),
#if (BSP_CFG_RTOS != 0)
                .p_usb_apl_callback = NULL,
#else
  .p_usb_apl_callback = NULL,
#endif
#if defined(NULL)
                .p_context = NULL,
#else
  .p_context = &NULL,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#else
                .p_transfer_tx = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#else
                .p_transfer_rx = &RA_NOT_DEFINED,
#endif
        };
#undef RA_NOT_DEFINED

/* Instance structure to use this module. */
const usb_instance_t g_basic1 =
{ .p_ctrl = &g_basic1_ctrl, .p_cfg = &g_basic1_cfg, .p_api = &g_usb_on_usb, };

usb_instance_ctrl_t g_basic0_ctrl;

#if !defined(g_usb_descriptor)
extern usb_descriptor_t g_usb_descriptor;
#endif
#define RA_NOT_DEFINED (1)
const usb_cfg_t g_basic0_cfg =
{ .usb_mode = USB_MODE_PERI,
  .usb_speed = USB_SPEED_FS,
  .module_number = 0,
  .type = USB_CLASS_PCDC,
#if defined(g_usb_descriptor)
                .p_usb_reg = g_usb_descriptor,
#else
  .p_usb_reg = &g_usb_descriptor,
#endif
  .usb_complience_cb = NULL,
#if defined(VECTOR_NUMBER_USBFS_INT)
                .irq       = VECTOR_NUMBER_USBFS_INT,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_RESUME)
                .irq_r     = VECTOR_NUMBER_USBFS_RESUME,
#else
  .irq_r = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_FIFO_0)
                .irq_d0    = VECTOR_NUMBER_USBFS_FIFO_0,
#else
  .irq_d0 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_FIFO_1)
                .irq_d1    = VECTOR_NUMBER_USBFS_FIFO_1,
#else
  .irq_d1 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_USB_INT_RESUME)
                .hsirq     = VECTOR_NUMBER_USBHS_USB_INT_RESUME,
#else
  .hsirq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_FIFO_0)
                .hsirq_d0  = VECTOR_NUMBER_USBHS_FIFO_0,
#else
  .hsirq_d0 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_FIFO_1)
                .hsirq_d1  = VECTOR_NUMBER_USBHS_FIFO_1,
#else
  .hsirq_d1 = FSP_INVALID_VECTOR,
#endif
  .ipl = (12),
  .ipl_r = (12),
  .ipl_d0 = (12),
  .ipl_d1 = (12),
  .hsipl = (BSP_IRQ_DISABLED),
  .hsipl_d0 = (BSP_IRQ_DISABLED),
  .hsipl_d1 = (BSP_IRQ_DISABLED),
#if (BSP_CFG_RTOS != 0)
                .p_usb_apl_callback = usb_pcdc_callback,
#else
  .p_usb_apl_callback = NULL,
#endif
#if defined(NULL)
                .p_context = NULL,
#else
  .p_context = &NULL,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#else
                .p_transfer_tx = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#else
                .p_transfer_rx = &RA_NOT_DEFINED,
#endif
        };
#undef RA_NOT_DEFINED

/* Instance structure to use this module. */
const usb_instance_t g_basic0 =
{ .p_ctrl = &g_basic0_ctrl, .p_cfg = &g_basic0_cfg, .p_api = &g_usb_on_usb, };

extern uint32_t g_fsp_common_thread_count;

const rm_freertos_port_parameters_t usb_thread_parameters =
{ .p_context = (void*) NULL, };

void usb_thread_create(void)
{
    /* Increment count so we will know the number of threads created in the RA Configuration editor. */
    g_fsp_common_thread_count++;

    /* Initialize each kernel object. */

#if 1
    usb_thread = xTaskCreateStatic (
#else
                    BaseType_t usb_thread_create_err = xTaskCreate(
                    #endif
                                    usb_thread_func,
                                    (const char*) "USB Thread", 1024 / 4, // In words, not bytes
                                    (void*) &usb_thread_parameters, //pvParameters
                                    1,
#if 1
                                    (StackType_t*) &usb_thread_stack,
                                    (StaticTask_t*) &usb_thread_memory
#else
                        & usb_thread
                        #endif
                                    );

#if 1
    if (NULL == usb_thread)
    {
        rtos_startup_err_callback (usb_thread, 0);
    }
#else
                    if (pdPASS != usb_thread_create_err)
                    {
                        rtos_startup_err_callback(usb_thread, 0);
                    }
                    #endif
}
static void usb_thread_func(void *pvParameters)
{
    /* Initialize common components */
    rtos_startup_common_init ();

    /* Initialize each module instance. */

#if (1 == BSP_TZ_NONSECURE_BUILD) && (1 == 1)
                    /* When FreeRTOS is used in a non-secure TrustZone application, portALLOCATE_SECURE_CONTEXT must be called prior
                     * to calling any non-secure callable function in a thread. The parameter is unused in the FSP implementation.
                     * If no slots are available then configASSERT() will be called from vPortSVCHandler_C(). If this occurs, the
                     * application will need to either increase the value of the "Process Stack Slots" Property in the rm_tz_context
                     * module in the secure project or decrease the number of threads in the non-secure project that are allocating
                     * a secure context. Users can control which threads allocate a secure context via the Properties tab when
                     * selecting each thread. Note that the idle thread in FreeRTOS requires a secure context so the application
                     * will need at least 1 secure context even if no user threads make secure calls. */
                     portALLOCATE_SECURE_CONTEXT(0);
                    #endif

    /* Enter user code for this thread. Pass task handle. */
    usb_thread_entry (pvParameters);
}
