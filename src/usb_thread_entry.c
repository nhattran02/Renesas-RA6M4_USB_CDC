#include <usb_thread.h>
#include "conf_usb.h"
#include "usb_pcdc.h"
#include "usb_hcdc.h"

/* PCDC Thread entry function */
/* pvParameters contains TaskHandle_t */
void usb_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

#ifdef USB_MODE
#if USB_MODE == 1   //Peripheral Mode
    usb_pcdc_task();
#elif USB_MODE == 0 //Host Mode
    usb_hcdc_task();
#endif
#endif

    /* TODO: add your own code here */
    while (1)
    {
        vTaskDelay (1);
    }
}
