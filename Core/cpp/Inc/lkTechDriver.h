/*
 * lkTechDriver.h
 *
 *  Created on: Feb 27, 2024
 *      Author: user
 */

#ifndef CPP_INC_LKTECHDRIVER_H_
#define CPP_INC_LKTECHDRIVER_H_

#include "main.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
#include "minibot_config.h"
#include "canbus.h"

class LkTechDriver
{
public:
	LkTechDriver(uint32_t id);

private:
	uint32_t _id = 0;
	CanDataSendTypeDef canDataSend;

};



#endif /* CPP_INC_LKTECHDRIVER_H_ */
