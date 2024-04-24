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
#include "servo_control.h"
#include "Servo.h"

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
extern Servo *servo[2];

extern ContrlMsgTypeDef contrlMsg;
extern StatusMsgTypeDef statusMsg;

BatteryMsgTypeDef batteryMsg;

uint8_t new_wifi_data = 0;
uint8_t new_remote_data = 0;
uint8_t new_bms_data = 0;

uint8_t rc_uart_buff[100];
uint8_t bms_uart_buff[100];
uint8_t wifi_uart_buff[100];
int32_t size_recv_buff = 0;
uint32_t bms_req_time = 0;
int32_t battery_capacity = 0;
uint8_t bms_err = 0;
uint8_t bms_detected = 0;
uint8_t smart_bms = 0;
uint8_t bms_jbd_request_msg[] = {0x01, 0x03, 0x00, 0x2E, 0x00, 0x01, 0xE4, 0x03};
uint8_t bms_jbd_request_msg0[] = {0xDD, 0xA5, 0x03, 0x00, 0xFF, 0xFD, 0x77};
uint8_t bms_jbd_request_msg1[] = {0xDD, 0xA5, 0x04, 0x00, 0xFF, 0xFC, 0x77};
uint8_t bms_smart_request_msg[]  = {0xA5, 0x40, 0x90, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D};

void StartUartWiFiTask(void *argument)
{
	for(;;)
	{
		if (new_wifi_data)
		{
			if (wifi_uart_buff[0] == START_MSG0 && wifi_uart_buff[1] == START_MSG1)
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
						if ((globData.current_comm == MOVE_NONE || wifi_uart_buff[3] == MOVE_EMERGY_STOP || wifi_uart_buff[3] == MOVE_RESET) && wifi_uart_buff[3] < MOVE_MAX)
						{
							memcpy(&contrlMsg, wifi_uart_buff, sizeof(ContrlMsgTypeDef));
							globData.current_comm = contrlMsg.comm;
						}
						globData.comm_count++;
						checkData();
						sendStatus();
					}
				}

				//memset(wifi_uart_buff, 0, 100);
			}
			else globData.cs_err++;
			new_wifi_data = 0;
			osDelay(2);
		}
		if (new_bms_data)
		{
			new_bms_data = 0;
			bms_err = 0;
			rcGetBattery();
			globData.error.bms_err = 0;
		}

		if (HAL_GetTick() - bms_req_time > 1000) {
			if (!bms_detected) {
				//HAL_UART_Transmit(&BMS_UART, (uint8_t*) bms_smart_request_msg, sizeof(bms_smart_request_msg), 100);
				HAL_UART_Transmit(&BMS_UART, (uint8_t*) bms_jbd_request_msg0, sizeof(bms_jbd_request_msg0), 100);
				bms_detected = 1;
			}
			else {
				if (smart_bms) HAL_UART_Transmit(&BMS_UART,	(uint8_t*) bms_smart_request_msg, sizeof(bms_smart_request_msg), 100);
				else HAL_UART_Transmit(&BMS_UART, (uint8_t*) bms_jbd_request_msg0, sizeof(bms_jbd_request_msg0), 100);
			}
			bms_req_time = HAL_GetTick();
			bms_err++;
			if (bms_err > 5) {
				globData.capacity = 0;
				bms_detected = 0;
				batteryMsg.bms_type = BMS_NONE;
				globData.error.bms_err = 1;
			}
		}
	}
}

void sendStatus()
{
	statusMsg.start_msg0 = START_MSG0;
	statusMsg.start_msg1 = START_MSG1;
	statusMsg.msg_id = MSG_STATUS;
	statusMsg.comm = globData.current_comm;
	statusMsg.pos_x = mdrivers[0]->getPos();
	statusMsg.pos_y = mdrivers[2]->getPos();
	statusMsg.pos_fork = mdrivers[3]->getPos();
	statusMsg.pos_servo = servo[0]->getStatus();
	statusMsg.action_comm = globData.action_comm;
	statusMsg.capacity = globData.capacity;
	statusMsg.sens = globData.sens;
	statusMsg.error = globData.error;
	statusMsg.cs_err = globData.cs_err;
	statusMsg.comm_count = globData.comm_count;
	statusMsg.msg_count++;
	statusMsg.CS = calculateCS((uint8_t *)&statusMsg, sizeof(statusMsg)-1);
	HAL_UART_Transmit(&WIFI_UART, (uint8_t*)&statusMsg, sizeof(statusMsg), 100);
}

void checkData()
{
	if (contrlMsg.comm == MOVE_RESET)
	{
		*(uint16_t*)&globData.error = 0;
		globData.current_comm = MOVE_RESET;
		globData.comm_count = 0;
		globData.cs_err = 0;
	}
	else if (contrlMsg.comm == MOVE_ACTION)
	{
		globData.action_comm = contrlMsg.action_comm;
		contrlMsg.comm = 0;
		contrlMsg.action_comm = 0;
		contrlMsg.pos_servo = 0;
	}
}

void SetManual()
{

}

