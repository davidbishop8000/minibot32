/*
 * inputs.cpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#include "inputs.h"
#include "minibot_config.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;
int32_t enc_prev0 = 0;
int32_t enc_prev1 = 0;

void StartInputsTask(void *argument)
{
	for(;;)
	{
		static int32_t currCounter0 = 0;
		static int32_t currCounter1 = 0;
		currCounter0 = __HAL_TIM_GET_COUNTER(&htim4);
		currCounter1 = __HAL_TIM_GET_COUNTER(&htim2);
		static int32_t enc_Y1 = 0;
		enc_Y1 = unwrap_encoder(currCounter0, &enc_prev0);
		globData.enc_Y1 = enc_Y1*Y_WHEEL_RATIO;
		globData.enc_fork = unwrap_encoder(currCounter1, &enc_prev1);
		IN_X02 ? (globData.sens.limit_sw1 = 0) : (globData.sens.limit_sw1 = 1);
		IN_X03 ? (globData.sens.limit_sw2 = 0) : (globData.sens.limit_sw2 = 1);
		IN_X04 ? (globData.sens.limit_platform_up = 0) : (globData.sens.limit_platform_up = 1);
		IN_X05 ? (globData.sens.limit_platform_down = 0) : (globData.sens.limit_platform_down= 1);
		IN_X06 ? (globData.sens.limit_servo_open= 0) : (globData.sens.limit_servo_open = 1);
		IN_X07 ? (globData.sens.limit_servo_close = 0) : (globData.sens.limit_servo_close = 1);
		IN_X08 ? (globData.sens.limit_fork_center = 0) : (globData.sens.limit_fork_center = 1);
		IN_X09 ? (globData.sens.limit_fork_forw = 0) : (globData.sens.limit_fork_forw = 1);
		IN_X10 ? (globData.sens.sw_box = 0) : (globData.sens.sw_box = 1);
		IN_X12 ? (globData.sens.sw_rackX = 0) : (globData.sens.sw_rackX = 1);
		IN_X13 ? (globData.sens.sw_rackY = 0) : (globData.sens.sw_rackY = 1);
		IN_X14 ? (globData.sens.r1 = 0) : (globData.sens.r1 = 1);
		IN_X15 ? (globData.sens.r2 = 0) : (globData.sens.r2 = 1);
		IN_X16 ? (globData.sens.r3 = 0) : (globData.sens.r3 = 1);
		IN_X17 ? (globData.sens.r4 = 0) : (globData.sens.r4 = 1);
		if (globData.sens.limit_sw2)
		{
			__HAL_TIM_SET_COUNTER(&htim4, 0);
			enc_prev0 = 0;
		}
		if (globData.sens.limit_fork_center)
		{
			__HAL_TIM_SET_COUNTER(&htim2, 0);
			enc_prev1 = 0;
		}
		osDelay(1);
	}
}

int32_t unwrap_encoder(uint16_t in, int32_t *prev)
{
    int32_t c32 = (int32_t)in - ENC0_HALF_PERIOD;
    int32_t dif = (c32-*prev);

    int32_t mod_dif = ((dif + ENC0_HALF_PERIOD) % ENC0_ONE_PERIOD) - ENC0_HALF_PERIOD;
    if(dif < -ENC0_HALF_PERIOD) {
        mod_dif += ENC0_ONE_PERIOD;
    }
    int32_t unwrapped = *prev + mod_dif;
    *prev = unwrapped;

    return unwrapped + ENC0_HALF_PERIOD;
}
