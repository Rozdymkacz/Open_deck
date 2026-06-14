#include "myapp.h"

#include "class/cdc/cdc_device.h"
#include "main.h"
#include "tusb.h"
//#include <cstring>
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue_typs.h"
#include "app_queues.h"
#include "usb_protocole/usb_protocole_parser.hpp"

void my_usb_reception_data(void)
{

  if (!tud_mounted())
  {    
    return;
  }

  if (!tud_cdc_available())
  {
    return;
  }

  UsbMessage_t msg;

  msg.len = tud_cdc_read(
    msg.data,
    sizeof(msg.data) - 1);

  if (msg.len == 0)
  {
    return;
  }

  msg.data[msg.len] = '\0';

  bool result = UsbProtocol::parse(msg.data, msg.len);
  if (!result)
  {
    printf("Unable to parse data\r\n");
    return;
  }

  osMessageQueuePut( usbRxQueueHandle,
    &msg,
    0,
    0);
}

void my_usb_send_data(void)
{

  UsbMessage_t tx_msg;

  while (osMessageQueueGetCount(usbTxQueueHandle) > 0)
  {
    osStatus_t status = osMessageQueueGet(usbTxQueueHandle, &tx_msg, NULL, 0);
    if(!status == osOK) return;

    uint32_t space = tud_cdc_write_available();

    if (space < tx_msg.len) {
      // nie ma miejsca -> wracamy później
      osMessageQueuePut(usbTxQueueHandle, &tx_msg, 0, 0);
      break;
    }

    tud_cdc_write(
      tx_msg.data,
      tx_msg.len);
  }
  tud_cdc_write_flush();
}

void my_usb_procesing_data(void)
{

  UsbMessage_t tx_msg;
  UsbMessage_t rx_msg;

  if (!(osMessageQueueGet(usbRxQueueHandle, &rx_msg, NULL, 0) == osOK)){
    return;
  }

  snprintf(
    tx_msg.data,
    sizeof(tx_msg.data),
    "witaj z innego taska napisales: %s \r\n",
    rx_msg.data);

  tx_msg.len = strlen(tx_msg.data);

  osMessageQueuePut(
    usbTxQueueHandle,
    &tx_msg,
    0,
    0);

}

void task_2500ms(void)
{

  UsbMessage_t tx_msg;

  strcpy(tx_msg.data, "Hielo from stm32\r\n");
  tx_msg.len = strlen(tx_msg.data);

  osMessageQueuePut(usbTxQueueHandle, &tx_msg, 0, 0);

}