void rcGetBattery() {
	if (bms_uart_buff[0] == 0xDD) smart_bms = 0;
	else if (bms_uart_buff[0] == 0xA5) smart_bms = 1;
	if (smart_bms)
	{
		uint8_t battery_comm = bms_uart_buff[2];
		if (battery_comm == 0x90)
		{
			batteryMsg.bms_type = BMS_SMART;

			batteryMsg.voltage = (bms_uart_buff[4] << 8) + bms_uart_buff[5];
			batteryMsg.current = (bms_uart_buff[8] << 8) + bms_uart_buff[9];
			batteryMsg.capacity_percent = (bms_uart_buff[10] << 8) + bms_uart_buff[11];
			globData.capacity = batteryMsg.capacity_percent/10;
		}
		else if (battery_comm == 0x91)
		{
			batteryMsg.max_volt = (bms_uart_buff[4] << 8) + bms_uart_buff[5];
			batteryMsg.min_volt = (bms_uart_buff[7] << 8) + bms_uart_buff[8];
		}
		else if (battery_comm == 0x92)
		{

		}
		else if (battery_comm == 0x93)
		{
			batteryMsg.remaining_capacity = (bms_uart_buff[8] << 24) +(bms_uart_buff[9] << 16) +(bms_uart_buff[10] << 8) + bms_uart_buff[11];
		}
		else if (battery_comm == 0x94)
		{
			batteryMsg.num_of_battery = bms_uart_buff[4];
			batteryMsg.num_of_NTC = bms_uart_buff[5];
		}
		else if (battery_comm == 0x95)
		{
			if (bms_uart_buff[4] == 0x01)
			{
				batteryMsg.cell_0 = (bms_uart_buff[5] << 8) + bms_uart_buff[6];
				batteryMsg.cell_1 = (bms_uart_buff[7] << 8) + bms_uart_buff[8];
				batteryMsg.cell_2 = (bms_uart_buff[9] << 8) + bms_uart_buff[10];
			}
			else if (bms_uart_buff[4] == 0x02)
			{
				batteryMsg.cell_3 = (bms_uart_buff[5] << 8) + bms_uart_buff[6];
				batteryMsg.cell_4 = (bms_uart_buff[7] << 8) + bms_uart_buff[8];
				batteryMsg.cell_5 = (bms_uart_buff[9] << 8) + bms_uart_buff[10];
				//batteryMsg.cell_3 = (bms_uart_buff[18] << 8) + bms_uart_buff[19];
				//batteryMsg.cell_4 = (bms_uart_buff[20] << 8) + bms_uart_buff[21];
				//batteryMsg.cell_5 = (bms_uart_buff[22] << 8) + bms_uart_buff[23];
			}
			else if (bms_uart_buff[4] == 0x03)
			{
				batteryMsg.cell_6 = (bms_uart_buff[5] << 8) + bms_uart_buff[6];
				batteryMsg.cell_7 = (bms_uart_buff[7] << 8) + bms_uart_buff[8];
				batteryMsg.cell_8 = (bms_uart_buff[9] << 8) + bms_uart_buff[10];
				//batteryMsg.cell_6 = (bms_uart_buff[27] << 8) + bms_uart_buff[28];
				//batteryMsg.cell_7 = (bms_uart_buff[29] << 8) + bms_uart_buff[30];
				//batteryMsg.cell_8 = (bms_uart_buff[31] << 8) + bms_uart_buff[32];
			}
			else if (bms_uart_buff[4] == 0x04)
			{
				batteryMsg.cell_9 = (bms_uart_buff[5] << 8) + bms_uart_buff[6];
				batteryMsg.cell_10 = (bms_uart_buff[7] << 8) + bms_uart_buff[8];
				batteryMsg.cell_11 = (bms_uart_buff[9] << 8) + bms_uart_buff[10];
				//batteryMsg.cell_9 = (bms_uart_buff[34] << 8) + bms_uart_buff[35];
				//batteryMsg.cell_10 = (bms_uart_buff[36] << 8) + bms_uart_buff[37];
				//batteryMsg.cell_11 = (bms_uart_buff[38] << 8) + bms_uart_buff[39];
			}
			else if (bms_uart_buff[4] == 0x05)
			{
				batteryMsg.cell_12 = (bms_uart_buff[5] << 8) + bms_uart_buff[6];
				batteryMsg.cell_13 = (bms_uart_buff[7] << 8) + bms_uart_buff[8];
				batteryMsg.cell_14 = (bms_uart_buff[9] << 8) + bms_uart_buff[10];
				//batteryMsg.cell_12 = (bms_uart_buff[40] << 8) + bms_uart_buff[41];
				//batteryMsg.cell_13 = (bms_uart_buff[42] << 8) + bms_uart_buff[43];
				//batteryMsg.cell_14 = (bms_uart_buff[44] << 8) + bms_uart_buff[45];
			}
			else if (bms_uart_buff[4] == 0x06)
			{
				batteryMsg.cell_15 = (bms_uart_buff[5] << 8) + bms_uart_buff[6];
				//batteryMsg.cell_15 = (bms_uart_buff[47] << 8) + bms_uart_buff[48];
			}
			bms_smart_request_msg[2] = 0x95;
			bms_smart_request_msg[12] = 0x82;
		}
		else if (battery_comm == 0x96)
		{
			batteryMsg.temp1 = bms_uart_buff[5]; //-40 to convert
			batteryMsg.temp2 = bms_uart_buff[6];
		}
		bms_smart_request_msg[2]++;
		bms_smart_request_msg[12]++;
		if (bms_smart_request_msg[2] > 0x96)
		{
			bms_smart_request_msg[2] = 0x90;
			bms_smart_request_msg[12] = 0x7D;
		}
	}
	else
	{
		uint8_t battery_comm = bms_uart_buff[1];
		if (battery_comm == 0x03)
		{
			batteryMsg.bms_type = BMS_JBD;
			batteryMsg.voltage = (bms_uart_buff[4] << 8) + bms_uart_buff[5];
			//batteryMsg.current = 65536 - ((bms_uart_buff[6] << 8) + bms_uart_buff[7]);
			//if (bms_uart_buff[6] & (1 << 8)) batteryMsg.current = -batteryMsg.current;
			batteryMsg.current = 0;//(bms_uart_buff[6] << 8) + bms_uart_buff[7];
			batteryMsg.remaining_capacity = (uint32_t)((bms_uart_buff[8] << 8) + bms_uart_buff[9]);
			batteryMsg.nominal_capacity = (bms_uart_buff[10] << 8) + bms_uart_buff[11];
			batteryMsg.cycles = (bms_uart_buff[12] << 8) + bms_uart_buff[13];
			batteryMsg.date = (bms_uart_buff[14] << 8) + bms_uart_buff[15];
			batteryMsg.balance_low = (bms_uart_buff[16] << 8) + bms_uart_buff[17];
			batteryMsg.balance_high = (bms_uart_buff[18] << 8) + bms_uart_buff[19];
			batteryMsg.protection = (bms_uart_buff[20] << 8) + bms_uart_buff[21];
			batteryMsg.version = bms_uart_buff[22];
			batteryMsg.capacity_percent = (uint16_t)bms_uart_buff[23];
			batteryMsg.MOS_state = bms_uart_buff[24];
			batteryMsg.num_of_battery = bms_uart_buff[25];
			batteryMsg.num_of_NTC = bms_uart_buff[26];
			batteryMsg.temp1 = ((bms_uart_buff[27] << 8) + bms_uart_buff[28]);
			batteryMsg.temp2 = ((bms_uart_buff[29] << 8) + bms_uart_buff[30]);

			globData.capacity = batteryMsg.capacity_percent;

			HAL_UART_Transmit(&BMS_UART, (uint8_t*)bms_jbd_request_msg1, sizeof(bms_jbd_request_msg1), 100);
		}
		else if (battery_comm == 0x04)
		{
			batteryMsg.battery_pack = bms_uart_buff[3];
			batteryMsg.cell_0 = (bms_uart_buff[4] << 8) + bms_uart_buff[5];
			batteryMsg.cell_1 = (bms_uart_buff[6] << 8) + bms_uart_buff[7];
			batteryMsg.cell_2 = (bms_uart_buff[8] << 8) + bms_uart_buff[9];
			batteryMsg.cell_3 = (bms_uart_buff[10] << 8) + bms_uart_buff[11];
			batteryMsg.cell_4 = (bms_uart_buff[12] << 8) + bms_uart_buff[13];
			batteryMsg.cell_5 = (bms_uart_buff[14] << 8) + bms_uart_buff[15];
			batteryMsg.cell_6 = (bms_uart_buff[16] << 8) + bms_uart_buff[17];
			batteryMsg.cell_7 = (bms_uart_buff[18] << 8) + bms_uart_buff[19];
			batteryMsg.cell_8 = (bms_uart_buff[20] << 8) + bms_uart_buff[21];
			batteryMsg.cell_9 = (bms_uart_buff[22] << 8) + bms_uart_buff[23];
			batteryMsg.cell_10 = (bms_uart_buff[24] << 8) + bms_uart_buff[25];
			batteryMsg.cell_11 = (bms_uart_buff[26] << 8) + bms_uart_buff[27];
			batteryMsg.cell_12 = (bms_uart_buff[28] << 8) + bms_uart_buff[29];
			batteryMsg.cell_13 = (bms_uart_buff[30] << 8) + bms_uart_buff[31];
			batteryMsg.cell_14 = (bms_uart_buff[32] << 8) + bms_uart_buff[33];
			batteryMsg.cell_15 = (bms_uart_buff[34] << 8) + bms_uart_buff[35];
		}
	}
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
		case HAL_UART_ERROR_PE:
			__HAL_UART_CLEAR_PEFLAG(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_NE:
			__HAL_UART_CLEAR_NEFLAG(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_FE:
			__HAL_UART_CLEAR_FEFLAG(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_ORE:
			__HAL_UART_CLEAR_OREFLAG(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
			break;
		case HAL_UART_ERROR_DMA:
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
