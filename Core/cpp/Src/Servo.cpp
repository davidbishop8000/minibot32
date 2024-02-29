/*
 * servo.cpp
 *
 *  Created on: Feb 29, 2024
 *      Author: user
 */

#include "Servo.h"

extern TIM_HandleTypeDef htim3;

Servo::Servo(TIM_HandleTypeDef tim, uint32_t timChannel)
{

}
uint8_t Servo::setAngel(int32_t angel)
{
	_angel = angel;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	TIM4->CCR1 = 25000;
	HAL_Delay(500);
	TIM4->CCR1 = 9;
	HAL_Delay(500);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	return 1;
}
uint32_t Servo::getAngel()
{
	return _angel;
}
uint8_t Servo::enable()
{
	return 1;
}
uint8_t Servo::disable()
{
	return 1;
}


