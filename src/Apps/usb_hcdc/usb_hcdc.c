/*
 * usb_hcdc.c
 *
 *  Created on: Jun 22, 2023
 *      Author: nhat.tranminh
 */
#include "usb_hcdc.h"
#include "r_usb_cstd_rtos.h"

/*******************************************************************************************************************//**
 * @addtogroup usb_hcdc_ep
 * @{
 **********************************************************************************************************************/
/** Private variables **/
static usb_hcdc_linecoding_t g_serial_state;
static usb_hcdc_linecoding_t g_com_parm;
static uint8_t  g_snd_buf[8] BSP_ALIGN_VARIABLE(4) = {RESET_VALUE};         //Send buffer
static uint8_t  g_rcv_buf[512] BSP_ALIGN_VARIABLE(4) = {RESET_VALUE};       //Receive buffer
static uint8_t  g_usb_dummy = RESET_VALUE;                                  //dummy variable to send
static volatile bool g_err_flag = false;                                    //flag bit

char welcome[200] = {"\r\nWelcome to USB HCDC project for Renesas EK-RA6M4!\r\n"};


void usb_hcdc_task(void)
{
    fsp_err_t err = FSP_SUCCESS;
    static usb_event_info_t *event_info = NULL;
    BaseType_t err_queue = pdFALSE;
    memset(&g_serial_state, RESET_VALUE, sizeof(g_serial_state));
    memset(&g_com_parm, RESET_VALUE, sizeof(g_com_parm));
    g_snd_buf[ZERO_INDEX] = '1';
    USB_HCDC_PRINTF(DB_LEVEL_INFO, welcome);

    /* OpenUSB */
    err = R_USB_Open(&g_basic1_ctrl, &g_basic1_cfg);
    if(err != FSP_SUCCESS)
    {
        USB_HCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, "\r\nError in opening USB Host CDC\r\n");
    }

    while(1)
    {
        /* Handle error if queue send fails*/
        if(g_err_flag) //queue send fail
        {
            handle_error(g_err_flag, "Error in sending usb event through queue");
        }
        /* Receive message from queue and analyzing the received message*/
        err_queue = xQueueReceive(g_hcdc_queue, &event_info, portMAX_DELAY);
        if(err_queue != FSP_SUCCESS)
        {
            handle_error(err_queue, "Error in receiving USB event message through queue");
        }
        switch(event_info->event)
        {
            case USB_STATUS_CONFIGURED:
            {
                set_line_coding(&g_basic1_ctrl, event_info->device_address);
            }
            break;
            case USB_STATUS_READ_COMPLETE:
            {
                usb_data_process(event_info);
            }
            break;
            case USB_STATUS_WRITE_COMPLETE:
            {
                err = R_USB_Read(&g_basic1_ctrl, g_rcv_buf, CDC_READ_DATA_LEN, event_info->device_address);
                if(err != FSP_SUCCESS)
                {
                    handle_error(err, "**R_USB_Read API Failed**");
                }
                USB_HCDC_PRINTF(DB_LEVEL_INFO, "\r\nReceive data from device: %s\r\n", g_rcv_buf);
            }
            break;
            case USB_STATUS_REQUEST_COMPLETE:
            {
                if((event_info->setup.request_type & USB_BREQUEST) == USB_CDC_SET_LINE_CODING)
                {
                    set_control_line_state(&g_basic1_ctrl, event_info->device_address);
                }
                else if((event_info->setup.request_type & USB_BREQUEST) == USB_CDC_SET_CONTROL_LINE_STATE)
                {
                    get_line_coding(&g_basic1_ctrl, event_info->device_address);
                }
                else if((event_info->setup.request_type & USB_BREQUEST) == USB_CDC_GET_LINE_CODING)
                {
                    err = R_USB_Write(&g_basic1_ctrl, g_snd_buf, CDC_WRITE_DATA_LEN, event_info->device_address);
                    if(err != FSP_SUCCESS)
                    {
                        handle_error(err, "**R_USB_Write API Failed**");
                    }
                }
                else
                {
                    /*None*/
                }
            }
            break;
            default:
            {
                /* None */
            }
            break;
        }
    }
}


