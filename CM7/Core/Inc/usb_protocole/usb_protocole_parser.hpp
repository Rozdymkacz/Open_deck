#pragma once

#include "usb_protocole/usb_message_descriptors.hpp"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

class UsbProtocolParser
{
public:

    static bool parse(
        const uint8_t* data,
        uint16_t length);

private:

    static bool parseHeader(
        const uint8_t* data,
        Header* header);
    
    static bool verifyLength(
        const Header* header,
        uint16_t length);

    static bool parsePacket(
        const uint8_t* data,
        uint16_t length,
        Packet* packet);

    static bool verifyCRC(
        const uint8_t* data,
        uint16_t length);

    static uint32_t calculateCRC(
        const uint8_t* data,
        uint16_t length);

    static bool dispatch(const Packet* packet);
};
#ifdef __cplusplus
}
#endif

