#include "usb_descriptors.h"
#include "tusb.h"

#include <string.h>

//=============================================================================
// Device Descriptor
//=============================================================================

uint8_t const* tud_descriptor_device_cb(void)
{
    static tusb_desc_device_t const desc_device =
    {
        .bLength            = sizeof(tusb_desc_device_t),
        .bDescriptorType    = TUSB_DESC_DEVICE,
        .bcdUSB             = 0x0200,

        .bDeviceClass       = TUSB_CLASS_MISC,
        .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
        .bDeviceProtocol    = MISC_PROTOCOL_IAD,

        .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

        .idVendor           = 0xCAFE,
        .idProduct          = 0x4000,
        .bcdDevice          = 0x0100,

        .iManufacturer      = 0x01,
        .iProduct           = 0x02,
        .iSerialNumber      = 0x03,

        .bNumConfigurations = 0x01
    };

    return (uint8_t const*) &desc_device;
}

//=============================================================================
// Device Qualifier Descriptor (required for HS)
//=============================================================================

uint8_t const* tud_descriptor_device_qualifier_cb(void)
{
    static tusb_desc_device_qualifier_t const desc_qualifier =
    {
        .bLength            = sizeof(tusb_desc_device_qualifier_t),
        .bDescriptorType    = TUSB_DESC_DEVICE_QUALIFIER,
        .bcdUSB             = 0x0200,

        .bDeviceClass       = TUSB_CLASS_MISC,
        .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
        .bDeviceProtocol    = MISC_PROTOCOL_IAD,

        .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
        .bNumConfigurations = 1,
        .bReserved          = 0
    };

    return (uint8_t const*) &desc_qualifier;
}

//=============================================================================
// Configuration Descriptor
//=============================================================================

enum
{
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN \
    (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)

uint8_t const* tud_descriptor_configuration_cb(uint8_t index)
{
    (void) index;

    static uint8_t const desc_configuration[] =
    {
        TUD_CONFIG_DESCRIPTOR(
            1,                  // configuration number
            ITF_NUM_TOTAL,      // interface count
            0,                  // string index
            CONFIG_TOTAL_LEN,
            0,                  // attributes
            100                 // max power (mA)
        ),

        TUD_CDC_DESCRIPTOR(
            ITF_NUM_CDC,
            4,                  // string index
            0x81,               // notification endpoint
            8,                  // notification endpoint size
            0x02,               // OUT endpoint
            0x82,               // IN endpoint
            512                 // HS max packet size
        )
    };

    return desc_configuration;
}

//=============================================================================
// Other Speed Configuration Descriptor (required for HS)
//=============================================================================

uint8_t const* tud_descriptor_other_speed_configuration_cb(uint8_t index)
{
    return tud_descriptor_configuration_cb(index);
}

//=============================================================================
// String Descriptors
//=============================================================================

static const char* string_desc_arr[] =
{
    (const char[]) { 0x09, 0x04 }, // 0: English language ID
    "MyCompany",                   // 1: Manufacturer
    "STM32H747 CDC",               // 2: Product
    "123456",                      // 3: Serial
    "CDC Interface"                // 4: Interface
};

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    static uint16_t desc_str[32];
    uint8_t chr_count;
    uint8_t i;

    (void) langid;

    if(index == 0)
    {
        memcpy(&desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    }
    else
    {
        if(index >= (sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
        {
            return NULL;
        }

        chr_count = (uint8_t) strlen(string_desc_arr[index]);

        if(chr_count > 31)
        {
            chr_count = 31;
        }

        for(i = 0; i < chr_count; i++)
        {
            desc_str[1 + i] =
                (uint16_t) string_desc_arr[index][i];
        }
    }

    desc_str[0] =
        (uint16_t)
        (
            (TUSB_DESC_STRING << 8) |
            ((2 * chr_count) + 2)
        );

    return desc_str;
}