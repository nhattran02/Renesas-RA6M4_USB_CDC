/*
 * usb_pcdc.h
 *
 *  Created on: Jun 18, 2023
 *      Author: nhat.tranminh
 */

#ifndef USB_PCDC_H_
#define USB_PCDC_H_

#include "dbg.h"

#define NUM_STRING_DESCRIPTOR               (7U)
#define READ_BUF_SIZE                       (512U)
#define LINE_CODING_LENGTH                  (0x07U) // Line coding length
#define NO_WAIT_TIME                        0

/*Define debug macro*/
#if defined(DB_USB_PCDC_EN)
#define USB_PCDC_PRINTF(level, ...)    do{\
    if(level >= DB_USB_PCDC_EN){\
    	DB_PRINTF(__VA_ARGS__);\
    }\
}while(0)

#define USB_PCDC_ERR_PRINTF(level, ...)    do{\
    if(level >= DB_USB_PCDC_EN){\
        DB_ERR_PRINTF(__VA_ARGS__);\
    }\
}while(0)

#define USB_PCDC_ERR_TRAP(level, err)    do{\
    if(level >= DB_USB_PCDC_EN){\
        DB_ERR_TRAP(err);\
    }\
}while(0)
#endif


/* Private functions */
fsp_err_t usbTransmit(const uint8_t *pBuffer, const uint16_t numBytes);
void usb_pcdc_task(void);
void read_buffer_task(void);
void process_data_task(void);

#endif /* USB_PCDC_USB_PCDC_H_ */
