/*
 * usb_hcdc.h
 *
 *  Created on: Jun 22, 2023
 *      Author: nhat.tranminh
 */

#ifndef USB_HCDC_H_
#define USB_HCDC_H_

#include <usb_thread.h>
#include "dbg.h"

#define RESET_VALUE                 (0x00)
#define SET_LINE_CODING             (USB_CDC_SET_LINE_CODING | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE)
#define GET_LINE_CODING             (USB_CDC_GET_LINE_CODING | USB_DEV_TO_HOST | USB_CLASS | USB_INTERFACE)
#define SET_CONTROL_LINE_STATE      (USB_CDC_SET_CONTROL_LINE_STATE | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE)
#define LINE_CODING_LENGTH          (0x07U)
#define VALUE_ZERO                  (0x0000U)
#define NO_WAIT_TIME                0
#define ZERO_INDEX                  0
#define CDC_WRITE_DATA_LEN          1
#define CDC_READ_DATA_LEN           512


/*Define debug macro*/
#if defined(DB_USB_HCDC_EN)
#define USB_HCDC_PRINTF(level, ...)    do{\
    if(level >= DB_USB_HCDC_EN){\
        DB_PRINTF(__VA_ARGS__);\
    }\
}while(0)

#define USB_HCDC_ERR_PRINTF(level, ...)    do{\
    if(level >= DB_USB_HCDC_EN){\
        DB_ERR_PRINTF(__VA_ARGS__);\
    }\
}while(0)

#define USB_HCDC_ERR_TRAP(level, err)    do{\
    if(level >= DB_USB_HCDC_EN){\
        DB_ERR_TRAP(err);\
    }\
}while(0)
#endif


void set_line_coding (usb_instance_ctrl_t * p_ctrl, uint8_t device_address);
void set_control_line_state (usb_instance_ctrl_t * p_ctrl, uint8_t device_address);
void get_line_coding (usb_instance_ctrl_t * p_ctrl, uint8_t device_address);
void usb_data_process(usb_event_info_t *event_info);
void handle_error(fsp_err_t err, char *err_str);
void usb_hcdc_task(void);
void usb_rtos_callback (usb_event_info_t *p_event_info, usb_hdl_t cur_task, usb_onoff_t usb_state);

#endif /* USB_HCDC_H_ */
