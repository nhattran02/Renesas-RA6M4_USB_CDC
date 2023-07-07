/*
 * usbLog.c
 *
 *  Created on: Jul 6, 2023
 *      Author: nhat.tranminh
 */

#include "usbLog.h"
#include "printf_util.h"
#include <usb_thread.h>
#include "stdio.h"

#define BUFFER_SIZE 512

int usbTransmit(const uint8_t *pBuffer, const uint16_t numBytes)
{
    int ret = R_USB_Write (&g_basic0_ctrl, (uint8_t*) pBuffer, numBytes, USB_CLASS_PCDC);

    /* Handle error */
    if (ret != 0){
        return -1;
    }
    return 0;
}


int USB_CDC_printf(const char *sFormat, ...)
{
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, sFormat);

    int len = vsnprintf_(buffer, BUFFER_SIZE, sFormat, args);

    va_end(args);
    if(len >= BUFFER_SIZE){
        return -1;
    }
    return usbTransmit((const uint8_t *)buffer, (const uint16_t)len);
}
