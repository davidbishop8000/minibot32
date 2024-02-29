/*
 * servo.h
 *
 *  Created on: Feb 29, 2024
 *      Author: user
 */

#ifndef CPP_INC_SERVO_H_
#define CPP_INC_SERVO_H_

#include "main.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
#include "minibot_config.h"

class Servo
{
public:
	Servo(TIM_HandleTypeDef tim, uint32_t timChannel);
	uint8_t setAngel(int32_t angel);
	uint32_t getAngel();
	uint8_t enable();
	uint8_t disable();

private:
	TIM_HandleTypeDef _tim;
	uint32_t _timChannel;
	uint32_t _angel;
};

#endif /* CPP_INC_SERVO_H_ */
