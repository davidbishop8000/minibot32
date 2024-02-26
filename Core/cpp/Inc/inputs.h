/*
 * inputs.hpp
 *
 *  Created on: May 5, 2023
 *      Author: USER
 */

#ifndef CPP_INC_INPUTS_H_
#define CPP_INC_INPUTS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "cmsis_os2.h"

/*
 Input
PB7     X00
PB6     X01
PC13    X02
PE6     X03
PE5     X04
PE4     X05
PE3     X06
PE2     X07
PE1     X08
PB5     X09
PB4     X10
PB3     X11
PD7     X12
PD6     X13
PD5     X14
PD4     X15
PD3     X16
PD2     X17
PD1     X18
PD0     X19
PC12    X20
PC11    X21 //rx
PC10    X22 //tx
PA15    X23
 */

#define ANTI_BOUNCE_TIME_SEARCH_MS 30
#define ANTI_BOUNCE_TIME_QUANT_MS 2
#define ANTI_BOUNCE_TIME_CANAL_MS 3000
#define LIFT_TIMEOUT 5000


#define ENC_ONE_PERIOD 65536
#define ENC_HALF_PERIOD 32768

#define IN_X00 GPIOB->IDR & (1 << 7)
#define IN_X01 GPIOB->IDR & (1 << 6)

#define IN_X02 GPIOC->IDR & (1 << 13)
#define IN_X03 GPIOE->IDR & (1 << 6)
#define IN_X04 GPIOE->IDR & (1 << 5)
#define IN_X05 GPIOE->IDR & (1 << 4)
#define IN_X06 GPIOE->IDR & (1 << 3)
#define IN_X07 GPIOE->IDR & (1 << 2)

#define IN_X08 GPIOE->IDR & (1 << 1)
#define IN_X09 GPIOB->IDR & (1 << 5)
#define IN_X10 GPIOB->IDR & (1 << 4)
#define IN_X11 GPIOB->IDR & (1 << 3)
#define IN_X12 GPIOD->IDR & (1 << 7)
#define IN_X13 GPIOD->IDR & (1 << 6)
#define IN_X14 GPIOD->IDR & (1 << 5)
#define IN_X15 GPIOD->IDR & (1 << 4)

#define IN_X16 GPIOD->IDR & (1 << 3)
#define IN_X17 GPIOD->IDR & (1 << 2)
#define IN_X18 GPIOD->IDR & (1 << 1)
#define IN_X19 GPIOD->IDR & (1 << 0)
#define IN_X20 GPIOC->IDR & (1 << 12)
#define IN_X21 GPIOC->IDR & (1 << 11)
#define IN_X22 GPIOC->IDR & (1 << 10)
#define IN_X23 GPIOA->IDR & (1 << 15)

void StartInputsTask(void *argument);

void InputsRead();

#ifdef __cplusplus
}
#endif

#endif /* CPP_INC_INPUTS_H_ */
