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

#define SERVO_MIN_W 1740
#define SERVO_MAX_W 1945
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 270

class Servo
{
public:
	Servo(TIM_HandleTypeDef *htim, uint32_t timChannel);

	uint8_t setAngle(int32_t angle);
	uint32_t getAngle();
	uint32_t getStatus();
	uint8_t enable();
	uint8_t disable();
	uint8_t setAngleHold(int32_t angle);


private:
	int map(int x, int in_min, int in_max, int out_min, int out_max);

	TIM_HandleTypeDef *_htim;
	uint32_t _timChannel;
	uint32_t _angle;
	uint32_t _enabled;
	uint32_t _set_pos;
	uint32_t _curr_pos;
};

#endif /* CPP_INC_SERVO_H_ */
