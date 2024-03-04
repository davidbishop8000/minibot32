/*
 * drivers_control.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: user
 */

#include <KeyaLKTechDriver.h>
#include "Servo.h"
#include "drivers_control.h"

extern TIM_HandleTypeDef htim3;
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;

void StartCanDriversTask(void *argument)
{
	Servo servo1(&htim3, TIM_CHANNEL_1);

	KeyaLKTechDriver DriverFB(0x140 + DRIVER1_LKTECH_ID);
	KeyaLKTechDriver DriverLR(0x140 + DRIVER2_LKTECH_ID);
	osDelay(1000);
	//DriverFB.disable();
	//DriverLR.disable();
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
		DriverFB.setSpeed(3000);
		osDelay(500);
		DriverFB.stop();
		osDelay(50);
		DriverFB.getEnc();
		osDelay(500);

		osDelay(1);
	}
}
