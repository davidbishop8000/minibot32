/*
 * uart_wifi.h
 *
 *  Created on: May 10, 2023
 *      Author: USER
 */

#ifndef CPP_INC_UART_WIFI_H_
#define CPP_INC_UART_WIFI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "cmsis_os2.h"
#include "usart.h"
#include <termoplast_config.h>

#define RECEV_MSG_CS_BYTE_NUM 7


void StartUartWiFiTask(void *argument);

void SendStatus();
void SetManual();
void SetJob();
void ConfigInit();
int ConfigUpdate();
void GetSTMConfig();
uint8_t calculateCS(uint8_t *msg, int msg_size);

#ifdef __cplusplus
}
#endif

#endif /* CPP_INC_UART_WIFI_H_ */
