/**
 * @file usb_commandId.hpp
 * @brief declaration of command IDs for USB communication
 *
 * @details This file contains the declaration of command IDs used for USB communication. it's must be identical in both stm32 and PC application.
 *
 * @author Rozdymkacz
 * @date 2026-07-15  
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <stdint.h>

namespace od
{

enum class CommandId : uint16_t
{
    Error            = 1,
    Ping             = 2,
    FirmwareVersion  = 3,
    ButtonEvent      = 4
};

} // namespace od
