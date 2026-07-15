// app_queues.h

#ifndef APP_QUEUES_H
#define APP_QUEUES_H

#include "cmsis_os.h"

extern osMessageQueueId_t usbTxQueueHandle;
extern osMessageQueueId_t usbRxQueueHandle;

#endif