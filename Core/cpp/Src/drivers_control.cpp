/*
 * drivers_control.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: user
 */

#include <KeyaLKTechDriver.h>
#include "Servo.h"
#include "drivers_control.h"
#include "canbus.h"
#include "InitCanDrivers.h"

extern TIM_HandleTypeDef htim3;
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;
extern ContrlMsgTypeDef contrlMsg;

KeyaLKTechDriver DriverFB(0x140 + DRIVER1_LKTECH_ID);
KeyaLKTechDriver DriverLR(0x140 + DRIVER2_LKTECH_ID);
KeyaLKTechDriver *mdrivers[DRIVERS_QUANT];

void StartCanDriversTask(void *argument)
{
	mdrivers[0] = &DriverFB;
	mdrivers[1] = &DriverLR;
	mdrivers[2] = &DriverFB;
	mdrivers[3] = &DriverFB;

	Servo servo1(&htim3, TIM_CHANNEL_1);

	//DriversObjectInit(mdrivers, DRIVERS_QUANT);
	osDelay(1000);
	//DriverFB.disable();
	//DriverLR.disable();
	enum MOVE_COMM command;
	for(;;)
	{
		/*int32_t speed = 30000;
		DriverFB.setSpeed(-speed);
		osDelay(3000);
		speed = 90000;
		DriverFB.setSpeed(speed);
		DriverLR.stop();
		DriverLR.disable();
		osDelay(3000);
		DriverFB.stop();
		DriverLR.enable();
		osDelay(3000);
		DriverFB.disable();
		DriverLR.setSpeed(speed);
		osDelay(3000);
		DriverFB.enable();
		servo1.setAngle(0);
		osDelay(1000);
		servo1.setAngle(90);
		osDelay(1000);
		servo1.setAngle(180);
		osDelay(1000);
		servo1.setAngle(270);
		osDelay(5000);*/


		/*
		DriverFB.setSpeed(3000);
		osDelay(500);
		DriverFB.stop();
		osDelay(50);
		DriverFB.readEnc();
		osDelay(500);
		globData.LKEncoder = DriverFB.getPos();
		globData.LKTemp = DriverFB.getTemp();
		DriverFB.setSpeed(3000);
		osDelay(500);
		DriverFB.stop();
		osDelay(50);
		DriverFB.readEnc();
		osDelay(500);
		globData.LKEncoder = DriverFB.getPos();
		globData.LKTemp = DriverFB.getTemp();
		*/
		//DriverFB.setPos(20000);
		//DriverFB.setSpeed(-3000);
		//osDelay(5000);
		//DriverFB.stop();
		DriverFB.readEnc();
		command = (MOVE_COMM)contrlMsg.comm;
		if (command != MOVE_NONE)
		{
			if (command == MOVE_POS_FB)
			{
				DriverFB.setPos(contrlMsg.pos_fb);
				if (DriverFB.getSpeed() == 0)
				{
					DriverLR.stop();
				}
				else
				{
					DriverLR.setSpeed(-DriverFB.getSpeed());
				}
				//DriverFB.setSpeed(contrlMsg.pos_fb);
			}
			if (command == MOVE_POS_LR)
			{
				DriverLR.setPos(contrlMsg.pos_lr);
			}
			else if (command == MOVE_SERVO)
			{
				if (contrlMsg.pos_servo == 1)
				{
					servo1.setAngle(0);
					contrlMsg.comm = 0;
				}
				else if (contrlMsg.pos_servo == 2)
				{
					servo1.setAngle(180);
					contrlMsg.comm = 0;
				}
			}
		}
		osDelay(1);
	}
}
