/*
 * uart_wifi.cpp
 *
 *  Created on: May 10, 2023
 *      Author: USER
 */

#include <uart_wifi.h>
#include <string.h>
#include "termoplast_config.h"
#include "flash_data.h"

//#include <stdio.h> //for test

extern UART_HandleTypeDef WIFI_UART;
extern DMA_HandleTypeDef WIFI_UART_DMA;

extern GlobDataTypeDef globData;
extern TermoplastConfigTypeDef termoplastConfig;
uint8_t new_wifi_data = 0;
uint8_t wifi_uart_buff[100];
StatusMsgTypeDef statusMsg;
JobMsgTypeDef jobMsg;
StmConfigTypeDef stmConf;

void StartUartWiFiTask(void *argument)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&WIFI_UART, wifi_uart_buff, sizeof(wifi_uart_buff));
	__HAL_DMA_DISABLE_IT(&WIFI_UART_DMA, DMA_IT_HT);
	ConfigInit();
	for(;;)
	{
		if ((new_wifi_data && wifi_uart_buff[2] == WIFI_CONTROL_ID)
				&& wifi_uart_buff[0] == START_MSG0 && wifi_uart_buff[1] == START_MSG1) {
			enum WIFI_MSG_ID MSG_ID = (WIFI_MSG_ID)wifi_uart_buff[3];
			if (MSG_ID == WIFI_GET_STATUS) {
				if (wifi_uart_buff[RECEV_MSG_CS_BYTE_NUM]
						!= calculateCS(wifi_uart_buff, RECEV_MSG_CS_BYTE_NUM)) {
					globData.cs_err++;
				}
				else {
					SendStatus();
				}
			}
			else if (MSG_ID == WIFI_SET_MANUAL_COM) {
				if (wifi_uart_buff[RECEV_MSG_CS_BYTE_NUM]
						!= calculateCS(wifi_uart_buff, RECEV_MSG_CS_BYTE_NUM)) {
					globData.cs_err++;
				}
				else {
					SetManual();
				}
			}
			else if (MSG_ID == WIFI_SET_JOB) {
				if (wifi_uart_buff[sizeof(JobMsgTypeDef) - 1]
						!= calculateCS(wifi_uart_buff, sizeof(JobMsgTypeDef) - 1)) {
					globData.cs_err++;
				}
				else {
					jobMsg = *(JobMsgTypeDef*) wifi_uart_buff;
					SetJob();
				}
			}
			else if (MSG_ID == WIFI_GET_STM_CONFIG) {
				if (wifi_uart_buff[RECEV_MSG_CS_BYTE_NUM]
						!= calculateCS(wifi_uart_buff, RECEV_MSG_CS_BYTE_NUM)) {
					globData.cs_err++;
				}
				else {
					GetSTMConfig();
				}
			}
			else if (MSG_ID == WIFI_SET_STM_CONFIG) {
				if (wifi_uart_buff[sizeof(StmConfigTypeDef)-1] != calculateCS(wifi_uart_buff, sizeof(StmConfigTypeDef)-1)) {
					globData.cs_err++;
				} else {
					stmConf = *(StmConfigTypeDef*)wifi_uart_buff;
					ConfigUpdate();
				}
			}
			new_wifi_data = 0;
			memset(wifi_uart_buff, 0, 100);
		}
		/*if (globData.current_status == STATUS_MANUAL && (globData.current_move_comm != 0 || globData.current_lift_comm != 0)) {
			if (HAL_GetTick() - release_button_tick > 500) {
				globData.current_move_comm = MOVE_STOP;
			}
			if (HAL_GetTick() - release_button_tick > 5000) {
				globData.current_lift_comm = LIFT_STOP;
			}
		}*/

		//osDelay(1000);
		//SendStatus(); //for test
	}
}

void SendStatus()
{
	statusMsg.start_msg0 = START_MSG0;
	statusMsg.start_msg1 = START_MSG1;
	statusMsg.control_id = WIFI_CONTROL_ID;
	statusMsg.msg_id = WIFI_GET_STATUS;
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
	if (jobMsg.comm == JOB_START)
	{
		globData.volume = jobMsg.volume;
		globData.time_hold = jobMsg.time_hold;
		globData.cycles_set = jobMsg.cycles;
		globData.heat_on = 1;
	}
	else if (jobMsg.comm != JOB_PAUSE || jobMsg.comm != JOB_RESUME)
	{
		globData.heat_on = 0;
	}
	globData.current_status = jobMsg.comm;
}

