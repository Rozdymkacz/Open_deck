#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CFG_TUSB_MCU              OPT_MCU_STM32H7
#define CFG_TUSB_OS               OPT_OS_FREERTOS

// USB HS + external ULPI PHY (DWC2)
#define CFG_TUSB_RHPORT1_MODE     (OPT_MODE_DEVICE | OPT_MODE_HIGH_SPEED)

// TinyUSB DWC2 driver
#define CFG_TUD_DWC2_DMA_ENABLE 1

// Endpoint 0
#define CFG_TUD_ENDPOINT0_SIZE    64

// CDC only
#define CFG_TUD_CDC               1
#define CFG_TUD_ENABLED           1
#define CFG_TUD_CDC_NOTIFY        1

#define CFG_TUD_CDC_RX_BUFSIZE 512
#define CFG_TUD_CDC_TX_BUFSIZE 512

#define CFG_TUD_CDC_EP_BUFSIZE 512
#define CFG_TUSB_MEM_SECTION __attribute__((section(".SRAM1")))
#define CFG_TUSB_MEM_ALIGN __attribute__((aligned(32)))

// Disable unused classes
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0

// Debug
#define CFG_TUSB_DEBUG            3

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */