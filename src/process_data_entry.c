#include "process_data.h"
#include "conf_usb.h"
#include "usb_pcdc.h"
#include "usb_hcdc.h"

/* Process Data entry function */
/* pvParameters contains TaskHandle_t */
void process_data_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    process_data_task();
    while(1){
        vTaskDelay(1);
    }
}
