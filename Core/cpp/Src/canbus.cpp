/*
 * canbus.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: user
 */

#include "canbus.h"
#include "InitCanDrivers.h"

extern UART_HandleTypeDef DEBUG_UART;
extern CAN_HandleTypeDef hcan1;
extern GlobDataTypeDef globData;

extern UART_HandleTypeDef WIFI_UART;
extern DMA_HandleTypeDef WIFI_UART_DMA;

volatile uint8_t NewCanMsg = 0;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8] = {0, };
uint8_t RxData[8] = {0, };
uint32_t TxMailbox = 0;
CanDataRecvTypeDef canDataRecv;
CanDataSendTypeDef prevCanData;

extern KeyaLKTechDriver DriverFB;
extern KeyaLKTechDriver DriverLR;

extern KeyaLKTechDriver *mdrivers[DRIVERS_QUANT];


void StartCanTask(void *argument) {
	UNUSED(argument);
	for (;;) {
		if (NewCanMsg == CAN_GET_MSG_OK) {
			NewCanMsg = CAN_GET_MSG_WAIT;
			CanMsgRead(&canDataRecv);
		}
		else if (NewCanMsg == CAN_GET_MSG_ERROR) {
			NewCanMsg = CAN_GET_MSG_WAIT;
		}
		osDelay(1);
	}
}

uint8_t CanMsgRead(CanDataRecvTypeDef *canDataRecv) {

	NVIC_DisableIRQ(CAN1_RX0_IRQn);
	if (RxHeader.IDE == CAN_STD_ID)
	{
		for (int i=0; i<2; i++)
		{
			if (RxHeader.StdId == mdrivers[i]->getId()) {
				if (RxData[0] == 0x9C)
				{
					mdrivers[i]->setEnc(*(uint16_t*) &RxData[6]);
					mdrivers[i]->setTemp(RxData[1]);
					mdrivers[i]->error_count = 0;
				}
				else if (RxData[0] == 0x9A)
				{
					mdrivers[i]->setError(RxData[7]);
				}
			}
		}
	}
	else
	{
		if (RxHeader.ExtId == 0x05800000 + DRIVER_KEYA_ID) {
			mdrivers[2]->error_count = 0;
			if (RxData[0] == 0x60)
			{
				mdrivers[2]->setTemp(RxData[6]);
				mdrivers[3]->setTemp(RxData[7]);
			}
			else if (RxData[1] == 0x0D) {
				//globData.voltage = RxData[7];
			}
			else if (RxData[1] == 0x12) {
				//diagMsg.motor1 = *(DriverErrMsgTypeDef*)&RxData[4];
				//diagMsg.motor2 = *(DriverErrMsgTypeDef*)&RxData[6];
			}
		}
	}
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	return 1;
}


uint8_t CanMsgSend(CAN_TxHeaderTypeDef *TxHeader, uint8_t canData[]) {

	if (TxHeader->ExtId != 0 || TxHeader->StdId != 0)
	{
		while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);
		if (HAL_CAN_AddTxMessage(&hcan1, TxHeader, canData, &TxMailbox) != HAL_OK) {
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



