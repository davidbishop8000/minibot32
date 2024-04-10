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

#include <stdio.h> //for test

extern UART_HandleTypeDef RC_UART;
extern DMA_HandleTypeDef RC_UART_DMA;

extern UART_HandleTypeDef BMS_UART;
extern DMA_HandleTypeDef BMS_UART_DMA;

extern UART_HandleTypeDef WIFI_UART;
extern DMA_HandleTypeDef WIFI_UART_DMA;

extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;

extern ContrlMsgTypeDef contrlMsg;

uint8_t new_wifi_data = 0;
uint8_t new_remote_data = 0;
uint8_t new_bms_data = 0;

uint8_t rc_uart_buff[100];
uint8_t bms_uart_buff[100];
uint8_t wifi_uart_buff[100];
StatusMsgTypeDef statusMsg;
JobMsgTypeDef jobMsg;
StmConfigTypeDef stmConf;

void StartUartWiFiTask(void *argument)
{
	//HAL_UARTEx_ReceiveToIdle_DMA(&WIFI_UART, wifi_uart_buff, sizeof(wifi_uart_buff));
	//__HAL_DMA_DISABLE_IT(&WIFI_UART_DMA, DMA_IT_HT);
	//ConfigInit();
	for(;;)
	{
		if (new_wifi_data && wifi_uart_buff[0] == START_MSG0 && wifi_uart_buff[1] == START_MSG1)
		{
			enum MSG_ID message_id = (MSG_ID)wifi_uart_buff[2];
			if (message_id == MSG_CONTROL)
			{
				if (wifi_uart_buff[sizeof(ContrlMsgTypeDef)-1] != calculateCS(wifi_uart_buff, sizeof(ContrlMsgTypeDef)-1))
				{
					globData.cs_err++;
				}
				else
				{
					memcpy(&contrlMsg, wifi_uart_buff, sizeof(ContrlMsgTypeDef));
					//SetJob();
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

void SendStatus()
{
	statusMsg.start_msg0 = START_MSG0;
	statusMsg.start_msg1 = START_MSG1;
	statusMsg.control_id = NET_CONTROL_ID;
	//statusMsg.msg_id = WIFI_GET_STATUS;
	statusMsg.cs_errors = globData.cs_err;
	statusMsg.temp1 = globData.temp1;
	statusMsg.temp2 = globData.temp2;
	statusMsg.temp3 = globData.temp3;
	statusMsg.sens = globData.sens;
	statusMsg.cycles_count = globData.cycles_count;
	statusMsg.cycles_set = globData.cycles_set;
	statusMsg.CS = calculateCS((uint8_t *)&statusMsg, sizeof(statusMsg)-1);

	//for test//////
	/*
	float Th1_temp =0;
	int intpart, fracpart;
	char txt1[11];

	Th1_temp = statusMsg.temp1;
	intpart = (int)Th1_temp;
	fracpart = (int)((Th1_temp - intpart) * 100);
	snprintf(txt1, 11, "%3d.%02d", intpart, fracpart);
	HAL_UART_Transmit(&WIFI_UART, (uint8_t*)&txt1, sizeof(txt1), 100);
	*/
	//////////////////////////


	HAL_UART_Transmit(&WIFI_UART, (uint8_t*)&statusMsg, sizeof(statusMsg), 100);
}

void SetManual()
{

}

void SetJob()
{
	memcpy(&contrlMsg, wifi_uart_buff, sizeof(ContrlMsgTypeDef));
}

void GetSTMConfig()
{
	stmConf.start_msg0 = START_MSG0;
	stmConf.start_msg1 = START_MSG1;
	stmConf.control_id = NET_CONTROL_ID;
	//stmConf.msg_id = WIFI_GET_STM_CONFIG;
	stmConf.termConfig = minibotConfig;
	stmConf.CS = calculateCS((uint8_t *)&stmConf, sizeof(StmConfigTypeDef)-1);
	HAL_UART_Transmit(&WIFI_UART, (uint8_t*)&stmConf, sizeof(StmConfigTypeDef), 100);
}

uint8_t calculateCS(uint8_t *msg, int msg_size) {
  uint8_t cs = 0;
  for (int i=0; i<msg_size; i++)
  {
    cs+=msg[i];
  }
  return cs;
}

void ConfigInit()
{
	flashReadData(&minibotConfig);
	if (minibotConfig.flash_init != FLASH_INIT)
	{
		minibotConfig.volume_per_rev = 100.0;
		minibotConfig.volume = 10.0;
		minibotConfig.motor1_speed = 1000;
		minibotConfig.motor1_acc = 500;
		minibotConfig.motor2_speed = 1000;
		minibotConfig.motor2_acc = 500;
		minibotConfig.time_hold = 10;
		minibotConfig.temp1 = 30.0;
		minibotConfig.temp2 = 30.0;
		minibotConfig.temp3 = 30.0;
		minibotConfig.Kp = 1.0;
		minibotConfig.Ki = 1.0;
		minibotConfig.Kd = 1.0;
		minibotConfig.bitParams.ind = 1;
	}
}

int ConfigUpdate()
{
	int err = 0;
	if (stmConf.termConfig.volume_per_rev > 0) {
		minibotConfig.volume_per_rev = stmConf.termConfig.volume_per_rev;
	}
	else err++;
	if (stmConf.termConfig.volume > 0) {
		minibotConfig.volume = stmConf.termConfig.volume;
	}
	else err++;
	if (stmConf.termConfig.motor1_speed > 0 && stmConf.termConfig.motor1_speed < 5000)
	{
		minibotConfig.motor1_speed = stmConf.termConfig.motor1_speed;
	}
	else err++;
	if (stmConf.termConfig.motor1_acc > 0 && stmConf.termConfig.motor1_acc < 5000)
	{
		minibotConfig.motor1_acc= stmConf.termConfig.motor1_acc;
	}
	else err++;
	if (stmConf.termConfig.motor2_speed > 0 && stmConf.termConfig.motor2_speed < 5000) {
		minibotConfig.motor2_speed = stmConf.termConfig.motor2_speed;
	}
	else err++;
	if (stmConf.termConfig.motor2_acc > 0 && stmConf.termConfig.motor2_acc < 5000) {
		minibotConfig.motor2_acc = stmConf.termConfig.motor2_acc;
	}
	else err++;
	if (stmConf.termConfig.motor2_acc > 0 && stmConf.termConfig.motor2_acc < 5000) {
		minibotConfig.motor2_acc = stmConf.termConfig.motor2_acc;
	}
	else err++;
	if (stmConf.termConfig.time_hold > 0) {
		minibotConfig.time_hold = stmConf.termConfig.time_hold;
	}
	else err++;
	if (stmConf.termConfig.temp1 > 0 && stmConf.termConfig.temp1 < 500) {
		minibotConfig.temp1 = stmConf.termConfig.temp1;
	}
	else err++;
	if (stmConf.termConfig.temp2 > 0 && stmConf.termConfig.temp2 < 500) {
		minibotConfig.temp2 = stmConf.termConfig.temp2;
	}
	else err++;
	if (stmConf.termConfig.temp3 > 0 && stmConf.termConfig.temp3 < 500) {
		minibotConfig.temp3 = stmConf.termConfig.temp3;
	}
	else err++;
	if (stmConf.termConfig.Kp > 0) {
		minibotConfig.Kp = stmConf.termConfig.Kp;
	}
	else err++;
	if (stmConf.termConfig.Ki > 0) {
		minibotConfig.Ki = stmConf.termConfig.Ki;
	}
	else err++;
	if (stmConf.termConfig.Kd > 0) {
		minibotConfig.Kd = stmConf.termConfig.Kd;
	}
	else err++;
	if (err)
	{
		globData.LEDB = LEDB_ERROR;
		return err;
	}
	minibotConfig.flash_init = FLASH_INIT;
	flashWriteData(&minibotConfig);
	globData.LEDB = LEDB_FLASH_OK;
	return 0;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	uint32_t er = HAL_UART_GetError(&huart1);
	switch (er) {
		case HAL_UART_ERROR_PE: // ошибка четности
			__HAL_UART_CLEAR_PEFLAG(&huart1);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_NE:  // шум на линии
			__HAL_UART_CLEAR_NEFLAG(&huart1);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_FE:  // ошибка фрейма
			__HAL_UART_CLEAR_FEFLAG(&huart1);
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
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == WIFI_UART_Ins) {
		new_wifi_data = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&WIFI_UART, wifi_uart_buff, sizeof(wifi_uart_buff));
		__HAL_DMA_DISABLE_IT(&WIFI_UART_DMA, DMA_IT_HT);
		//HAL_UART_Transmit(&WIFI_UART, (uint8_t*)"DEBA ok", 7, 100);
	}
	else if (huart->Instance == BMS_UART_Ins) {
		new_bms_data = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&BMS_UART, bms_uart_buff, sizeof(bms_uart_buff));
		__HAL_DMA_DISABLE_IT(&BMS_UART_DMA, DMA_IT_HT);
	}
	else if (huart->Instance == RC_UART_Ins) {
		new_remote_data = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&RC_UART, rc_uart_buff, sizeof(rc_uart_buff));
		__HAL_DMA_DISABLE_IT(&RC_UART_DMA, DMA_IT_HT);
	}
}
