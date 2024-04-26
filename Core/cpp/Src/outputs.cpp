/*
 * outputs.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */


#include "outputs.h"
#include "minibot_config.h"

extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;
extern ContrlMsgTypeDef contrlMsg;

//extern IWDG_HandleTypeDef hiwdg;

uint32_t lift_check_timer = 0;
uint32_t curr_lift_move = 0;

uint32_t blink_timer = 0;
uint32_t blink_interval = 300;

void StartOutputsTask(void *argument)
{
	for(;;)
	{
		SetOutputs();
		if (globData.LEDB) LedBlink();
		osDelay(5);
	}
}

void SetOutputs()
{
	static uint8_t check_timer_en = 0;
	if (globData.current_comm == MOVE_LIFT && !globData.error.driverL_err)
	{
		if (contrlMsg.pos_lift == LIFT_UP)
		{
			if (globData.sens.limit_platform_up == 0)
			{
				Y01_ON;
				Y02_OFF;
				curr_lift_move = LIFT_UP;
				if (!check_timer_en)
				{
					lift_check_timer = HAL_GetTick();
					check_timer_en = 1;
				}
			}
		}
		else if (contrlMsg.pos_lift == LIFT_DOWN)
		{
			if (globData.sens.limit_platform_down == 0)
			{
				Y01_ON;
				Y02_ON;
				curr_lift_move = LIFT_DOWN;
				if (!check_timer_en)
				{
					lift_check_timer = HAL_GetTick();
					check_timer_en = 1;
				}
			}
		}
		if (HAL_GetTick() - lift_check_timer > 8000) {

			globData.error.driverL_err = 1;
			globData.current_comm = MOVE_NONE;
			Y01_OFF;
			Y02_OFF;
			curr_lift_move = LIFT_NONE;
			check_timer_en = 0;
		}
	}
	else
	{
		Y01_OFF;
		Y02_OFF;
		check_timer_en = 0;
		/*Y16_ON;
		osDelay(100);
		Y17_ON;
		osDelay(20);
		Y16_OFF;
		osDelay(100);
		Y18_ON;
		osDelay(20);
		Y17_OFF;
		osDelay(100);
		Y18_OFF;
		osDelay(30);
		for (int32_t i=20; i>0; i--)
		{
			Y19_OFF;
			Y20_ON;
			osDelay(2*i);
			Y19_ON;
			Y20_OFF;
			osDelay(2*i);
		}
		Y19_OFF;
		osDelay(500);*/
	}
	if (globData.sens.limit_platform_up){
		globData.lift_status = LIFT_UP;
		if (curr_lift_move == LIFT_UP)
		{
			Y01_OFF;
			Y02_OFF;
			check_timer_en = 0;
			curr_lift_move = LIFT_NONE;
			contrlMsg.pos_lift = LIFT_NONE;
			globData.current_comm = MOVE_NONE;
		}
	}
	if (globData.sens.limit_platform_down){
		globData.lift_status = LIFT_DOWN;
		if (curr_lift_move == LIFT_DOWN)
		{
			Y01_OFF;
			Y02_OFF;
			check_timer_en = 0;
			curr_lift_move = LIFT_NONE;
			contrlMsg.pos_lift = LIFT_NONE;
			globData.current_comm = MOVE_NONE;
		}
	}
	if (HAL_GetTick() - blink_timer > blink_interval)
	{
		static uint32_t x = 0;
		x = !x;
		if (!*(uint16_t*)&globData.error) {
			Y09_OFF;
			if (x){
				Y08_ON;
				blink_interval = 100;
			}
			else {
				Y08_OFF;
				blink_interval = 800;
			}
		}
		else
		{
			Y08_OFF;
			if (x) Y09_ON;
			else Y09_OFF;
			blink_interval-= 10;
			if (blink_interval < 10) blink_interval = 300;
		}
		blink_timer = HAL_GetTick();
	}
}

void LedBlink() {
	static const uint32_t ledb_time[] = {
			0,
			20,  //LEDB_POWER_ON
			200, //LEDB_START
			100, //LEDB_COMPLETED
			10,  //LEDB_FLASH_OK
			200   //LEDB_ERROR
	};
	static const uint32_t ledb_rep[] = {
			0,
			1, //LEDB_POWER_ON
			1, //LEDB_START
			1, //LEDB_COMPLETED
			3,  //LEDB_FLASH_OK
			3   //LEDB_ERROR
	};

	static int32_t ledb_active_time = 0;
	static int32_t ledb_repeats = 0;
	ledb_repeats = ledb_rep[globData.LEDB];
	ledb_active_time = ledb_time[globData.LEDB];
	for (int i = 0; i < ledb_repeats; i++)
	{
		osDelay(ledb_active_time);
		Y05_ON;
		osDelay(ledb_active_time);
		Y05_OFF;
	}
	globData.LEDB = 0;
}
