/**
 * @file fr_queues_externs.h
 * @brief declaration of queue handles for USB communication
 *
 * @details This file contains the declaration of queue handles used for USB communication.
 *
 * @author Rozdymkacz
 * @date 2026-07-15  
 *
 * @copyright Copyright (c) 2026
 */
#ifndef APP_QUEUES_H
#define APP_QUEUES_H

#include "cmsis_os.h"

extern osMessageQueueId_t usbTxQueueHandle;
extern osMessageQueueId_t usbRxQueueHandle;

#endif