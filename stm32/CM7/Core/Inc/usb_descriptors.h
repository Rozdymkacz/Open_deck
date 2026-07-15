#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tusb.h"

/**
 * @brief Callback zwracający Device Descriptor.
 *        Wywoływany automatycznie przez TinyUSB podczas enumeracji.
 */
uint8_t const * tud_descriptor_device_cb(void);

/**
 * @brief Callback zwracający Configuration Descriptor (zawiera interfejsy CDC).
 * @param index Indeks konfiguracji (zazwyczaj 0).
 */
uint8_t const * tud_descriptor_configuration_cb(uint8_t index);

/**
 * @brief Callback zwracający String Descriptors (Manufacturer, Product, Serial, itp.).
 * @param index  Indeks ciągu (0 = Language ID, 1+ = teksty).
 * @param langid ID języka żądane przez hosta.
 */
uint16_t const * tud_descriptor_string_cb(uint8_t index, uint16_t langid);

#ifdef __cplusplus
}
#endif

#endif /* USB_DESCRIPTORS_H */