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

#define LK_ENC_ONE_PERIOD 65536
#define LK_ENC_HALF_PERIOD 32768

class KeyaLKTechDriver
{

public:
	KeyaLKTechDriver(uint32_t extId, uint32_t axis, uint32_t stdId);
	KeyaLKTechDriver(uint32_t extId, uint32_t axis);
	KeyaLKTechDriver(uint32_t stdId);
	uint8_t setSpeed(int32_t speed);
	uint8_t setPos(int32_t pos);
	void setEnc(int32_t pos);
	void setTemp(int8_t temp);
	void setOffset();
	uint8_t stop();
	uint8_t stopHold();
	uint8_t enable();
	uint8_t disable();
	uint8_t readEnc();
	int32_t getPos();
	int32_t getSpeed();
	uint32_t getId();
	int8_t getTemp();
	int32_t UnwrapEncoder(uint16_t in, int32_t *prev);

private:
	uint8_t sendData();
	CAN_TxHeaderTypeDef _canTxHeader;
	uint8_t _canData[8];
	uint32_t _axis;
	int32_t _speed;
	int32_t _enc;
	int32_t _enc_offset = 0;
	int32_t _prevEnc;
	int8_t _temp;
};


#endif /* CPP_INC_KEYALKTECHDRIVER_H_ */
