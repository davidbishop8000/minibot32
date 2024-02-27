/*
 * inputs.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#include "inputs.h"
#include "minibot_config.h"
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;

void StartInputsTask(void *argument)
{
	for(;;)
	{
		IN_X02 ? (globData.sens.limit_sw1 = 0) : (globData.sens.limit_sw1 = 1);
		IN_X03 ? (globData.sens.limit_sw2 = 0) : (globData.sens.limit_sw2 = 1);
		IN_X04 ? (globData.sens.button_manual_mode = 0) : (globData.sens.button_manual_mode = 1);
		IN_X05 ? (globData.sens.button_motor1_forw = 0) : (globData.sens.button_motor1_forw= 1);
		IN_X06 ? (globData.sens.button_motor1_back= 0) : (globData.sens.button_motor1_back = 1);
		IN_X07 ? (globData.sens.button_motor2_forw = 0) : (globData.sens.button_motor2_forw = 1);
		IN_X08 ? (globData.sens.button_motor2_back = 0) : (globData.sens.button_motor2_back = 1);
		IN_X09 ? (globData.sens.button_start = 0) : (globData.sens.button_start = 1);
		IN_X10 ? (globData.sens.button_pause = 0) : (globData.sens.button_pause = 1);
		IN_X11 ? (globData.sens.button_stop = 0) : (globData.sens.button_stop = 1);
		IN_X12 ? (globData.sens.button8 = 0) : (globData.sens.button8 = 1);
		osDelay(1);
	}
}
