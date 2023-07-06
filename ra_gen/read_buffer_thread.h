/* generated thread header file - do not edit */
#ifndef READ_BUFFER_THREAD_H_
#define READ_BUFFER_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void read_buffer_thread_entry(void * pvParameters);
                #else
extern void read_buffer_thread_entry(void *pvParameters);
#endif
FSP_HEADER
FSP_FOOTER
#endif /* READ_BUFFER_THREAD_H_ */
