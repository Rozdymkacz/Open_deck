/**
 * @file myapp.cpp
 * @brief receiving and sending data from usb, and processing it
 *
 * @details
 * This file contains the implementation of the first stage data processing for the application. 
 * It includes functions for USB data reception, sending, and processing, as well as a periodic task that runs every 2500 milliseconds.
 *
 * @author Rozdymkacz
 * @date 2026-07-15
 *
 * @copyright Copyright (c) 2026
 *
 * @todo
 * - split into several files, one for each thread
 * - improve data passing to the parser 
 */

#include "myapp.hpp"

#include "class/cdc/cdc_device.h"
#include "main.h"
#include "tusb.h"
//#include <cstring>
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue_typs.h"
#include "fr_queues_externs.h"
#include "usb/usb_protocole_parser.hpp"

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

  bool result = od::UsbProtocolParser::parse(reinterpret_cast<uint8_t*>(msg.data), msg.len);

  if (!result)
  {
    printf("Unable to parse data\r\n");
    //return;
  }else{
    printf("Parsed data\r\n");
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
