/**
 * @file usb_protocole_parser.cpp
 * @brief Parser for USB protocol data packets
 *
 * @details This file contains the implementation of the USB protocol parser class, which is responsible for parsing incoming USB data packets and dispatching them to the appropriate handlers.
 *
 * @author Rozdymkacz
 * @date 2026-07-15
 *
 * @copyright Copyright (c) 2026
 */
#include "usb/usb_protocole_parser.hpp"

#include "usb/usb_message_descriptors.hpp"
#include "usb/usb_commandId.hpp"

#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include "cmsis_os2.h"
#include "queue_typs.h"
#include "fr_queues_externs.h"
#include "crc.h"
#include "main.h"

namespace od
{

constexpr uint16_t UsbProtocolVersionMajor = 0;
constexpr uint16_t UsbProtocolVersionMinor = 1;

constexpr uint8_t HeaderLength = sizeof(Packet::header);
constexpr uint16_t PayloadLength = sizeof(Packet::payload);
constexpr uint8_t CrcLength = sizeof(Packet::crc);

bool UsbProtocolParser::parse( const uint8_t* data, uint16_t length)
{

    if (length < HeaderLength + CrcLength) // Minimum size: header + CRC
    {
        return false;
    }

    static Header header;
    memset(&header, 0, HeaderLength);

    if (!parseHeader(data, &header))
    {
        return false;
    }

    if (!verifyLength(&header, length))
    {
        return false;
    }

    static Packet packet;
    memset(&packet, 0, sizeof(Packet));

    if (!parsePacket(data, length, &packet))
    {
        return false;
    }

    if (!verifyCRC(data, length))
    {
        return false;
    }

    return dispatch(&packet);
}

bool UsbProtocolParser::parsePacket(const uint8_t* data, uint16_t length, Packet* packet)
{
    memcpy(
        &packet->header,
        data,
        HeaderLength);

    memcpy(
        packet->payload,
        data + HeaderLength,
        packet->header.payloadSize);

    memcpy(
        &packet->crc,
        data + HeaderLength + packet->header.payloadSize,
        CrcLength);

    return true;
}

bool UsbProtocolParser::parseHeader(const uint8_t* data, Header* header)
{
    memcpy(header, data, HeaderLength);

    if (header->protocolVersionMajor != UsbProtocolVersionMajor ||
        header->protocolVersionMinor != UsbProtocolVersionMinor)
    {
        return false;
    }

    return true;
}

bool UsbProtocolParser::verifyLength(const Header* header, uint16_t length)
{
    if (length > sizeof(Packet))
    {
        return false;
    }

    uint32_t expectedLength = HeaderLength + header->payloadSize + CrcLength;
    return length == expectedLength;
}

bool UsbProtocolParser::verifyCRC(const uint8_t* data, uint16_t length)
{
    /* for debug
    uint32_t receivedCrc;
    memcpy(&receivedCrc, data + length - sizeof(uint32_t), sizeof(uint32_t));
    uint32_t calculatedCrc = calculateCRC(data, length - sizeof(uint32_t));
    return receivedCrc == calculatedCrc;
    */
    return true;
}

uint32_t UsbProtocolParser::calculateCRC(const uint8_t* data, uint16_t length)
{

    uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)data, length / sizeof(uint32_t));
    return crc;
}

bool UsbProtocolParser::dispatch(const Packet* packet)
{
    UsbMessage_t tx_msg;
    switch (static_cast<CommandId>(packet->header.commandId))
    {
        case CommandId::Ping:
        printf("Received Ping command: %.*s\r\n", packet->header.payloadSize, packet->payload);

        break;

        case CommandId::FirmwareVersion:
            printf("Received FirmwareVersion command: %.*s\r\n", packet->header.payloadSize, packet->payload);
            break;
        case CommandId::ButtonEvent:
            printf("Received ButtonEvent command: %.*s\r\n", packet->header.payloadSize, packet->payload);
            break;
        default:
            printf("Received unknown command: %d\r\n", packet->header.commandId);
            return false; // Unknown command
    }

    return true;
}

}//namespace od