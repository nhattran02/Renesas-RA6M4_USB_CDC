/*
 * conf_usb.h
 *
 *  Created on: Jun 19, 2023
 *      Author: nhat.tranminh
 */

#ifndef CONFIGS_CONF_USB_H_
#define CONFIGS_CONF_USB_H_

#define BAUD_RATE_115200    (115200ul)
#define BAUD_RATE_9600      (9600ul)

/* Define Platform */
#define RENESAS_PLATFORM


/* Enable USB HCDC or PCDC */
#define USB_MODE            1 // 1 for PCDC - 0 for HCDC

/* Debug levels */
#define DB_LEVEL_INFO       0
#define DB_LEVEL_WARN       1
#define DB_LEVEL_CRITICAL   2

/*Change debug level for features*/
#define DB_USB_PCDC_EN      DB_LEVEL_INFO
#define DB_USB_HCDC_EN      DB_LEVEL_INFO



#endif /* CONFIGS_CONF_USB_H_ */
