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

#define DRIVER_KEYA_ID 0x01

#define DRIVERS_QUANT 4

#define POS_X_TOLERANCE 100 //допуск позиции X, в режиме поиска стойки с какого расстояния начинать искать стойку
#define POS_Y_TOLERANCE 100
#define POS_X_RACK_DIST 10000 //на какое макс расстоянии искать стойку по X
#define POS_Y_RACK_DIST 10000
#define POS_FORK_TOLERANCE 50
#define LK_MAX_SPEED 100000
#define LK_MIN_SPEED 20000
#define KEYA_MAX_SPEED 3000 //6000
#define KEYA_MIN_SPEED 500
#define FORK_MAX_SPEED 3000 //6000
#define FORK_MIN_SPEED 1500
#define FORK_LIMIT_POS 100000 //расстояние выдвижения вилл
#define X_WHEEL_RATIO 1
#define Y_WHEEL_RATIO 1

#define RACK_SENS_X 0 //поиск стойки по X
#define RACK_SENS_Y 0 //поиск стойки по Y

#define SERVO_ANGLE_CLOSE 90 //заданный угол сервоприводов при закрытых упорах

#define FLASH_INIT 0x44

////////Control//////////
#define START_MSG0 0x84
#define START_MSG1 0x53
#define NET_CONTROL_ID 0x64

#define MAX_TEMP 200.0

typedef struct {
	uint8_t start_msg0;
	uint8_t start_msg1;
	uint8_t _id;
	uint8_t msg_id;

	uint16_t voltage;
	int16_t current;

	uint32_t remaining_capacity;
	uint16_t nominal_capacity;
	uint16_t n0;

	uint16_t cycles;
	uint16_t date;
	uint16_t balance_low;
	uint16_t balance_high;

	uint16_t protection;
	uint16_t capacity_percent;

	uint8_t version;
	uint8_t MOS_state;
	uint8_t num_of_battery;
	uint8_t num_of_NTC;

	uint16_t temp1;
	uint16_t temp2;

	uint8_t battery_pack;
	uint8_t p0;
	uint8_t p1;
	uint8_t p2;

	uint16_t max_volt;
	uint16_t min_volt;

	uint16_t cell_0;
	uint16_t cell_1;
	uint16_t cell_2;
	uint16_t cell_3;
	uint16_t cell_4;
	uint16_t cell_5;
	uint16_t cell_6;
	uint16_t cell_7;
	uint16_t cell_8;
	uint16_t cell_9;
	uint16_t cell_10;
	uint16_t cell_11;
	uint16_t cell_12;
	uint16_t cell_13;
	uint16_t cell_14;
	uint16_t cell_15;
	uint8_t bms_type;
	uint8_t reservz1;
	uint8_t reservz2;
	uint8_t CS;
} BatteryMsgTypeDef;

enum BMS_TYPE {
	BMS_NONE = 0,
	BMS_SMART,
	BMS_JBD,
	BMS_MAX,
};


typedef struct
{
	unsigned char b0 : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
} BitParasmsTypeDef;

typedef struct {
	uint32_t p0;
	uint32_t p1;
	uint32_t p2;
	uint32_t p3;
	BitParasmsTypeDef bitParams;
	uint8_t u0;
	uint8_t u1;
	uint8_t CS;
} MinibotConfigTypeDef;

typedef struct {
	uint8_t start_msg0;
	uint8_t start_msg1;
	uint8_t control_id;
	uint8_t msg_id;
	MinibotConfigTypeDef botConfig;
	uint8_t reservU0;
	uint8_t reservU1;
	uint8_t reservU2;
	uint8_t CS;
} StmConfigTypeDef;

typedef struct {
	unsigned char limit_sw1				:1;
	unsigned char limit_sw2				:1;
	unsigned char limit_platform_up		:1;
	unsigned char limit_platform_down	:1;
	unsigned char limit_servo_open		:1;
	unsigned char limit_servo_close		:1;
	unsigned char limit_fork_center		:1;
	unsigned char sw_box				:1;
	unsigned char sw_rackX				:1;
	unsigned char sw_rackY				:1;
	unsigned char lim0					:1;
	unsigned char r0					:1;
	unsigned char r1					:1;
	unsigned char r2					:1;
	unsigned char r3					:1;
	unsigned char r4					:1;
} SensorsTypeDef;

typedef struct {
	unsigned char driverX_err	:1;
	unsigned char driverY_err	:1;
	unsigned char driverF_err	:1;
	unsigned char driverL_err	:1;
	unsigned char bms_err		:1;
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
	unsigned char cs_err :1;
} ErrorMsgTypeDef;

typedef struct {
	uint8_t current_status;
	uint8_t wifi_status;
	uint8_t current_comm;
	uint8_t action_comm;
	uint8_t cs_err;
	uint8_t LEDB;
	SensorsTypeDef sens;
	ErrorMsgTypeDef error;
	uint32_t LKEncoder;
	int32_t enc_Y1;
	int32_t enc_fork;
	int32_t lift_status;
	uint32_t comm_count;
	uint8_t capacity;
	uint8_t rack_sens;
} GlobDataTypeDef;

enum MSG_ID {
	MSG_NONE = 0,
	MSG_STATUS = 1,
	MSG_CONTROL,
	MSG_MAX,
};

enum MOVE_COMM
 {
	MOVE_NONE = 0,
	MOVE_POS_X,		//left-right move
	MOVE_POS_Y,		//back-forw move
	MOVE_POS_FORK,	//fork move
	MOVE_LIFT,
	MOVE_SERVO,
	MOVE_EMERGY_STOP,
	MOVE_RESET,
	MOVE_ACTION,
	MOVE_ERROR,
	MOVE_MAX,
};

enum ACTION_COMM
 {
	ACTION_NONE = 0,
	ACTION_GET_BOX_L,
	ACTION_GET_BOX_R,
	ACTION_PUT_BOX_L,
	ACTION_PUT_BOX_R,
	ACTION_ERROR,
	ACTION_MAX,
};

enum LIFT_POS {
	LIFT_NONE = 0,
	LIFT_UP,
	LIFT_DOWN,
	LIFT_MAX,
};

enum SERVO_POS {
	SERVO_NONE = 0,
	SERVO_OPEN,
	SERVO_CLOSE,
	SERVO_MAX,
};

typedef struct
{
	uint8_t start_msg0;
	uint8_t start_msg1;
	uint8_t msg_id;
	uint8_t comm;
	int32_t pos_x;
	int32_t pos_y;
	int32_t pos_fork;
	int32_t pos_lift;
	int32_t pos_servo;
	uint8_t action_comm;
	uint8_t x_hold;
	uint8_t y_hold;
	uint8_t r3;
	uint8_t r2;
	uint8_t r1;
	uint8_t r0;
	uint8_t CS;
} ContrlMsgTypeDef;

typedef struct
{
	uint8_t start_msg0;
	uint8_t start_msg1;
	uint8_t msg_id;
	uint8_t comm;
	int32_t pos_x;
	int32_t pos_y;
	int32_t pos_fork;
	int32_t pos_lift;
	int32_t pos_servo;
	int32_t status;
	int32_t msg_count;
	int32_t cs_err;
	uint32_t comm_count;
	SensorsTypeDef sens;
	ErrorMsgTypeDef error;
	uint8_t action_comm;
	uint8_t x_hold;
	uint8_t y_hold;
	uint8_t capacity;
	uint8_t r2;
	uint8_t r1;
	uint8_t r0;
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
