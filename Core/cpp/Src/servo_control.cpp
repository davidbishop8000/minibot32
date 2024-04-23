/*
 * servo_control.cpp
 *
 *  Created on: March 5, 2024
 *      Author: USER
 */

#include "servo_control.h"
#include "Servo.h"
#include "stdio.h"

extern TIM_HandleTypeDef htim3;
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;
extern ContrlMsgTypeDef contrlMsg;
Servo servo1(&htim3, TIM_CHANNEL_1);
Servo servo2(&htim3, TIM_CHANNEL_2);
Servo *servo[2];

void StartServoTask(void *argument)
{
	servo[0] = &servo1;
	servo[1] = &servo2;
	enum MOVE_COMM command;
	for (;;) {
		command = (MOVE_COMM)globData.current_comm;
		if (command == MOVE_SERVO) {
			if ((SERVO_POS)contrlMsg.pos_servo == SERVO_OPEN)
			{
				servo1.setAngle(0);
				servo2.setAngle(180);
			}
			else if ((SERVO_POS)contrlMsg.pos_servo == SERVO_CLOSE)
			{
				servo1.setAngle(180);
				servo2.setAngle(0);
			}
			osDelay(800);
			servo1.disable();
			servo2.disable();
			globData.current_comm = MOVE_NONE;
		}
		osDelay(10);
	}
}
