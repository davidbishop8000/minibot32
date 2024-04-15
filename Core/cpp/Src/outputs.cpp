/*
 * outputs.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */


#include "outputs.h"
#include "minibot_config.h"


extern IWDG_HandleTypeDef hiwdg;
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;
extern ContrlMsgTypeDef contrlMsg;

//extern IWDG_HandleTypeDef hiwdg;


void StartOutputsTask(void *argument)
{
	for(;;)
	{
		HAL_IWDG_Refresh(&hiwdg);
		SetOutputs();
		if (globData.LEDB) LedBlink();
		osDelay(5);
	}
}

void SetOutputs()
{
	if (contrlMsg.comm == MOVE_LIFT)
	{
		if (contrlMsg.pos_lift == 1 && globData.sens.limit_platform_up == 0)
		{
			Y01_ON;
			Y02_OFF;
		}
		else
		{
			Y01_OFF;
			contrlMsg.comm = 0;
		}
		if (contrlMsg.pos_lift == 2 && globData.sens.limit_platform_down == 0)
		{
			Y01_OFF;
			Y02_ON;
		}
		else
		{
			Y02_OFF;
			contrlMsg.comm = 0;
		}
	}
	else if (!*(uint16_t*)&globData.error) {
		Y21_ON;
		osDelay(100);
		Y22_ON;
		osDelay(20);
		Y21_OFF;
		osDelay(100);
		Y23_ON;
		osDelay(20);
		Y22_OFF;
		osDelay(100);
		Y23_OFF;
		osDelay(300);
	}
	else
	{
		Y16_ON;
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
		osDelay(500);
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
