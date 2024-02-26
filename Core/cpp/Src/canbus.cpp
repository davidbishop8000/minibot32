/*
 * canbus.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: user
 */

#include "canbus.h"

//extern CAN_TxHeaderTypeDef TxHeader;
//extern CAN_RxHeaderTypeDef RxHeader;
//extern uint8_t TxData[8];
//extern uint8_t RxData[8];
//extern uint32_t TxMailbox;

extern UART_HandleTypeDef DEBUG_UART;
extern CAN_HandleTypeDef hcan1;
extern GlobDataTypeDef globData;
char ns_str[30];
float velocity = 20.0;
volatile uint8_t NewCanMsg = 0;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8] = { 0, };
uint8_t RxData[8] = { 0, };
uint32_t TxMailbox = 0;
CanDataRecvTypeDef canDataRecv;
CanDataSendTypeDef prevCanData;

void StartCanTask(void *argument) {
	UNUSED(argument);
	for (;;) {
		static uint32_t err_count = 0;
		if (NewCanMsg == CAN_GET_MSG_OK) {
			NewCanMsg = CAN_GET_MSG_WAIT;
			err_count = 0;
			//globData.error.driver_conn = 0;
			CanMsgRead(&canDataRecv);
		}
		else if (NewCanMsg == CAN_GET_MSG_ERROR) {
			//HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) "CAN error\r\n", 11, 100);
			err_count++;
			if (err_count > 20 && DEBUG_MODE == 0)
			{
				/*globData.error.driver_conn = 1;
				globData.drv_cpu_temp = 0;
				globData.motor1_temp = 0;
				globData.motor2_temp = 0;
				globData.voltage = 0;*/
			}
			NewCanMsg = CAN_GET_MSG_WAIT;
		}
		osDelay(1);
	}
}

uint8_t CanMsgRead(CanDataRecvTypeDef *canDataRecv) {

	NVIC_DisableIRQ(CAN1_RX0_IRQn);

	/*uint32_t drvId = RxHeader.StdId >> 5;
	uint32_t cmd_id;
	float voltage;
	if (drvId == DRIVER_MOVE_ID)
	{
		cmd_id = DRIVER_MOVE_ID << 5 ^ RxHeader.StdId;
		switch(cmd_id) {
		case MSG_ODRIVE_HEARTBEAT:
			//HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) "heart\r\n", 7, 100);
			break;
		case MSG_GET_MOTOR_ERROR:
			HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) "mot_er\r\n", 8, 100);
			break;
		case MSG_GET_ENCODER_ERROR:
			HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) "enc_er\r\n", 8, 100);
			break;
		case MSG_SET_AXIS_REQUESTED_STATE:
			HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) "state\r\n", 7, 100);
			break;
		case MSG_GET_ENCODER_COUNT:
			//HAL_UART_Transmit(&huart1, (uint8_t*) "count\r\n", 8, 100);
			break;
		case MSG_GET_VBUS_VOLTAGE:
			voltage = *(float *)&RxData;
			canDataRecv->voltage = voltage;
			char str[20];
			sprintf(str, "voltage %.2f\r\n", canDataRecv->voltage);
			HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) str, strlen(str), 100);
			break;
		default:
			break;
		}
	}
	HAL_UART_Transmit(&DEBUG_UART, RxData, 8, 100);
	*/
//	else if (drvId == DRIVER_LIFT_ID)
//	{

//	}
	/*uint32_t drvId = RxHeader.StdId - 0x580;
	if (drvId == DRIVER_MOVE_ID) {
		if (RxData[0] == 0x40 && RxData[1] == 0x04 && RxData[2] == 0x21 && RxData[3] == 0x01)
		{
			globData.enc_mot = *(int32_t*)&RxData[4];
		}
	}
	*/
#ifdef DRIVER_SINUS
	//uint32_t drvId = DRIVER_MOVE_ID + 0x05800000;

//	uint32_t drvId = DRIVER_MOVE_ID + 0x07000000;
	if (RxHeader.ExtId == 0x05800001 && RxData[0] == 0x60) {
		if (RxData[1] == 0x0F) {
			globData.drv_cpu_temp = RxData[5];
			globData.motor1_temp = RxData[6];
			globData.motor2_temp = RxData[7];
		}
		else if (RxData[1] == 0x0D) {
			globData.voltage = RxData[7];
		}
		else if (RxData[1] == 0x12) {
			//RxData[4]; //data H fault of motor 1
			//RxData[5]; //data L fault of motor 1
			//RxData[6]; //data H fault of motor 2
			//RxData[7]; //data L fault of motor 2
			diagMsg.motor1 = *(DriverErrMsgTypeDef*)&RxData[4];
			diagMsg.motor2 = *(DriverErrMsgTypeDef*)&RxData[6];
		}
	}
#endif
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	return 1;
}


uint8_t CanMsgSend(CanDataSendTypeDef *canDataSend) {


	//uint8_t check_msg = 0; //проверка на тоже самое сообщение
	/*if (canDataSend->canId != prevCanData.canId || canDataSend->canRTR != prevCanData.canRTR) {
		check_msg = 1;
	}
	if (check_msg == 0) {
		for (int i=0; i < sizeof(canDataSend->canData); i++) {
			if (canDataSend->canData[i] != prevCanData.canData[i]) {
				check_msg = 1;
			}
		}
	}
	if (check_msg == 0) return 0;*/
#ifdef DRIVER_SINUS
	//////////////////////////SINUS china drv///////////////////
	/*prevCanData.canId = canDataSend->canId;
	prevCanData.canRTR = canDataSend->canRTR;
	for (int i = 0; i < sizeof(canDataSend->canData); i++) {
		prevCanData.canData[i] = canDataSend->canData[i];
	}*/
	TxHeader.StdId = canDataSend->canId;
	TxHeader.ExtId = canDataSend->canExtId;
	TxHeader.RTR = canDataSend->canRTR;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = 0;
	//globData.can_mutex = 0;
	for (int i = 0; i < sizeof(TxData); i++) {
		TxData[i] = canDataSend->canData[i];
	}
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
		//HAL_UART_Transmit(&huart1, (uint8_t*) "no_trans\r\n", 10, 100);
		return 0;
	}
	return 1;
#else
	/////////////////////china drv//////////////////
	prevCanData.canId = canDataSend->canId;
	prevCanData.canRTR = canDataSend->canRTR;
	for (uint32_t i=0; i < sizeof(canDataSend->canData); i++) {
		prevCanData.canData[i] = canDataSend->canData[i];
	}
	TxHeader.StdId = canDataSend->canId;
	TxHeader.ExtId = 0;
	TxHeader.RTR = canDataSend->canRTR;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	for(uint32_t i = 0; i < sizeof(TxData); i++) {
		TxData[i] = canDataSend->canData[i];
	}
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
		//HAL_UART_Transmit(&huart1, (uint8_t*) "no_trans\r\n", 10, 100);
		return 0;
	}
	return 1;
#endif


	////////////////////////////vesc///////////////////

	/*
	//TxHeader.StdId = canDataSend->canId;
	TxHeader.ExtId = canDataSend->canExtId;
	TxHeader.RTR = canDataSend->canRTR;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = 0;
	for(int i = 0; i < sizeof(TxData); i++) {
		TxData[i] = canDataSend->canData[i];
	}
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan2) == 0);
	if (HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
		return 0;
	}
	return 1;
	osDelay(10);
	*/

	///////////////////////////////////////////////////
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		Error_Handler();
	}
	NewCanMsg = CAN_GET_MSG_OK;
}
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	NewCanMsg = CAN_GET_MSG_ERROR;
}



