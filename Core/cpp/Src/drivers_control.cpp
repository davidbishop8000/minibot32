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

extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;
extern ContrlMsgTypeDef contrlMsg;

KeyaLKTechDriver driverX1(0x140 + DRIVER1_LKTECH_ID, globData);
KeyaLKTechDriver driverX2(0x140 + DRIVER2_LKTECH_ID, globData);
KeyaLKTechDriver driverY1(DRIVER_KEYA_ID + 0x600, 0x01, globData);
KeyaLKTechDriver driverY2(DRIVER_KEYA_ID + 0x600, 0x02, globData);
KeyaLKTechDriver *mdrivers[DRIVERS_QUANT];

void StartCanDriversTask(void *argument)
{
	driversInit();
	enum MOVE_COMM command;
	for(;;)
	{
		driverX1.readEnc();
		osDelay(2);
		driverX2.readEnc();
		osDelay(2);
		driverY1.setEnc(globData.enc_idle);
		command = (MOVE_COMM)contrlMsg.comm;
		if (command != MOVE_NONE)
		{
			if (command == MOVE_POS_X)
			{
				driverX1.setPos(contrlMsg.pos_x);
				if (driverX1.getSpeed() == 0)
				{
					driverX2.stop();
				}
				else
				{
					driverX2.setSpeed(-driverX1.getSpeed());
				}
				//DriverFB.setSpeed(contrlMsg.pos_fb);
			}
			else if (command == MOVE_POS_Y)
			{
				//DriverY.setPos(contrlMsg.pos_y);
			}
			if (command == MOVE_EMERGY_STOP)
			{
				driversStop();
			}
		}
		osDelay(1);
	}
}

void driversInit()
{
	mdrivers[0] = &driverX1;
	mdrivers[1] = &driverX2;
	mdrivers[2] = &driverY1;
	mdrivers[3] = &driverY2;
	osDelay(1000);
	driverX1.resetError();
	osDelay(5);
	driverX2.resetError();
	osDelay(5);
	driverX1.enable();
	osDelay(5);
	driverX2.enable();
	osDelay(5);
}

void driversStop() {
	osDelay(2);
	driverX1.stop();
	osDelay(2);
	driverX2.stop();
	osDelay(2);
	driverY1.stop();
	osDelay(2);
}
