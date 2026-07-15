/**
 * @file myapp.hpp
 * @brief declaration of command IDs for USB communication
 *
 * @details This file contains the declaration of command IDs used for USB communication. it's must be identical in both stm32 and PC application.
 *
 * @author Rozdymkacz
 *
 * @date 2026-07-15
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void my_usb_reception_data(void);
void my_usb_send_data(void);

void my_usb_procesing_data(void);
void task_2500ms(void);

#ifdef __cplusplus
}
#endif

