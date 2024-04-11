/*
 * uart_wifi.cpp
 *
 *  Created on: May 10, 2023
 *      Author: USER
 */

#include "uart_wifi.h"
#include <string.h>
#include "minibot_config.h"
#include "flash_data.h"
#include "KeyaLKTechDriver.h"

#include <stdio.h> //for test

extern UART_HandleTypeDef RC_UART;
extern DMA_HandleTypeDef RC_UART_DMA;

extern UART_HandleTypeDef BMS_UART;
extern DMA_HandleTypeDef BMS_UART_DMA;

extern UART_HandleTypeDef WIFI_UART;
extern DMA_HandleTypeDef WIFI_UART_DMA;

extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;
extern KeyaLKTechDriver *mdrivers[DRIVERS_QUANT];

extern ContrlMsgTypeDef contrlMsg;
extern StatusMsgTypeDef statusMsg;

uint8_t new_wifi_data = 0;
uint8_t new_remote_data = 0;
uint8_t new_bms_data = 0;

uint8_t rc_uart_buff[100];
uint8_t bms_uart_buff[100];
uint8_t wifi_uart_buff[100];

void StartUartWiFiTask(void *argument)
{
	for(;;)
	{
		if (new_wifi_data && wifi_uart_buff[0] == START_MSG0 && wifi_uart_buff[1] == START_MSG1)
		{
			enum MSG_ID message_id = (MSG_ID)wifi_uart_buff[2];
			if (message_id == MSG_STATUS)
			{
				if (wifi_uart_buff[sizeof(StatusMsgTypeDef)-1] != calculateCS(wifi_uart_buff, sizeof(StatusMsgTypeDef)-1))
				{
					globData.cs_err++;
					HAL_UART_Transmit(&WIFI_UART, (uint8_t*)"error", 5, 100);
				}
				else
				{
					//memcpy(&contrlMsg, wifi_uart_buff, sizeof(ContrlMsgTypeDef));
					sendStatus();
				}
			}
			else if (message_id == MSG_CONTROL)
			{
				if (wifi_uart_buff[sizeof(ContrlMsgTypeDef)-1] != calculateCS(wifi_uart_buff, sizeof(ContrlMsgTypeDef)-1))
				{
					globData.cs_err++;
				}
				else
				{
					memcpy(&contrlMsg, wifi_uart_buff, sizeof(ContrlMsgTypeDef));
					sendStatus();
				}
			}
			new_wifi_data = 0;
			memset(wifi_uart_buff, 0, 100);
		}

		//osDelay(1000);
		//SendStatus(); //for test
		//globData.LKEncoder++;
		//uint8_t str[30];
		//sprintf((char*)str, (char*)"e: %d, t: %d\n\r", globData.LKEncoder, globData.LKTemp);
		//HAL_UART_Transmit(&huart1, str, strlen((char *)str), 100);
		//HAL_UART_Transmit(&WIFI_UART, (uint8_t*)"WIFI ok", 7, 100);
		osDelay(1);
	}
}

void sendStatus()
{
	statusMsg.start_msg0 = START_MSG0;
	statusMsg.start_msg1 = START_MSG1;
	statusMsg.msg_id = MSG_STATUS;
	statusMsg.pos_fb = mdrivers[0]->getPos();
	statusMsg.pos_lr = mdrivers[1]->getPos();;
	statusMsg.pos_fork += 2;
	statusMsg.pos_servo += 3;

	statusMsg.CS = calculateCS((uint8_t *)&statusMsg, sizeof(statusMsg)-1);
	HAL_UART_Transmit(&WIFI_UART, (uint8_t*)&statusMsg, sizeof(statusMsg), 100);
}

void SetManual()
{

}

uint8_t calculateCS(uint8_t *msg, int msg_size) {
  uint8_t cs = 0;
  for (int i=0; i<msg_size; i++)
  {
    cs+=msg[i];
  }
  return cs;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	uint32_t er = HAL_UART_GetError(huart);
	switch (er) {
		case HAL_UART_ERROR_PE: // ошибка четности
			__HAL_UART_CLEAR_PEFLAG(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_NE:  // шум на линии
			__HAL_UART_CLEAR_NEFLAG(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_FE:  // ошибка фрейма
			__HAL_UART_CLEAR_FEFLAG(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_ORE:  // overrun error
			__HAL_UART_CLEAR_OREFLAG(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_DMA:  // ошибка DMA
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		default:
			break;
	}
	if (huart->Instance == WIFI_UART_Ins) {
		new_wifi_data = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&WIFI_UART, wifi_uart_buff,
				sizeof(wifi_uart_buff));
		__HAL_DMA_DISABLE_IT(&WIFI_UART_DMA, DMA_IT_HT);
	}
	else if (huart->Instance == BMS_UART_Ins) {
		new_bms_data = 0;
		HAL_UARTEx_ReceiveToIdle_DMA(&BMS_UART, bms_uart_buff,
				sizeof(bms_uart_buff));
		__HAL_DMA_DISABLE_IT(&BMS_UART_DMA, DMA_IT_HT);
	}
	else if (huart->Instance == RC_UART_Ins) {
		new_remote_data = 0;
		HAL_UARTEx_ReceiveToIdle_DMA(&RC_UART, rc_uart_buff,
				sizeof(rc_uart_buff));
		__HAL_DMA_DISABLE_IT(&RC_UART_DMA, DMA_IT_HT);
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == WIFI_UART_Ins) {
		new_wifi_data = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&WIFI_UART, wifi_uart_buff, sizeof(wifi_uart_buff));
		__HAL_DMA_DISABLE_IT(&WIFI_UART_DMA, DMA_IT_HT);
		//HAL_UART_Transmit(&WIFI_UART, (uint8_t*)"DEBA ok\r\n", 9, 100);
		//HAL_UART_Transmit(&WIFI_UART, wifi_uart_buff, sizeof(wifi_uart_buff), 100);
	}
	else if (huart->Instance == BMS_UART_Ins) {
		new_bms_data = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&BMS_UART, bms_uart_buff, sizeof(bms_uart_buff));
		__HAL_DMA_DISABLE_IT(&BMS_UART_DMA, DMA_IT_HT);
		//HAL_UART_Transmit(&WIFI_UART, (uint8_t*)"BMS ok", 6, 100);
	}
	else if (huart->Instance == RC_UART_Ins) {
		new_remote_data = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&RC_UART, rc_uart_buff, sizeof(rc_uart_buff));
		__HAL_DMA_DISABLE_IT(&RC_UART_DMA, DMA_IT_HT);
		//HAL_UART_Transmit(&WIFI_UART, (uint8_t*)"RC ok", 5, 100);
	}
}
