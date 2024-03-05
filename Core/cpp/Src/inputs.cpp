/*
 * inputs.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#include "inputs.h"
#include "minibot_config.h"

extern TIM_HandleTypeDef htim4;
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

int32_t unwrap_encoder(uint16_t in, int32_t *prev)
{
    int32_t c32 = (int32_t)in - ENC_HALF_PERIOD;
    int32_t dif = (c32-*prev);

    int32_t mod_dif = ((dif + ENC_HALF_PERIOD) % ENC_ONE_PERIOD) - ENC_HALF_PERIOD;
    if(dif < -ENC_HALF_PERIOD) {
        mod_dif += ENC_ONE_PERIOD;
    }
    int32_t unwrapped = *prev + mod_dif;
    *prev = unwrapped;

    return unwrapped + ENC_HALF_PERIOD;
}
