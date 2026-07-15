/**
 * @file usb_message_descriptors.hpp
 * @brief Definitions of data structures for USB communication
 *
 * @details
 * This file contains the definitions of the data structures used for USB communication. 
 * It defines the Header and Packet structures, which are used to encapsulate the data being sent and received over USB.
 *
 * @author Rozdymkacz
 * @date 2026-07-15
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <stdint.h>
#include <stdio.h>

namespace od
{

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

}//namespace od
