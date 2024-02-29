/*
 * drivers_control.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: user
 */

#include <KeyaLKTechDriver.h>
#include "drivers_control.h"

extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;

void StartCanDriversTask(void *argument)
{
	KeyaLKTechDriver DriverFB(0x140 + DRIVER1_LKTECH_ID);
	KeyaLKTechDriver DriverLR(0x140 + DRIVER2_LKTECH_ID);

	for(;;)
	{
		int32_t speed = 30000;
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
		osDelay(3000);

	}
	osDelay(1);
}
