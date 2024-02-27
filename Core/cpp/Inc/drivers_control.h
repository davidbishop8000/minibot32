/*
 * drivers_control.h
 *
 *  Created on: Feb 26, 2024
 *      Author: user
 */

#ifndef CPP_INC_DRIVERS_CONTROL_H_
#define CPP_INC_DRIVERS_CONTROL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
#include "minibot_config.h"
#include "canbus.h"

#define COLLISION_MAX_TIME 1200
#define COLLISION_MAX_DIMM 3

void StartCanDriversTask(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* CPP_INC_DRIVERS_CONTROL_H_ */
