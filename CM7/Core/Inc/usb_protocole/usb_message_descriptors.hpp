#pragma once

struct __attribute__((packed)) Header
{
    uint16_t protocolVersionMajor;
    uint16_t protocolVersionMinor;

    uint16_t commandId;

    uint16_t payloadSize;
};

template<size_t PayloadSize>
struct __attribute__((packed)) Packet
{
    Header header;

    uint8_t payload[PayloadSize];

    uint32_t crc;
};