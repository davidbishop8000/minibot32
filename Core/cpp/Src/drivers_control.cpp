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

	DriversObjectInit(mdrivers, DRIVERS_QUANT);
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

		osDelay(1);
	}
}