/*******************************************************************************************************************//**
 * @brief In this function initializes to set control line state information for host control transfer.
 * @param[IN]   p_ctrl         Pointer to control structure
 * @param[IN]   device_address         USB device address
 * @retval      None.
 ***********************************************************************************************************************/
void set_control_line_state (usb_instance_ctrl_t * p_ctrl, uint8_t device_address)
{
    usb_setup_t setup;
    fsp_err_t err = FSP_SUCCESS;
    setup.request_type      = SET_CONTROL_LINE_STATE;       /* bRequestCode:SET_CONTROL_LINE_STATE, bmRequestType */
    setup.request_value     = VALUE_ZERO;                   /* wValue:Zero */
    setup.request_index     = VALUE_ZERO;                   /* wIndex:Interface */
    setup.request_length    = VALUE_ZERO;                   /* wLength:Zero */

    err = R_USB_HostControlTransfer(p_ctrl, &setup, &g_usb_dummy, device_address);
    if (err != FSP_SUCCESS)
    {
        handle_error (err, "**R_USB_HOSTControlTransfer API FAILED**");
    }

} /* End of function cdc_set_control_line_state */

/*******************************************************************************************************************//**
 * @brief In this function initializes to set line coding information for host control transfer.
 * @param[IN]   p_ctrl         Pointer to control structure
 * @param[IN]   device_address  USB device address
 * @retval      None.
 ***********************************************************************************************************************/
 void set_line_coding (usb_instance_ctrl_t * p_ctrl, uint8_t device_address)
{
     usb_setup_t setup;
     fsp_err_t err = FSP_SUCCESS;
     g_com_parm.dwdte_rate   = (uint32_t)USB_HCDC_SPEED_9600;
     g_com_parm.bdata_bits   = USB_HCDC_DATA_BIT_8;
     g_com_parm.bchar_format = USB_HCDC_STOP_BIT_1;
     g_com_parm.bparity_type = USB_HCDC_PARITY_BIT_NONE;


     setup.request_type      = SET_LINE_CODING;        /* bRequestCode:SET_LINE_CODING, bmRequestType */
     setup.request_value     = VALUE_ZERO;             /* wValue:Zero */
     setup.request_index     = VALUE_ZERO;             /* wIndex:Interface */
     setup.request_length    = LINE_CODING_LENGTH;     /* Data:Line Coding Structure */

     /* Request Control transfer */
     err = R_USB_HostControlTransfer(p_ctrl, &setup, (uint8_t *)&g_com_parm, device_address);
     if (err != FSP_SUCCESS)
     {
         handle_error (err, "**R_USB_HostControlTransfer API FAILED**");
     }
} /* End of function cdc_set_line_coding */


 /*******************************************************************************************************************//**
  * @brief In this function initializes to get line coding information for host control transfer.
  * @param[IN]   p_ctrl                pointer to control structure
  * @param[IN]   device_address         USB device address
  * @retval      None.
  ***********************************************************************************************************************/
void get_line_coding (usb_instance_ctrl_t * p_ctrl, uint8_t device_address)
{
    usb_setup_t setup;
    fsp_err_t err = FSP_SUCCESS;
    setup.request_type      = GET_LINE_CODING;      /* bRequestCode:GET_LINE_CODING, bmRequestType */
    setup.request_value     = VALUE_ZERO;               /* wValue:Zero */
    setup.request_index     = VALUE_ZERO;               /* wIndex:Interface */
    setup.request_length    = LINE_CODING_LENGTH;   /* Data:Line Coding Structure */

    /* Request Control transfer */
    err = R_USB_HostControlTransfer(p_ctrl, &setup, (uint8_t *)&g_com_parm, device_address);
    if (err != FSP_SUCCESS)
    {
        handle_error (err, "**R_USB_HostControlTransfer API FAILED**");
    }

} /* End of function cdc_get_line_coding */

/*******************************************************************************************************************//**
 * @brief  This function is called to do closing of usb module using its HAL level API and handles the error trap.
 *  Handle the Error internally with Proper Message. Application handles the rest.
 * @param[IN] err       Return values of APIs
 * @param[IN] err_str   Print message from the failed API call
 * @retval    None
 **********************************************************************************************************************/
