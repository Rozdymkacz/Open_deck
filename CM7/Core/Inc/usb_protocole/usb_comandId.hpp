#pragma once

#include <stdint.h>

enum class CommandId : uint16_t
{
    Error            = 1,
    Ping             = 2,
    FirmwareVersion  = 3,
    ButtonEvent      = 4
};

