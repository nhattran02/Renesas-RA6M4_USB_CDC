/*
 * commandSupport.c
 *
 *  Created on: Jun 25, 2023
 *      Author: nhat.tranminh
 */

#include "commandSupport.h"
#include "hal_data.h"
#include "usb_pcdc.h"

void AT_LED_HANDLER(void *params);
void AT_BLINK_HANDLER(void *params);
void AT_TIME_HANDLER(void *params);

CDC_Commands_Type CDC_Command_Table[] =
{
{ "AT+LED\r\n", 0, AT_LED_HANDLER },
{ "AT+BLINK\r\n", 0, AT_BLINK_HANDLER },
{ "AT+TIME\r\n", 0, AT_TIME_HANDLER},
};

/*************************************
 * ***** Local Functions    **********
 *************************************/

uint32_t hashFunc(char *str)
{
    if(str == NULL){
        return 0; // check NULL pointer
    }
    uint8_t len = (uint8_t)strlen(str);
    uint32_t sum = 0;
    for (uint8_t i = 0; i < len; ++i){
        sum += i*((uint32_t)str[i]);
    }
    return sum * len;
}

void initHashTable(void)
{
    uint8_t numOfCmds = sizeof(CDC_Command_Table) / sizeof(CDC_Command_Table[0]);
    for (uint8_t i = 0; i < numOfCmds; i++)
    {
        CDC_Command_Table[i].hash = hashFunc(CDC_Command_Table[i].cmd);
    }
}


uint8_t hashTableLookup(char * cmd){
    uint32_t hash = hashFunc(cmd);
    uint8_t number_of_commands = sizeof(CDC_Command_Table)/sizeof(CDC_Command_Table[0]);
    for(uint8_t i = 0; i < number_of_commands ; ++i){
        if(hash == CDC_Command_Table[i].hash){
            return i + 1; // this is the command index;
        }
    }
    //Command not found
    return 0;
}

void AT_LED_HANDLER(void *params)
{
    (void)params;
    usbTransmit((const uint8_t *)"AT_LED_HANDLER() is called\r\n", (const uint16_t)sizeof("AT_LED_HANDLER() is called\r\n"));
}

void AT_BLINK_HANDLER(void *params){
    (void)params;
    usbTransmit((const uint8_t *)"AT_BLINK_HANDLER() is called\r\n", (const uint16_t)sizeof("AT_BLINK_HANDLER() is called\r\n"));
}

void AT_TIME_HANDLER(void *params){
    (void)params;
    usbTransmit((const uint8_t *)"AT_TIME_HANDLER() is called\r\n", (const uint16_t)sizeof("AT_TIME_HANDLER() is called\r\n"));
}
