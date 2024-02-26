/*
 * stepper_control.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#include "stepper_control.h"
#include "accelStepper.h"
extern GlobDataTypeDef globData;
extern TermoplastConfigTypeDef termoplastConfig;

void StartSteppersTask(void *argument)
{
	AccelStepper motor1(GPIOC, GPIO_PIN_9, GPIOD, GPIO_PIN_15, GPIOC, GPIO_PIN_6);
	AccelStepper motor2(GPIOC, GPIO_PIN_8, GPIOD, GPIO_PIN_14, GPIOD, GPIO_PIN_12);
	motor1.setMinPulseWidth(6); //6
	motor1.setMaxSpeed(1000.0);
	motor1.setAcceleration(500);
	//motor1.setSpeed(1000.0);

	motor2.setMinPulseWidth(6);
	motor2.setMaxSpeed(1000.0);
	motor2.setAcceleration(500);
	motor2.setSpeed(1000.0);
	DWT_Init();
	long pos = 1000; //globData.volume
	int state = 0;
	globData.cycles_set = 5;
	int manual_mode_trig = 0;
	motor1_dis();
	motor_press_dis();
	for(;;)
	{
		if (globData.temp1 > termoplastConfig.temp1) globData.heating_ok = 1;
		else if ((globData.temp1 < termoplastConfig.temp1 - 5) || globData.temp1 > 500 ) globData.heating_ok = 0;
		if (state == 0)
		{
			if (globData.heating_ok && globData.current_status == JOB_START)
			{
				if (globData.cycles_count < globData.cycles_set)
				{
					state++;
				}
			}
		}
		else if (state == 1)
		{
			motor1.moveTo(pos);
			state++;
		}
		else if (state == 2)
		{
			while(motor1.run());
			state++;
		}
		else if (state == 3)
		{
			osDelay(500);
			state++;
		}
		else if (state == 4)
		{
			motor1.moveTo(-pos);
			state++;
		}
		else if (state == 5)
		{
			while(motor1.run());
			state++;
		}
		else if (state == 6)
		{
			globData.cycles_count++;
			if (globData.cycles_count >= globData.cycles_set)
			{
				globData.current_status = JOB_COMPLETED;
				globData.heat_on = 0;
			}
			osDelay(1000);
			state = 0;
		}

		//motor_press_en();
		//motor2.setSpeed(1000.0); //-1000.0
		//motor2.runSpeed();
		//
		//motor_press_dis();
		if (globData.sens.button_manual_mode) {
			manual_mode_trig = 1;
			if (globData.current_move_comm == MOVE_MOTOR1_FORW) {
				motor1_en();
				motor1.setSpeed(1000.0);
				motor1.runSpeed();
			}
			else if (globData.current_move_comm == MOVE_MOTOR1_BACK) {
				motor1_en();
				motor1.setSpeed(-1000.0);
				motor1.runSpeed();
			}
			else if (globData.current_move_comm == MOVE_MOTOR2_FORW) {
				motor_press_en();
				motor2.setSpeed(1000.0);
				motor2.runSpeed();
			}
			else if (globData.current_move_comm == MOVE_MOTOR2_BACK) {
				motor_press_en();
				motor2.setSpeed(-1000.0);
				motor2.runSpeed();
			}
			else {
				motor1_dis();
				motor_press_dis();
			}
			osDelay(1);
		}
		else if (manual_mode_trig) {
			manual_mode_trig = 0;
			motor1_dis();
			motor_press_dis();
		}
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


