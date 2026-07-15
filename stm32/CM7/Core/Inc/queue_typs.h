#ifndef QUEUE_TEPES_H
#define QUEUE_TEPES_H

#include <stdint.h>

typedef struct
{
    uint16_t len;
    char data[126];
} UsbMessage_t;
#endif