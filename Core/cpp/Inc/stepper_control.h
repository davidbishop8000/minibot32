/*
 * stepper_control.h
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#ifndef CPP_INC_STEPPER_CONTROL_H_
#define CPP_INC_STEPPER_CONTROL_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
#include "termoplast_config.h"

void StartSteppersTask(void *argument);

void motor1_en();
void motor1_dis();
void motor_press_en();
void motor_press_dis();

#ifdef __cplusplus
}
#endif

#endif /* CPP_INC_STEPPER_CONTROL_H_ */
