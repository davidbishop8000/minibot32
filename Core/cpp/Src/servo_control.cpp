/*
 * servo_control.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#include "servo_control.h"
#include "stdio.h"
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;

void StartServoTask(void *argument)
{
	
	for(;;)
	{
		osDelay(1000);
	}
}
