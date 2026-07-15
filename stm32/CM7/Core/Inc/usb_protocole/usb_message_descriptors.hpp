#pragma once

#include <stdint.h>
#include <stdio.h>

struct __attribute__((packed)) Header
{
    uint16_t protocolVersionMajor;
    uint16_t protocolVersionMinor;

    uint16_t commandId;

    uint16_t payloadSize;
};

struct __attribute__((packed)) Packet
{
    Header header;

    uint8_t payload[500];

    uint32_t crc;
};