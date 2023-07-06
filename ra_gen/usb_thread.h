/* generated thread header file - do not edit */
#ifndef USB_THREAD_H_
#define USB_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void usb_thread_entry(void * pvParameters);
                #else
extern void usb_thread_entry(void *pvParameters);
#endif
#include "r_usb_basic.h"
#include "r_usb_basic_api.h"
#include "r_usb_hcdc_api.h"
#include "r_usb_pcdc_api.h"
FSP_HEADER
/* Basic on USB Instance. */
extern const usb_instance_t g_basic1;

/** Access the USB instance using these structures when calling API functions directly (::p_api is not used). */
extern usb_instance_ctrl_t g_basic1_ctrl;
extern const usb_cfg_t g_basic1_cfg;

#ifndef NULL
void NULL(void*);
#endif

#if 2 == BSP_CFG_RTOS
#ifndef NULL
void NULL(usb_event_info_t *, usb_hdl_t, usb_onoff_t);
#endif
#endif
/** CDC Driver on USB Instance. */
/* Basic on USB Instance. */
extern const usb_instance_t g_basic0;

/** Access the USB instance using these structures when calling API functions directly (::p_api is not used). */
extern usb_instance_ctrl_t g_basic0_ctrl;
extern const usb_cfg_t g_basic0_cfg;

#ifndef NULL
void NULL(void*);
#endif

#if 2 == BSP_CFG_RTOS
#ifndef usb_pcdc_callback
void usb_pcdc_callback(usb_event_info_t *, usb_hdl_t, usb_onoff_t);
#endif
#endif
/** CDC Driver on USB Instance. */
FSP_FOOTER
#endif /* USB_THREAD_H_ */
