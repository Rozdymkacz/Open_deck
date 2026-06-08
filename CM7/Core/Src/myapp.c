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

volatile uint32_t usb_send_calls = 0;
volatile uint32_t usb_queue_dos = 0;

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

  osMessageQueuePut( usbRxQueueHandle,
    &msg,
    0,
    0);
}

void my_usb_send_data(void)
{

  usb_send_calls++;
  UsbMessage_t tx_msg;

  while (osMessageQueueGetCount(usbTxQueueHandle) > 0)
  {
    osStatus_t status = osMessageQueueGet(usbTxQueueHandle, &tx_msg, NULL, 0);
    if(!status == osOK) return;

    usb_queue_dos++;
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

/*
void my_cdc_task(void)
{
    
    task_1000ms();
    task_2500ms();

    if (!tud_mounted())
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        return;
    }

    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

    if (!tud_cdc_available())
    {
        return;
    }

    char usb_rx_buf[64];
    char tx_buffer[128];

    uint32_t count = tud_cdc_read(usb_rx_buf, sizeof(usb_rx_buf) - 1);

    if (count == 0)
    {
        return;
    }

    usb_rx_buf[count] = '\0';

    int len = snprintf(
        tx_buffer,
        sizeof(tx_buffer),
        "Hello from stm32, you sent: %s",
        usb_rx_buf
    );

    tud_cdc_write(tx_buffer, len);
    tud_cdc_write_flush();
}


void task_2500ms(void)
{
  static uint32_t last_time = 0;

  if ((HAL_GetTick() - last_time) >= 2500)
  {
    last_time = HAL_GetTick();
    
    if (tud_mounted())
    {
      if (tud_cdc_write_available() >= 17)
      {
        tud_cdc_write("Hello from STM32\r\n", 17);
        tud_cdc_write_flush();
      }
    }

  }
} //task_2500ms


void task_1000ms(void)
{
  static uint32_t last_time = 0;

  if ((HAL_GetTick() - last_time) >= 1000)
  {
    last_time = HAL_GetTick();
    printf( "mounted=%u uavilable=%u \r\n", tud_mounted(), tud_cdc_available());
  }

}// task_1000ms
*/