/*
 * usb_pcdc.c
 *
 *  Created on: Jun 18, 2023
 *      Author: nhat.tranminh
 */

#include <LowLevel/DebugUtils/dbg.h>
#include "usb_pcdc.h"
#include <stdio.h>
#include <string.h>
#include <usb_thread.h>
#include <ringBuf.h>
#include <stringFunction.h>
#include <parse.h>
#include "commandSupport.h"
#include "usbLog.h"

/* Global variables */
extern uint8_t g_apl_device[];
extern uint8_t g_apl_configuration[];
extern uint8_t g_apl_hs_configuration[];
extern uint8_t g_apl_qualifier_descriptor[];
extern uint8_t *g_apl_string_table[];


const usb_descriptor_t usb_descriptor =
{ g_apl_device, /* Pointer to the device descriptor */
  g_apl_configuration, /* Pointer to the configuration descriptor for Full-speed */
  g_apl_hs_configuration, /* Pointer to the configuration descriptor for Hi-speed */
  g_apl_qualifier_descriptor, /* Pointer to the qualifier descriptor */
  g_apl_string_table, /* Pointer to the string descriptor table */
  NUM_STRING_DESCRIPTOR
};

usb_status_t usb_event;
fsp_err_t g_err = FSP_SUCCESS;
volatile usb_pcdc_linecoding_t g_line_coding;
usb_pcdc_ctrllinestate_t g_control_line_state = { .bdtr = 0, .brts = 0, };
ring_buffer_t ring_buffer;
size_t command_size;
static usb_status_t g_usb_status;
static uint8_t g_usb_buf[READ_BUF_SIZE] = {0};
static bool b_usb_attach = false;
cavATCommandParser CommandParser;

const char p_welcome[] = { "\r\n********************************************************\r\n"
                         "*                  USB Peripheral CDC                  *\r\n"
                         "********************************************************\r\n" };

void usb_pcdc_task(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Initilize ring buffer*/
    initRingBuffer(&ring_buffer);

    /* Open USB instance */
    err = R_USB_Open (&g_basic0_ctrl, &g_basic0_cfg);
    if (err != FSP_SUCCESS)
    {
        USB_PCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err);
    }
    USB_PCDC_PRINTF(DB_LEVEL_INFO, "\r\nUSB opened successfully.\r\n");

    while (1)
    {
       BaseType_t err_semaphore = xSemaphoreTake(g_usb_read_cpt_sem, portMAX_DELAY);
       if (err_semaphore != pdTRUE)
       {
           USB_PCDC_ERR_PRINTF(DB_LEVEL_INFO, "\r\nxSemaphoreTake on g_usb_read_cpt_sem Failed \r\n");
           USB_PCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err_semaphore);
       }

       err = R_USB_Read (&g_basic0_ctrl, g_usb_buf, READ_BUF_SIZE, USB_CLASS_PCDC);
       if (err != FSP_SUCCESS)
       {
           USB_PCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err);
       }

#if 0
    snprintf((char *)g_usb_buf, sizeof(g_usb_buf), "AT+EE\r\n");
#endif

        writeData(&ring_buffer, (const char *)g_usb_buf);
        xSemaphoreGive(g_data_available_sem);
        memset (g_usb_buf, 0, sizeof(g_usb_buf)); // Clean g_usb_buf
    }
}


void read_buffer_task(void)
{
    parseResult ret_parse;
    BaseType_t err_queue;
    bool err_buf;
    char read_data[RING_BUFFER_SIZE];
    while(1){
        xSemaphoreTake(g_data_available_sem, portMAX_DELAY);
        memset(read_data, 0, sizeof(read_data));
        err_buf = readData(&ring_buffer, RING_BUFFER_SIZE - 1, read_data);

        if(err_buf){
            cleanParserData(&CommandParser);
            ret_parse = parseData(&CommandParser, read_data, (uint16_t)strlen(read_data));

            if(ret_parse == VALID_AT){
                for(int i = 0; i < CommandParser.commandCount; i++){
                    err_queue = xQueueSend(usb_cmd_queue, (const void *)CommandParser.commands[i], (TickType_t)(NO_WAIT_TIME));
                    if (err_queue != pdTRUE)
                    {
                        USB_PCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\nFailed to send the command to queue\r\n");
                    }
                }
            }
        }else{
            USB_PCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\nFailed to read data from ring buffer\r\n");
        }
    }
}

void process_data_task(void)
{
    char cmdStr[512];
    char params[64];
    initHashTable();
    while(1){
        BaseType_t err_queue = xQueueReceive(usb_cmd_queue, (void * const)cmdStr, (TickType_t)portMAX_DELAY);
        if(err_queue != pdTRUE){
            USB_PCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\nxQueueReceive failed\r\n");
        }
        USB_PCDC_PRINTF(DB_LEVEL_INFO, "\r\nThe command is: %s\r\n", cmdStr);
        uint8_t index = hashTableLookup(cmdStr);
        if(index != 0){
            CDC_Command_Table[index - 1].pfunc(params);
        }else{
            USB_CDC_printf("%sis not supported\r\n", cmdStr);
        }
    }
}


