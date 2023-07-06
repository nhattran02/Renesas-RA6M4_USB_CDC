#include "read_buffer_thread.h"
#include "conf_usb.h"
#include "usb_pcdc.h"
#include "usb_hcdc.h"

/* Read Buffer entry function */
/* pvParameters contains TaskHandle_t */
void read_buffer_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    read_buffer_task();
    while (1)
    {
        vTaskDelay (1);
    }
}
