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
KeyaLKTechDriver driverY1(DRIVER_KEYA_ID + 0x06000000, 0x01, globData);
KeyaLKTechDriver driverY2(DRIVER_KEYA_ID + 0x06000000, 0x02, globData);
KeyaLKTechDriver *mdrivers[DRIVERS_QUANT];

void StartCanDriversTask(void *argument)
{
	mdrivers[0] = &driverX1;
	mdrivers[1] = &driverX2;
	mdrivers[2] = &driverY1;
	mdrivers[3] = &driverY2;
	driversInit();
	uint32_t err_check_timer = 0;
	enum MOVE_COMM command;
	for(;;)
	{
		driverX1.readEnc();
		osDelay(2);
		driverX2.readEnc();
		osDelay(2);
		driverY1.readEnc();
		osDelay(2);
		driverY1.setEnc(globData.enc_idle);
		command = (MOVE_COMM)globData.current_comm;
		if (command != MOVE_NONE)
		{
			if (command == MOVE_POS_X)
			{
				driverX1.setPos(contrlMsg.pos_x);
				if (driverX1.getSpeed() == 0)
				{
					driverX2.stop();
					globData.current_comm = MOVE_NONE;
				}
				else
				{
					driverX2.setSpeed(-driverX1.getSpeed());
				}
			}
			else if (command == MOVE_POS_Y)
			{
				driverY1.setPos(contrlMsg.pos_y);
				if (driverY1.getSpeed() == 0)
				{
					globData.current_comm = MOVE_NONE;
				}
			}
			else if (command == MOVE_POS_FORK)
			{
				driverY2.setPos(contrlMsg.pos_fork);
				if (driverY2.getSpeed() == 0)
				{
					globData.current_comm = MOVE_NONE;
				}
			}
			if (command == MOVE_EMERGY_STOP)
			{
				driversStop();
			}
			if (command == MOVE_RESET)
			{
				driversStop();
				driversInit();
				globData.current_comm = MOVE_NONE;
			}
		}
		else
		{
			driverX1.readError();
			osDelay(2);
			driverX2.readError();
			osDelay(2);
			if (driverX1.getError())
			{
				driverX1.resetError();
				osDelay(2);
			}
			if (driverX2.getError())
			{
				driverX2.resetError();
				osDelay(2);
			}
			driversStop();
		}
		if (HAL_GetTick() - err_check_timer > 1000) {
			driverX1.error_count++;
			if (driverX1.error_count > 3) globData.error.driverX_err = 1;
			else globData.error.driverX_err = 0;
			driverY1.error_count++;
			if (driverY1.error_count > 3)
			{
				globData.error.driverY_err = 1;
				globData.error.driverF_err = 1;
			}
			else
			{
				globData.error.driverY_err = 0;
				globData.error.driverF_err = 0;
			}
			err_check_timer = HAL_GetTick();
		}
		osDelay(1);
	}
}

void driversInit()
{
	osDelay(5);
	driverX1.resetError();
	osDelay(5);
	driverX2.resetError();
	osDelay(5);
	driverX1.enable();
	osDelay(5);
	driverX2.enable();
	osDelay(5);
	driverY1.enable();
	osDelay(5);
}

void driversStop() {
	osDelay(2);
	driverX1.stop();
	osDelay(2);
	driverX2.stop();
	osDelay(2);
	driverY1.stop();
	osDelay(5);
}