/*******************************************************************************************************************//**
 * @brief       USB PCDC with freertos callback
 * @param[IN]   usb_event_info_t    usb event
 * @param[IN]   usb_hdl_t           task
 * @param[IN]   usb_onoff_t         state
 * @retval      None
 **********************************************************************************************************************/
void usb_pcdc_callback(usb_event_info_t *p_pcdc_event, usb_hdl_t task, usb_onoff_t state)
{
    FSP_PARAMETER_NOT_USED(task);
    FSP_PARAMETER_NOT_USED(state);
    fsp_err_t err = FSP_SUCCESS;
    switch (p_pcdc_event->event)
    {
        case USB_STATUS_REQUEST:
        {
            if ((p_pcdc_event->setup.request_type & USB_BREQUEST) == USB_PCDC_SET_LINE_CODING)
            {
                USB_PCDC_PRINTF(DB_LEVEL_INFO, "\r\nUSB STATUS : USB_STATUS_REQUEST \nRequest_Type: USB_PCDC_SET_LINE_CODING \r\n");
                err = R_USB_PeriControlDataGet (&g_basic0_ctrl, (uint8_t*) &g_line_coding, LINE_CODING_LENGTH);
                if (err != FSP_SUCCESS)
                {
                    USB_PCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\n**Error to set line coding**\r\n");
                    USB_PCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err);
                }
            }
            else if ((p_pcdc_event->setup.request_type & USB_BREQUEST) == USB_PCDC_GET_LINE_CODING)
            {
                USB_PCDC_PRINTF(DB_LEVEL_INFO, "\r\nUSB STATUS : USB_STATUS_REQUEST \nRequest_Type: USB_PCDC_GET_LINE_CODING \r\n");
                err = R_USB_PeriControlDataSet (&g_basic0_ctrl, (uint8_t*) &g_line_coding, LINE_CODING_LENGTH);
                if (err != FSP_SUCCESS)
                {
                    USB_PCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\n**Error to get line coding**\r\n");
                    USB_PCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err);
                }

            }
            else if ((p_pcdc_event->setup.request_type & USB_BREQUEST) == USB_PCDC_SET_CONTROL_LINE_STATE)
            {
                USB_PCDC_PRINTF(DB_LEVEL_INFO, "\r\nUSB STATUS : USB_STATUS_REQUEST \nRequest_Type: USB_PCDC_SET_CONTROL_LINE_STATE \r\n");
                err = R_USB_PeriControlStatusSet (&g_basic0_ctrl, USB_SETUP_STATUS_ACK);
                if (err != FSP_SUCCESS)
                {
                    USB_PCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\n**Error to set control line state**\r\n");
                    USB_PCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err);
                }
                usbTransmit((const uint8_t *)p_welcome, (uint16_t)strlen(p_welcome));
            }
            else
            {
                USB_PCDC_PRINTF(DB_LEVEL_INFO, "\r\nUnsupported USB class request.\r\n");
            }
            break;
        }

        case USB_STATUS_READ_COMPLETE:
        {
            USB_PCDC_PRINTF(DB_LEVEL_INFO, "\r\nUSB STATUS : USB_STATUS_READ_COMPLETE \r\n");
            BaseType_t err_semaphore = xSemaphoreGive(g_usb_read_cpt_sem);
            if (err_semaphore != pdTRUE)
            {
                USB_PCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\n xSemaphoreGive on g_usb_read_cpt_sem Failed \r\n");
                USB_PCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err_semaphore);
            }
            break;
        }

        case USB_STATUS_WRITE_COMPLETE:
        {
            USB_PCDC_PRINTF(DB_LEVEL_INFO, "\r\nUSB STATUS : USB_STATUS_WRITE_COMPLETE \r\n");
            break;
        }

        case USB_STATUS_CONFIGURED:
        {
            g_usb_status = USB_STATUS_CONFIGURED;
            break;
        }

        case USB_STATUS_REQUEST_COMPLETE:
        {
            if (g_usb_status == USB_STATUS_CONFIGURED)
            {
                err = R_USB_Read (&g_basic0_ctrl, g_usb_buf, READ_BUF_SIZE, USB_CLASS_PCDC);
                if (err != FSP_SUCCESS)
                {
                    USB_PCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\n**Read USB failed**\r\n");
                    USB_PCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err);
                }
                g_usb_status = USB_STATUS_REQUEST_COMPLETE;
            }
            break;
        }

        case USB_STATUS_DETACH:
        case USB_STATUS_SUSPEND:
        {
            b_usb_attach = false;
            memset (g_usb_buf, 0, sizeof(g_usb_buf));
            break;
        }
        case USB_STATUS_RESUME:
        {
            b_usb_attach = true;
            break;
        }
        default:
        {
            break;
        }
    }
}
