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
	KeyaLKTechDriver(uint32_t extId, uint32_t axis, uint32_t stdId, GlobDataTypeDef &globData);
	KeyaLKTechDriver(uint32_t extId, uint32_t axis, GlobDataTypeDef &globData);
	KeyaLKTechDriver(uint32_t stdId, GlobDataTypeDef &globData);
	uint8_t setSpeed(int32_t speed);
	uint8_t setPos(int32_t pos);
	void setEnc(int32_t pos);
	void setTemp(int8_t temp);
	void setOffset();
	void setHolding(int8_t hold);
	void setError(int8_t error);
	uint8_t stop();
	uint8_t stopHold();
	uint8_t enable();
	uint8_t disable();
	uint8_t readEnc();
	uint8_t readError();
	uint8_t resetError();
	int32_t getPos();
	int32_t getSpeed();
	uint32_t getId();
	int8_t getTemp();
	int8_t getError();
	int8_t getHolding();
	int32_t error_count{};

private:
	int32_t UnwrapEncoder(uint16_t in, int32_t *prev);
	uint8_t sendData();
	CAN_TxHeaderTypeDef _canTxHeader;
	uint8_t _canData[8];
	uint32_t _axis{};
	int32_t _speed{};
	int32_t _enc{};
	int32_t _enc_offset{};
	int32_t _prevEnc{};
	int8_t _temp{};
	int8_t _holding{};
	int8_t _enabled{};
	int8_t _error{};
	GlobDataTypeDef &_globData;
};


#endif /* CPP_INC_KEYALKTECHDRIVER_H_ */
