#pragma once

#include "usb_protocole/usb_comandId.hpp"

#ifdef __cplusplus
extern "C" {
#endif

class UsbProtocol
{
public:

    static bool parse(
        const uint8_t* data,
        uint16_t length);

private:

    static bool parseHeader(
        const uint8_t* data,
        Header& header);

    static bool verifyLength(
        const Header& header,
        uint16_t length);

    static bool verifyCRC(
        const uint8_t* data,
        uint16_t length);

    static uint32_t calculateCRC(
        const uint8_t* data,
        uint16_t length);

    static bool dispatch(
        const Header& header,
        const uint8_t* payload);
};
#ifdef __cplusplus
}
#endif

