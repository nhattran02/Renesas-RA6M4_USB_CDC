/*
 * conf_usb.h
 *
 *  Created on: Jun 19, 2023
 *      Author: nhat.tranminh
 */

#ifndef DBG_H_
#define DBG_H_

/* generic headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_data.h"
#include "conf_usb.h"
#include "SEGGER_RTT.h"


#define BIT_SHIFT_8  (8u)
#define SIZE_64      (64u)

#define LVL_ERR      (1u)       /* error conditions   */

#define RESET_VALUE             (0x00)



#define SEGGER_INDEX            (0)

#define DB_PRINTF(fn_, ...)      SEGGER_RTT_printf (SEGGER_INDEX,(fn_), ##__VA_ARGS__);

#define DB_ERR_PRINTF(fn_, ...)  if(LVL_ERR)\
        SEGGER_RTT_printf (SEGGER_INDEX, "[ERR] In Function: %s(), %s",__FUNCTION__,(fn_),##__VA_ARGS__);

#define DB_ERR_TRAP(err)        if(err) {\
        SEGGER_RTT_printf(SEGGER_INDEX, "\r\nReturned Error Code: 0x%x  \r\n", err);\
        __asm("BKPT #0\n");} /* trap upon the error  */

#define DB_READ(read_data)     SEGGER_RTT_Read (SEGGER_INDEX, read_data, sizeof(read_data));

#define DB_CHECK_DATA          SEGGER_RTT_HasKey()


#endif /* DBG_H_ */
