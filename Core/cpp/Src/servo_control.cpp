/*
 * stepper_control.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#include "servo_control.h"
#include "stdio.h"
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;

void StartServoTask(void *argument)
{
	
	for(;;)
	{
		osDelay(3000);
	}
}

void motor1_en()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
}

void motor1_dis()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
}

void motor_press_en()
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
}

void motor_press_dis()
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}


