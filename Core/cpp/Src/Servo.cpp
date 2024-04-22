/*
 * servo.cpp
 *
 *  Created on: Feb 29, 2024
 *      Author: user
 */

#include "Servo.h"

Servo::Servo(TIM_HandleTypeDef *htim, uint32_t timChannel)
{
	_htim = htim;
	_timChannel = timChannel;
}
uint8_t Servo::setAngle(int32_t angle)
{
	HAL_TIM_PWM_Start(_htim, _timChannel);
	uint32_t angl = Servo::map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_MIN_W, SERVO_MAX_W);
//	if (_timChannel == TIM_CHANNEL_1) _htim->CCR1 = angl;
//	else if (_timChannel == TIM_CHANNEL_2) _htim->CCR2 = angl;
//	else if (_timChannel == TIM_CHANNEL_3) _htim->CCR3 = angl;
//	else if (_timChannel == TIM_CHANNEL_4) _htim->CCR4 = angl;
	__HAL_TIM_SET_COMPARE(_htim, _timChannel, angl);
	_angle = angle;
	//osDelay(800);
	//HAL_TIM_PWM_Stop(_htim, _timChannel);
	return 1;
}

uint8_t Servo::setAngleHold(int32_t angle)
{
	if (_enabled)
	{
		_angle = angle;
		__HAL_TIM_SET_COMPARE(_htim, _timChannel, Servo::map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_MIN_W, SERVO_MAX_W));
		osDelay(50);
		return 1;
	}
	return 0;
}
uint32_t Servo::getAngle()
{
	return _angle;
}
uint8_t Servo::enable()
{
	_enabled = 1;
	HAL_TIM_PWM_Start(_htim, _timChannel);
	return 1;
}
uint8_t Servo::disable()
{
	_enabled = 0;
	HAL_TIM_PWM_Stop(_htim, _timChannel);
	return 1;
}
int Servo::map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
