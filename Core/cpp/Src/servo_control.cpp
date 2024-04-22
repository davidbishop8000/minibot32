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
Servo servo3(&htim3, TIM_CHANNEL_3);
Servo servo4(&htim3, TIM_CHANNEL_4);
Servo *servo[4];

void StartServoTask(void *argument)
{
	servo[0] = &servo1;
	servo[1] = &servo2;
	servo[2] = &servo2;
	servo[3] = &servo3;
	enum MOVE_COMM command;
	for (;;) {
		command = (MOVE_COMM)globData.current_comm;
		if (command == MOVE_SERVO1) {
			servo1.setAngle(contrlMsg.pos_servo1);
			servo2.setAngle(270-contrlMsg.pos_servo1);
			osDelay(800);
			servo1.disable();
			servo2.disable();
			globData.current_comm = MOVE_NONE;
		}
		else if (command == MOVE_SERVO2) {
			servo3.setAngle(contrlMsg.pos_servo2);
			servo4.setAngle(270-contrlMsg.pos_servo2);
			osDelay(800);
			servo3.disable();
			servo4.disable();
			globData.current_comm = MOVE_NONE;
		}
		osDelay(10);
	}
}
