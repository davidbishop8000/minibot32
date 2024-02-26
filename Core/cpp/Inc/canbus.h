/*
 * canbus.h
 *
 *  Created on: Feb 26, 2024
 *      Author: user
 */

#ifndef INC_CAN_DRIVER_CONTROL_H_
#define INC_CAN_DRIVER_CONTROL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "cmsis_os2.h"
#include "minibot_config.h"

enum CanGetMsgStatus {
	CAN_GET_MSG_WAIT					=  0,
	CAN_GET_MSG_OK                      =  1,
	CAN_GET_MSG_ERROR                   =  2,
};

typedef struct {
	int32_t encoder;
	float voltage;
} CanDataRecvTypeDef;

typedef struct {
	uint32_t canId;
	uint32_t canExtId;
	uint32_t canRTR;
	uint8_t canData[8];
} CanDataSendTypeDef;

void StartCanTask(void *argument);
uint8_t CanMsgRead(CanDataRecvTypeDef *canData);
uint8_t CanMsgSend(CanDataSendTypeDef *canDataSend);

#ifdef __cplusplus
}
#endif

#endif /* INC_CAN_DRIVER_CONTROL_H_ */



