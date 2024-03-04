/*
 * lkTechDriver.h
 *
 *  Created on: Feb 27, 2024
 *      Author: user
 */

#ifndef CPP_INC_KEYALKTECHDRIVER_H_
#define CPP_INC_KEYALKTECHDRIVER_H_

#include "main.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
#include "minibot_config.h"
#include "canbus.h"

class KeyaLKTechDriver
{
public:
	KeyaLKTechDriver(uint32_t extId, uint32_t axis, uint32_t stdId);
	KeyaLKTechDriver(uint32_t extId, uint32_t axis);
	KeyaLKTechDriver(uint32_t stdId);
	uint8_t setSpeed(int32_t speed);
	uint8_t stop();
	uint8_t enable();
	uint8_t disable();
	uint8_t getEnc();

private:
	uint8_t sendData();
	CAN_TxHeaderTypeDef _canTxHeader;
	uint8_t _canData[8];
	uint32_t _axis;
	int32_t _speed;
};



#endif /* CPP_INC_KEYALKTECHDRIVER_H_ */
