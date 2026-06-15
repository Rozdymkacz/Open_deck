#include "usb_protocole/usb_protocole_parser.hpp"

#include "usb_protocole/usb_message_descriptors.hpp"
#include "usb_protocole/usb_comandId.hpp"

#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include "cmsis_os2.h"
#include "queue_typs.h"
#include "app_queues.h"
#include "crc.h"
#include "main.h"

constexpr uint16_t ProtocolVersionMajor = 0;
constexpr uint16_t ProtocolVersionMinor = 1;
constexpr uint16_t HeaderLength = sizeof(Packet::header);
constexpr uint16_t CrcLength = sizeof(Packet::crc);

bool UsbProtocol::parse( const uint8_t* data, uint16_t length)
{

    if (length < sizeof(Packet::header) + sizeof(Packet::crc)) // Minimum size: header + CRC
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

    Packet packet;

    if (!parsePacket(data, length, packet))
    {
        return false;
    }

    if (!verifyCRC(data, length))
    {
        return false;
    }

    return dispatch(&packet);
}

bool UsbProtocol::parsePacket(const uint8_t* data, uint16_t length, Packet& packet)
{
    memcpy(
        &packet.header,
        data,
        sizeof(Packet::header));

    memcpy(
        packet.payload,
        data + sizeof(Packet::header),
        packet.header.payloadSize);

    memcpy(
        &packet.crc,
        data + sizeof(Packet::header) + packet.header.payloadSize,
        sizeof(packet.crc));

    return true;
}

bool UsbProtocol::parseHeader(const uint8_t* data, Header& header)
{
    memcpy(&header, data, sizeof(Packet::header));

    if (header.protocolVersionMajor != ProtocolVersionMajor ||
        header.protocolVersionMinor != ProtocolVersionMinor)
    {
        return false;
    }

    return true;
}

bool UsbProtocol::verifyLength(const Header& header, uint16_t length)
{
    uint32_t expectedLength = sizeof(Packet::header) + header.payloadSize + sizeof(Packet::crc);
    return length == expectedLength;
}

bool UsbProtocol::verifyCRC(const uint8_t* data, uint16_t length)
{
    /* for debug
    uint32_t receivedCrc;
    memcpy(&receivedCrc, data + length - sizeof(uint32_t), sizeof(uint32_t));
    uint32_t calculatedCrc = calculateCRC(data, length - sizeof(uint32_t));
    return receivedCrc == calculatedCrc;
    */
    return true;
}

uint32_t UsbProtocol::calculateCRC(const uint8_t* data, uint16_t length)
{

    uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)data, length / sizeof(uint32_t));
    return crc;
}

bool UsbProtocol::dispatch(const Packet* packet)
{
    UsbMessage_t tx_msg;
    switch (static_cast<CommandId>(packet->header.commandId))
    {
        case CommandId::Ping:
        memset(&tx_msg.data, 0, sizeof(tx_msg.data));
            snprintf(
                tx_msg.data,
                sizeof(tx_msg.data),
                "Received Ping Message\r\n");

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

    tx_msg.len = strlen(tx_msg.data);

    osMessageQueuePut(
    usbTxQueueHandle,
    &tx_msg,
    0,
    0);

    return true;
}