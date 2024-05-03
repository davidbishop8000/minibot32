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

extern IWDG_HandleTypeDef hiwdg;
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;
extern ContrlMsgTypeDef contrlMsg;
extern Servo *servo[2];

KeyaLKTechDriver driverX1(0x140 + DRIVER1_LKTECH_ID, globData);
KeyaLKTechDriver driverX2(0x140 + DRIVER2_LKTECH_ID, globData);
KeyaLKTechDriver driverY1(DRIVER_KEYA_ID + 0x06000000, 0x01, globData);
KeyaLKTechDriver driverFork(DRIVER_KEYA_ID + 0x06000000, 0x02, globData);
KeyaLKTechDriver *mdrivers[DRIVERS_QUANT];
int32_t act_state = 0;

void StartCanDriversTask(void *argument)
{
	mdrivers[0] = &driverX1;
	mdrivers[1] = &driverX2;
	mdrivers[2] = &driverY1;
	mdrivers[3] = &driverFork;
	driversInit();
	uint32_t err_check_timer = 0;
	enum MOVE_COMM command;
	int32_t rack_stateX = 0;
	int32_t x_pos_dir = 0;
	int32_t rack_stateY = 0;
	int32_t y_pos_dir = 0;
	int32_t fork_home_state = 0;

	for(;;)
	{
		HAL_IWDG_Refresh(&hiwdg);
		driverX1.readEnc();
		osDelay(2);
		driverX2.readEnc();
		osDelay(2);
		driverY1.readEnc();
		osDelay(2);
		driverY1.setEnc(globData.enc_Y1);
		driverFork.setEnc(globData.enc_fork);
		command = (MOVE_COMM)globData.current_comm;
		if (command == MOVE_POS_X)
		{
			if (RACK_SENS_X == 0)
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
			else
			{
				if (rack_stateX == 0)
				{
					x_pos_dir = driverX1.getPos();
					rack_stateX++;
				}
				else if (rack_stateX == 1)
				{
					driverX1.setPos(contrlMsg.pos_x);
					if (driverX1.getSpeed() == 0)
					{
						driverX2.stop();
						rack_stateX++;
					}
					else
					{
						driverX2.setSpeed(-driverX1.getSpeed());
					}
				}
				else if(rack_stateX == 2)
				{
					if (globData.sens.sw_rackX)
					{
						globData.current_comm = MOVE_NONE;
						rack_stateX = 0;
					}
					else rack_stateX++;
				}
				else if(rack_stateX == 3)
				{
					if ((driverX1.getPos() - x_pos_dir) > 0)
					{
						driverX1.setSpeed(LK_MIN_SPEED);
						driverX2.setSpeed(-LK_MIN_SPEED);
					}
					else
					{
						driverX1.setSpeed(-LK_MIN_SPEED);
						driverX2.setSpeed(LK_MIN_SPEED);
					}
					x_pos_dir = driverX1.getPos();
					rack_stateX++;
				}
				else if(rack_stateX == 4)
				{
					int32_t abs_x_dist = x_pos_dir - driverX1.getPos();
					if (abs_x_dist < 0) {abs_x_dist = -abs_x_dist;}
					if (globData.sens.sw_rackX || (abs_x_dist > POS_X_RACK_DIST))
					{
						globData.current_comm = MOVE_NONE;
						rack_stateX = 0;
					}
				}
			}
		}
		else if (command == MOVE_POS_Y)
		{
			if (RACK_SENS_Y == 0)
			{
				driverY1.setPos(contrlMsg.pos_y);
				if (driverY1.getSpeed() == 0)
				{
					globData.current_comm = MOVE_NONE;
				}
			}
			else
			{
				if (rack_stateY == 0)
				{
					y_pos_dir = driverY1.getPos();
					rack_stateY++;
				}
				else if (rack_stateY == 1)
				{
					driverY1.setPos(contrlMsg.pos_y);
					if (driverY1.getSpeed() == 0)
					{
						rack_stateY++;
					}
				}
				else if(rack_stateY == 2)
				{
					if (globData.sens.sw_rackY)
					{
						globData.current_comm = MOVE_NONE;
						rack_stateY = 0;
					}
					else rack_stateY++;
				}
				else if(rack_stateY == 3)
				{
					if ((driverY1.getPos() - y_pos_dir) > 0)
					{
						driverY1.setSpeed(LK_MIN_SPEED);
					}
					else
					{
						driverY1.setSpeed(-LK_MIN_SPEED);
					}
					y_pos_dir = driverY1.getPos();
					rack_stateY++;
				}
				else if(rack_stateY == 4)
				{
					int32_t abs_y_dist = y_pos_dir - driverY1.getPos();
					if (abs_y_dist < 0) {abs_y_dist = -abs_y_dist;}
					if (globData.sens.sw_rackY || (abs_y_dist > POS_Y_RACK_DIST))
					{
						globData.current_comm = MOVE_NONE;
						rack_stateY = 0;
					}
				}
			}
		}
		else if (command == MOVE_POS_FORK)
		{
			driverFork.setPos(contrlMsg.pos_fork);
			if (driverFork.getSpeed() == 0)
			{
				globData.current_comm = MOVE_NONE;
			}
		}
		else if (command == MOVE_ACTION)
		{
			if (act_state == 0)
			{
				if (servo[0]->getStatus() != SERVO_OPEN && (globData.action_comm == ACTION_GET_BOX_L || globData.action_comm == ACTION_GET_BOX_R))
				{
					contrlMsg.pos_servo = SERVO_OPEN;
				}
				else if (servo[0]->getStatus() != SERVO_CLOSE && (globData.action_comm == ACTION_PUT_BOX_L || globData.action_comm == ACTION_PUT_BOX_R))
				{
					contrlMsg.pos_servo = SERVO_CLOSE;
				}
				else
				{
					act_state++;
				}
			}
			else if (act_state == 1)
			{
				if (globData.action_comm == ACTION_GET_BOX_L || globData.action_comm == ACTION_PUT_BOX_L)
				{
					driverX1.setPos(FORK_LIMIT_POS);
				}
				if (globData.action_comm == ACTION_GET_BOX_R || globData.action_comm == ACTION_PUT_BOX_R)
				{
					driverX1.setPos(-FORK_LIMIT_POS);
				}
				if (driverX1.getSpeed() == 0)
				{
					act_state++;
				}
			}
			else if (act_state == 2)
			{
				if (servo[0]->getStatus() != SERVO_CLOSE && (globData.action_comm == ACTION_GET_BOX_L || globData.action_comm == ACTION_GET_BOX_R))
				{
					contrlMsg.pos_servo = SERVO_CLOSE;
				}
				else if (servo[0]->getStatus() != SERVO_OPEN && (globData.action_comm == ACTION_PUT_BOX_L || globData.action_comm == ACTION_PUT_BOX_R))
				{
					contrlMsg.pos_servo = SERVO_OPEN;
				}
				else
				{
					act_state++;
					contrlMsg.pos_servo = SERVO_NONE;
				}
			}
			else if (act_state == 3)
			{
				driverX1.setPos(0);
				if (driverX1.getSpeed() == 0)
				{
					act_state = 0;
					globData.current_comm = MOVE_NONE;
					globData.action_comm = ACTION_NONE;
				}
			}
		}
		else if (command == MOVE_HOME_X)
		{
			driverX1.setSpeed(-LK_MIN_SPEED);
			if (driverX1.getSpeed() == 0)
			{
				driverX2.stop();
				globData.current_comm = MOVE_NONE;
			}
			else
			{
				driverX2.setSpeed(driverX1.getSpeed());
			}
			if(globData.sens.limit_sw1)
			{
				driverX1.stop();
				driverX2.stop();
				globData.current_comm = MOVE_NONE;
			}
		}
		else if (command == MOVE_HOME_Y)
		{
			driverY1.setSpeed(-KEYA_MIN_SPEED);
			if(globData.sens.limit_sw2)
			{
				driverY1.stop();
				globData.current_comm = MOVE_NONE;
			}
		}
		else if (command == MOVE_HOME_F)
		{
			if (fork_home_state == 0)
			{
				driverFork.setSpeed(FORK_MIN_SPEED);
				if (globData.sens.limit_fork_forw)
				{
					driverFork.stop();
				}
				fork_home_state++;
			}
			else if (fork_home_state == 1)
			{
				driverFork.setSpeed(-FORK_MIN_SPEED);
				if (globData.sens.limit_fork_center)
				{
					driverFork.stop();
					globData.current_comm = MOVE_NONE;
				}
			}
		}
		else
		{
			x_pos_dir = 0;
			y_pos_dir = 0;
			fork_home_state = 0;
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
		if (command == MOVE_EMERGY_STOP)
		{
			driversStop();
		}
		if (command == MOVE_RESET)
		{
			x_pos_dir = 0;
			y_pos_dir = 0;
			fork_home_state = 0;
			driversStop();
			driversInit();
			globData.current_comm = MOVE_NONE;
			globData.action_comm = ACTION_NONE;
			act_state = 0;
		}
		if (HAL_GetTick() - err_check_timer > 1000) {
			driverX1.error_count++;
			if (driverX1.error_count > 3)
			{
				globData.error.driverX_err = 1;
				driverY1.disable();
			}
			else
			{
				globData.error.driverX_err = 0;
			}
			driverY1.error_count++;
			if (driverY1.error_count > 3)
			{
				globData.error.driverY_err = 1;
				globData.error.driverF_err = 1;
				driverY1.disable();
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
