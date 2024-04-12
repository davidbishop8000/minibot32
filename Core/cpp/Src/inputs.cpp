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
int32_t enc_prev = 0;

void StartInputsTask(void *argument)
{
	for(;;)
	{
		static int32_t currCounter = 0;
		currCounter = __HAL_TIM_GET_COUNTER(&htim4);
		static int32_t enc_idle_tick = 0;
		enc_idle_tick = unwrap_encoder(currCounter, &enc_prev);
		globData.enc_idle = enc_idle_tick*Y_WHEEL_RATIO;
		IN_X02 ? (globData.sens.limit_sw1 = 0) : (globData.sens.limit_sw1 = 1);
		IN_X03 ? (globData.sens.limit_sw2 = 0) : (globData.sens.limit_sw2 = 1);
		IN_X04 ? (globData.sens.limit_platform_up = 0) : (globData.sens.limit_platform_up = 1);
		IN_X05 ? (globData.sens.limit_platform_down = 0) : (globData.sens.limit_platform_down= 1);
		IN_X06 ? (globData.sens.limit_servo_open= 0) : (globData.sens.limit_servo_open = 1);
		IN_X07 ? (globData.sens.limit_servo_close = 0) : (globData.sens.limit_servo_close = 1);
		IN_X08 ? (globData.sens.lim0 = 0) : (globData.sens.lim0 = 1);
		IN_X09 ? (globData.sens.lim1 = 0) : (globData.sens.lim1 = 1);
		IN_X10 ? (globData.sens.lim2 = 0) : (globData.sens.lim2 = 1);
		IN_X11 ? (globData.sens.lim3 = 0) : (globData.sens.lim3 = 1);
		IN_X12 ? (globData.sens.lim4 = 0) : (globData.sens.lim4 = 1);
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
