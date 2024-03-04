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

extern UART_HandleTypeDef WIFI_UART;
extern DMA_HandleTypeDef WIFI_UART_DMA;

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
	/*//uint32_t drvId = DRIVER_MOVE_ID + 0x05800000;

	//uint32_t drvId = DRIVER_MOVE_ID + 0x07000000;
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
	}*/

	//uint32_t drvId = 0x140 + DRIVER1_LKTECH_ID;
	//if (RxHeader.StdId == drvId && RxData[0] == 0x9C) {
			globData.LKTemp = RxData[1];//*(uint16_t*) &RxData[6];
	//}
			globData.LKEncoder = *(uint16_t*) &RxData[6];
			//HAL_UART_Transmit(&WIFI_UART, (uint8_t*) "get\r\n", 5, 100);
			//HAL_UART_Transmit(&WIFI_UART, RxData, 8, 100);

	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	return 1;
}


uint8_t CanMsgSend(CAN_TxHeaderTypeDef *TxHeader, uint8_t canData[]) {

	if (TxHeader->ExtId != 0 || TxHeader->StdId != 0)
	{
		while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);
		if (HAL_CAN_AddTxMessage(&hcan1, TxHeader, canData, &TxMailbox) != HAL_OK) {
			//HAL_UART_Transmit(&huart1, (uint8_t*) "no_trans\r\n", 10, 100);
			return 0;
		}
	}
	return 1;
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



