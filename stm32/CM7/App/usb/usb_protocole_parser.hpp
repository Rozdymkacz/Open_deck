/**
 * @file usb_protocole_parser.hpp
 * @brief Parser for USB protocol data packets
 *
 * @details This file contains the declaration of the USB protocol parser class, which is responsible for parsing incoming USB data packets and dispatching them to the appropriate handlers.
 *
 * @author Rozdymkacz
 * @date 2026-07-15
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "usb/usb_message_descriptors.hpp"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace od
{

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

}//namespace od

#ifdef __cplusplus
}
#endif

