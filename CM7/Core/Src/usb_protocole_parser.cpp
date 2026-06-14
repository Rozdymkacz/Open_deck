#include "usb_protocole/usb_protocole_parser.hpp"

#include "usb_protocole/usb_message_descriptors.hpp"

#include <stdint.h>
#include <stdio.h>
#include "cmsis_os2.h"
#include "queue_typs.h"
#include "app_queues.h"
#include "crc.h"

constexpr size_t HeaderSize = sizeof(Header);
constexpr uint16_t ProtocolVersionMajor = 0;
constexpr uint16_t ProtocolVersionMinor = 1;

bool UsbProtocol::parse( const uint8_t* data, uint16_t length)
{
    if (length < HeaderSize)
    {
        return false;
    }

    Header header;

    if (!parseHeader(data, header))
    {
        return false;
    }

    if (!verifyLength(header, length))
    {
        return false;
    }

    if (!verifyCRC(data, length))
    {
        return false;
    }

    const uint8_t* payload = data + HeaderSize;
    return dispatch(header, payload);
}

bool UsbProtocol::parseHeader(const uint8_t* data, Header& header)
{
    memcpy(&header, data, sizeof(Header));

    if (header.protocolVersionMajor != ProtocolVersionMajor ||
        header.protocolVersionMinor != ProtocolVersionMinor)
    {
        return false;
    }

    return true;
}

bool UsbProtocol::verifyLength(const Header& header, uint16_t length)
{
    uint32_t expectedLength = HeaderSize + header.payloadSize + sizeof(header.crc);
    return length == expectedLength;
}

bool UsbProtocol::verifyCRC(const uint8_t* data, uint16_t length)
{
    uint16_t receivedCrc;
    memcpy(&receivedCrc, data + length - sizeof(uint16_t), sizeof(uint16_t));
    uint16_t calculatedCrc = calculateCRC(data, length - sizeof(uint16_t));
    return receivedCrc == calculatedCrc;
}

uint32_t UsbProtocol::calculateCRC(const uint8_t* data, uint16_t length)
{
    uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)data, length / sizeof(uint32_t));
    return crc;
}

bool UsbProtocol::dispatch(const Header& header, const uint8_t* payload)
{
    switch (static_cast<CommandId>(header.commandId))
    {
        case CommandId::Ping:
            // Handle Ping command
            break;
        case CommandId::FirmwareVersion:
            // Handle FirmwareVersion command
            break;
        case CommandId::ButtonEvent:
            // Handle ButtonEvent command
            break;
        default:
            return false; // Unknown command
    }
    return true;
}