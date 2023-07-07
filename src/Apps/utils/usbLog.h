/*
 * usbLog.h
 *
 *  Created on: Jul 6, 2023
 *      Author: nhat.tranminh
 */

#ifndef APPS_UTILS_USBLOG_H_
#define APPS_UTILS_USBLOG_H_

#include "stdint.h"

int USB_CDC_printf(const char *sFormat, ...);
int usbTransmit(const uint8_t *pBuffer, const uint16_t numBytes);


#endif /* APPS_UTILS_USBLOG_H_ */
