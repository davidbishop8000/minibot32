/*
 * inputs.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#include <inputs.h>
#include "termoplast_config.h"
extern GlobDataTypeDef globData;
extern TermoplastConfigTypeDef termoplastConfig;

void StartInputsTask(void *argument)
{
	for(;;)
	{
		InputsRead();
		osDelay(1);
	}
}

void InputsRead() {
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

	static int button_trig = 0;
	if (globData.sens.button_manual_mode)
	{
		if (globData.sens.button_motor1_forw) {
			globData.current_move_comm = MOVE_MOTOR1_FORW;
		}
		else if (globData.sens.button_motor1_back) {
			globData.current_move_comm = MOVE_MOTOR1_BACK;
		}
		else if (globData.sens.button_motor2_forw) {
			globData.current_move_comm = MOVE_MOTOR2_FORW;
		}
		else if (globData.sens.button_motor2_back) {
			globData.current_move_comm = MOVE_MOTOR2_BACK;
		}
		else {
			globData.current_move_comm = MOVE_STOP;
		}
	}
	else
	{
		if (globData.sens.button_motor2_forw && button_trig == 0)
		{
			button_trig = 1;
			if (globData.cycles_set < 500)
			{
				globData.cycles_set++;
			}
			osDelay(5);
		}
		else if (globData.sens.button_motor2_back && button_trig == 0)
		{
			button_trig = 1;
			if (globData.cycles_set > 0)
			{
				globData.cycles_set--;
			}
			osDelay(5);
		}
		else if (globData.sens.button_motor1_forw && button_trig == 0)
		{
			button_trig = 1;
			globData.current_status = JOB_PAUSE;
			osDelay(5);
		}
		else if (globData.sens.button_motor1_back && button_trig == 0)
		{
			button_trig = 1;
			globData.volume = termoplastConfig.volume;
			globData.time_hold = termoplastConfig.time_hold;
			globData.heat_on = 1;
			globData.current_status = JOB_START;
			osDelay(5);
		}
		else if (globData.sens.button_motor2_forw == 0 && globData.sens.button_motor2_back == 0 && globData.sens.button_motor1_forw == 0 && globData.sens.button_motor1_back == 0)
		{
			button_trig = 0;
		}
	}
}
