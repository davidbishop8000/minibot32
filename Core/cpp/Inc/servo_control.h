/*
 * servo_control.h
 *
 *  Created on: March 5, 2024
 *      Author: USER
 */

#ifndef CPP_INC_SERVO_CONTROL_H_
#define CPP_INC_SERVO_CONTROL_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
#include "minibot_config.h"

void StartServoTask(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* CPP_INC_SERVO_CONTROL_H_ */
