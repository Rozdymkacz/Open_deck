/**
 * @file queue_types.h
 * @brief declaration of queue types for USB communication
 *
 * @details This file contains the declaration of queue types used for USB communication.
 *
 * @author Rozdymkacz
 * @date 2026-07-15  
 *
 * @copyright Copyright (c) 2026
 */

#ifndef QUEUE_TYPES_H
#define QUEUE_TYPES_H

#include <stdint.h>

typedef struct
{
    uint16_t len;
    char data[126];
} UsbMessage_t;

#endif