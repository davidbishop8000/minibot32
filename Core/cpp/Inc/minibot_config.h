/*
 * term_config.h
 *
 *  Created on: May 10, 2023
 *      Author: USER
 */

#ifndef CPP_INC_TERMOPLAST_CONFIG_H_
#define CPP_INC_TERMOPLAST_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

#define DEBUG_MODE 0

#define WIFI_UART huart1
#define WIFI_UART_DMA hdma_usart1_rx
#define WIFI_UART_Ins USART1

#define BMS_UART huart2
#define BMS_UART_DMA hdma_usart2_rx
#define BMS_UART_Ins USART2

#define RC_UART huart3
#define RC_UART_DMA  hdma_usart3_rx
#define RC_UART_Ins USART3

#define DEBUG_UART RC_UART
#define DEBUG_UART_DMA RC_UART_DMA
#define DEBUG_UART_Ins RC_UART_Ins

#define DRIVER_LKTECH_INVERS 0
#define DRIVER_KEYA_INVERS 0

#define DRIVER1_LKTECH_ID 0x01
#define DRIVER2_LKTECH_ID 0x02
#define DRIVER3_LKTECH_ID 0x03
#define DRIVER4_LKTECH_ID 0x04

#define DRIVER_KEYA_ID 0x05

#define DRIVERS_QUANT 4

#define POS_TOLERANCE 100
#define LK_MAX_SPEED 10000

#define FLASH_INIT 0x44

////////Control//////////
#define START_MSG0 0x84
#define START_MSG1 0x53
#define NET_CONTROL_ID 0x64

#define MAX_TEMP 200.0

typedef struct
{
	unsigned char ind : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
} BitParasmsTypeDef;

typedef struct {
	float volume_per_rev;
	uint32_t motor1_speed;
	uint32_t motor1_acc;
	uint32_t motor2_speed;
	uint32_t motor2_acc;
	uint32_t time_hold;
	float volume;
	float temp1;
	float temp2;
	float temp3;
	float Kp;
	float Ki;
	float Kd;
	BitParasmsTypeDef bitParams;
	uint8_t reservU0;
	uint8_t flash_init;
	uint8_t CS;
} MinibotConfigTypeDef;

typedef struct {
	uint8_t start_msg0;
	uint8_t start_msg1;
	uint8_t control_id;
	uint8_t msg_id;
	MinibotConfigTypeDef termConfig;
	uint8_t reservU0;
	uint8_t reservU1;
	uint8_t reservU2;
	uint8_t CS;
} StmConfigTypeDef;

typedef struct {
	unsigned char limit_sw1				:1;
	unsigned char limit_sw2				:1;
	unsigned char button_manual_mode	:1;
	unsigned char button_motor1_forw	:1;
	unsigned char button_motor1_back	:1;
	unsigned char button_motor2_forw	:1;
	unsigned char button_motor2_back	:1;
	unsigned char button_start			:1;
	unsigned char button_pause			:1;
	unsigned char button_stop			:1;
	unsigned char button8				:1;
	unsigned char r0					:1;
	unsigned char r1					:1;
	unsigned char r2					:1;
	unsigned char r3					:1;
	unsigned char r4					:1;
} SensorsTypeDef;

typedef struct {
	unsigned char err0  :1;
	unsigned char err1  :1;
	unsigned char err2  :1;
	unsigned char err3  :1;
	unsigned char err4  :1;
	unsigned char err5  :1;
	unsigned char err6  :1;
	unsigned char err7  :1;
	unsigned char err8  :1;
	unsigned char err9  :1;
	unsigned char err10 :1;
	unsigned char err11 :1;
	unsigned char err12 :1;
	unsigned char err13 :1;
	unsigned char err14 :1;
	unsigned char err15 :1;
} ErrorMsgTypeDef;

typedef struct {
	uint8_t current_status;
	uint8_t wifi_status;
	uint8_t current_move_driver;
	uint8_t current_comm;
	uint8_t cs_err;
	uint8_t LEDB;
	float temp1;
	float temp2;
	float temp3;
	uint32_t heat_on;
	uint32_t heating_ok;
	SensorsTypeDef sens;
	ErrorMsgTypeDef error;
	uint32_t cycles_count;
	uint32_t cycles_set;
	float volume;
	uint32_t time_hold;
	//uint16_t LKEncoder;
	uint32_t LKEncoder;
	int8_t LKTemp;
} GlobDataTypeDef;

typedef struct {
	uint8_t start_msg0;
	uint8_t start_msg1;
	uint8_t control_id;
	uint8_t msg_id;
	uint8_t comm;
	uint8_t c1;
	uint8_t c2;
	uint8_t c3;
	float volume;
	uint32_t time_hold;
	uint32_t cycles;
	uint8_t r0;
	uint8_t r1;
	uint8_t r2;
	uint8_t CS;
} JobMsgTypeDef;

enum MSG_ID {
	MSG_NONE = 0,
	MSG_STATUS = 1,
	MSG_CONTROL,
	MSG_EMERGY_STOP,
	MSG_MAX,
};

enum MOVE_COMM
 {
	MOVE_NONE = 0,
	MOVE_POS_FB,		//left-right move
	MOVE_POS_LR,		//back-forw move
	MOVE_POS_FORK,		//fork move
	MOVE_SERVO,
	MOVE_ERROR,
	MOVE_MAX,
};

typedef struct
{
	uint8_t start_msg0;
	uint8_t start_msg1;
	uint8_t msg_id;
	uint8_t comm;
	int32_t pos_fb;
	int32_t pos_lr;
	int32_t pos_fork;
	int32_t pos_servo;
	uint8_t r0;
	uint8_t r1;
	uint8_t r2;
	uint8_t CS;
} ContrlMsgTypeDef;

typedef struct
{
	uint8_t start_msg0;
	uint8_t start_msg1;
	uint8_t msg_id;
	uint8_t comm;
	int32_t pos_fb;
	int32_t pos_lr;
	int32_t pos_fork;
	int32_t pos_servo;
	uint8_t r0;
	uint8_t r1;
	uint8_t r2;
	uint8_t CS;
} StatusMsgTypeDef;

enum LED_BLINK {
	LEDB_POWER_ON = 1,
	LEDB_START,
	LEDB_COMPLETED,
	LEDB_FLASH_OK,
	LEDB_ERROR,
	LEDB_MAX,
};

enum SERVO_COMM {
	SERVO_ZERO = 0,
	SERVO_FORW_OPEN,
	SERVO_FORW_CLOSE,
	SERVO_BACK_OPEN,
	SERVO_BACK_CLOSE,
	SERVO_ERROR,
	SERVO_MAX,
};

enum BEEPER {
	BEEP_POWER_ON = 1,
	BEEP_SEARCH = 2,
	BEEP_START,
	BEEP_COMPLETED,
	BEEP_WARNING,
	BEEP_ERROR,
	BEEP_FLASH_OK,
	BEEP_MAX,
};

#ifdef __cplusplus
}
#endif

#endif /* CPP_INC_TERMOPLAST_CONFIG_H_ */