void GetSTMConfig()
{
	stmConf.start_msg0 = START_MSG0;
	stmConf.start_msg1 = START_MSG1;
	stmConf.control_id = WIFI_CONTROL_ID;
	stmConf.msg_id = WIFI_GET_STM_CONFIG;
	stmConf.termConfig = termoplastConfig;
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
	flashReadData(&termoplastConfig);
	if (termoplastConfig.flash_init != FLASH_INIT)
	{
		termoplastConfig.volume_per_rev = 100.0;
		termoplastConfig.volume = 10.0;
		termoplastConfig.motor1_speed = 1000;
		termoplastConfig.motor1_acc = 500;
		termoplastConfig.motor2_speed = 1000;
		termoplastConfig.motor2_acc = 500;
		termoplastConfig.time_hold = 10;
		termoplastConfig.temp1 = 30.0;
		termoplastConfig.temp2 = 30.0;
		termoplastConfig.temp3 = 30.0;
		termoplastConfig.Kp = 1.0;
		termoplastConfig.Ki = 1.0;
		termoplastConfig.Kd = 1.0;
		termoplastConfig.bitParams.ind = 1;
	}
}

int ConfigUpdate()
{
	int err = 0;
	if (stmConf.termConfig.volume_per_rev > 0) {
		termoplastConfig.volume_per_rev = stmConf.termConfig.volume_per_rev;
	}
	else err++;
	if (stmConf.termConfig.volume > 0) {
		termoplastConfig.volume = stmConf.termConfig.volume;
	}
	else err++;
	if (stmConf.termConfig.motor1_speed > 0 && stmConf.termConfig.motor1_speed < 5000)
	{
		termoplastConfig.motor1_speed = stmConf.termConfig.motor1_speed;
	}
	else err++;
	if (stmConf.termConfig.motor1_acc > 0 && stmConf.termConfig.motor1_acc < 5000)
	{
		termoplastConfig.motor1_acc= stmConf.termConfig.motor1_acc;
	}
	else err++;
	if (stmConf.termConfig.motor2_speed > 0 && stmConf.termConfig.motor2_speed < 5000) {
		termoplastConfig.motor2_speed = stmConf.termConfig.motor2_speed;
	}
	else err++;
	if (stmConf.termConfig.motor2_acc > 0 && stmConf.termConfig.motor2_acc < 5000) {
		termoplastConfig.motor2_acc = stmConf.termConfig.motor2_acc;
	}
	else err++;
	if (stmConf.termConfig.motor2_acc > 0 && stmConf.termConfig.motor2_acc < 5000) {
		termoplastConfig.motor2_acc = stmConf.termConfig.motor2_acc;
	}
	else err++;
	if (stmConf.termConfig.time_hold > 0) {
		termoplastConfig.time_hold = stmConf.termConfig.time_hold;
	}
	else err++;
	if (stmConf.termConfig.temp1 > 0 && stmConf.termConfig.temp1 < 500) {
		termoplastConfig.temp1 = stmConf.termConfig.temp1;
	}
	else err++;
	if (stmConf.termConfig.temp2 > 0 && stmConf.termConfig.temp2 < 500) {
		termoplastConfig.temp2 = stmConf.termConfig.temp2;
	}
	else err++;
	if (stmConf.termConfig.temp3 > 0 && stmConf.termConfig.temp3 < 500) {
		termoplastConfig.temp3 = stmConf.termConfig.temp3;
	}
	else err++;
	if (stmConf.termConfig.Kp > 0) {
		termoplastConfig.Kp = stmConf.termConfig.Kp;
	}
	else err++;
	if (stmConf.termConfig.Ki > 0) {
		termoplastConfig.Ki = stmConf.termConfig.Ki;
	}
	else err++;
	if (stmConf.termConfig.Kd > 0) {
		termoplastConfig.Kd = stmConf.termConfig.Kd;
	}
	else err++;
	if (err)
	{
		globData.LEDB = LEDB_ERROR;
		return err;
	}
	termoplastConfig.flash_init = FLASH_INIT;
	flashWriteData(&termoplastConfig);
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
	}
}