void handle_error(fsp_err_t err, char *err_str)
{
    fsp_err_t error = FSP_SUCCESS;

    /* close opened USB module */
    error = R_USB_Close(&g_basic1_ctrl);
    /* Handle error */
    if(error != FSP_SUCCESS)
    {
        USB_HCDC_ERR_PRINTF (DB_LEVEL_CRITICAL, "**\r\n R_USB_Close API FAILED **\r\n");
    }
    USB_HCDC_ERR_PRINTF(DB_LEVEL_CRITICAL, err_str);
    USB_HCDC_ERR_TRAP(DB_LEVEL_CRITICAL, err);
} /* End of function handle_error() */


/*******************************************************************************************************************//**
 * @brief     This function is callback for FreeRTOS + HCDC and triggers when USB event occurs from the device.
 * @param[IN]   p_event_info    Triggered event
 * @param[IN]   cur_task        current task handle
 * @param[IN]   usb_state       State of USB
 * @retval      None.
 ***********************************************************************************************************************/
void usb_rtos_callback (usb_event_info_t *p_event_info, usb_hdl_t cur_task, usb_onoff_t usb_state)
{
    FSP_PARAMETER_NOT_USED (cur_task);
    FSP_PARAMETER_NOT_USED (usb_state);

    /* Send event received to queue */
    if ((xQueueSend(g_hcdc_queue, (const void *)&p_event_info, (TickType_t)(NO_WAIT_TIME))) != pdTRUE)
    {
        g_err_flag = true;
    }
} /* End of function usb_rtos_callback */


/*******************************************************************************************************************//**
  * @brief This function is to do data process with peripheral device
  * @param[IN]   event_info             data process in HCDC event type
  * @retval      None.
  ***********************************************************************************************************************/
void usb_data_process(usb_event_info_t *event_info)
{
    fsp_err_t err = FSP_SUCCESS;
    if (USB_CLASS_HCDC == event_info->type)
    {
        USB_HCDC_PRINTF(DB_LEVEL_INFO, "\r\nEvent Info Type: USB_CLASS_HCDC\r\n");
        if (RESET_VALUE < event_info->data_size)
        {
            /* Send the received data to USB Host */
            switch(g_snd_buf[ZERO_INDEX])
            {
                case '1':
                {
                    g_snd_buf[ZERO_INDEX]= '2';
                }
                break;
                case '2':
                {
                    g_snd_buf[ZERO_INDEX]= '\r';
                }
                break;
                case '\r':
                {
                    g_snd_buf[ZERO_INDEX]= '1';
                }
                break;
                default:
                {
                    /* No operation */
                }
                break;
            }

            err = R_USB_Write (&g_basic1_ctrl, g_snd_buf, CDC_WRITE_DATA_LEN, event_info->device_address);
            if (FSP_SUCCESS != err)
            {
                handle_error (err, "**R_USB_Write API FAILED**");
            }
            USB_HCDC_PRINTF(DB_LEVEL_INFO, "\r\n Sent data :%s", g_rcv_buf);
        }
        else
        {

            /* Send the data reception request when the zero-length packet is received. */
            err = R_USB_Read (&g_basic1_ctrl,g_rcv_buf, event_info->data_size,
                        event_info->device_address);
            if (FSP_SUCCESS != err)
            {
                handle_error (err, "**R_USB_Read API FAILED**");
            }
            USB_HCDC_PRINTF(DB_LEVEL_INFO, "\r\n Receive data :%s", g_rcv_buf);
        }
    }
    else
    {
        USB_HCDC_PRINTF(DB_LEVEL_INFO, "\r\nEvent Info Type: SerialState\r\n");
        /* Class notification "SerialState" receive start */
        err = R_USB_Read (&g_basic1_ctrl, (uint8_t *) &g_serial_state,
        USB_HCDC_SERIAL_STATE_MSG_LEN, event_info->device_address);
        /* Error Handle */
        if (FSP_SUCCESS != err)
        {
            handle_error (err, "**R_USB_Read API FAILED**");
        }
    }
} /* End of function usb_data_process */
