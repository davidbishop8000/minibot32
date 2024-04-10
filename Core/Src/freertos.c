/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "minibot_config.h"
#include "outputs.h"
#include "inputs.h"
#include "servo_control.h"
#include "uart_wifi.h"
#include "drivers_control.h"
#include "canbus.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

GlobDataTypeDef globData;
MinibotConfigTypeDef minibotConfig;
ContrlMsgTypeDef contrlMsg;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for OutputsTask */
osThreadId_t OutputsTaskHandle;
uint32_t OutputsTaskBuffer[ 128 ];
osStaticThreadDef_t OutputsTaskControlBlock;
const osThreadAttr_t OutputsTask_attributes = {
  .name = "OutputsTask",
  .cb_mem = &OutputsTaskControlBlock,
  .cb_size = sizeof(OutputsTaskControlBlock),
  .stack_mem = &OutputsTaskBuffer[0],
  .stack_size = sizeof(OutputsTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for InputsTask */
osThreadId_t InputsTaskHandle;
uint32_t InputsTaskBuffer[ 128 ];
osStaticThreadDef_t InputsTaskControlBlock;
const osThreadAttr_t InputsTask_attributes = {
  .name = "InputsTask",
  .cb_mem = &InputsTaskControlBlock,
  .cb_size = sizeof(InputsTaskControlBlock),
  .stack_mem = &InputsTaskBuffer[0],
  .stack_size = sizeof(InputsTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ServoTask */
osThreadId_t ServoTaskHandle;
uint32_t ServoTaskBuffer[ 128 ];
osStaticThreadDef_t ServoTaskControlBlock;
const osThreadAttr_t ServoTask_attributes = {
  .name = "ServoTask",
  .cb_mem = &ServoTaskControlBlock,
  .cb_size = sizeof(ServoTaskControlBlock),
  .stack_mem = &ServoTaskBuffer[0],
  .stack_size = sizeof(ServoTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartWiFiTask */
osThreadId_t UartWiFiTaskHandle;
uint32_t UartWiFiTaskBuffer[ 256 ];
osStaticThreadDef_t UartWiFiTaskControlBlock;
const osThreadAttr_t UartWiFiTask_attributes = {
  .name = "UartWiFiTask",
  .cb_mem = &UartWiFiTaskControlBlock,
  .cb_size = sizeof(UartWiFiTaskControlBlock),
  .stack_mem = &UartWiFiTaskBuffer[0],
  .stack_size = sizeof(UartWiFiTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CanDriversTask */
osThreadId_t CanDriversTaskHandle;
uint32_t CanDriversTaskBuffer[ 128 ];
osStaticThreadDef_t CanDriversTaskControlBlock;
const osThreadAttr_t CanDriversTask_attributes = {
  .name = "CanDriversTask",
  .cb_mem = &CanDriversTaskControlBlock,
  .cb_size = sizeof(CanDriversTaskControlBlock),
  .stack_mem = &CanDriversTaskBuffer[0],
  .stack_size = sizeof(CanDriversTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CanTask */
osThreadId_t CanTaskHandle;
uint32_t CanTaskBuffer[ 256 ];
osStaticThreadDef_t CanTaskControlBlock;
const osThreadAttr_t CanTask_attributes = {
  .name = "CanTask",
  .cb_mem = &CanTaskControlBlock,
  .cb_size = sizeof(CanTaskControlBlock),
  .stack_mem = &CanTaskBuffer[0],
  .stack_size = sizeof(CanTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MissionsTask */
osThreadId_t MissionsTaskHandle;
uint32_t MissionsTaskBuffer[ 256 ];
osStaticThreadDef_t MissionsTaskControlBlock;
const osThreadAttr_t MissionsTask_attributes = {
  .name = "MissionsTask",
  .cb_mem = &MissionsTaskControlBlock,
  .cb_size = sizeof(MissionsTaskControlBlock),
  .stack_mem = &MissionsTaskBuffer[0],
  .stack_size = sizeof(MissionsTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void StartOutputsTask(void *argument);
extern void StartInputsTask(void *argument);
extern void StartServoTask(void *argument);
extern void StartUartWiFiTask(void *argument);
extern void StartCanDriversTask(void *argument);
extern void StartCanTask(void *argument);
extern void StartMissionsTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of OutputsTask */
  OutputsTaskHandle = osThreadNew(StartOutputsTask, NULL, &OutputsTask_attributes);

  /* creation of InputsTask */
  InputsTaskHandle = osThreadNew(StartInputsTask, NULL, &InputsTask_attributes);

  /* creation of ServoTask */
  ServoTaskHandle = osThreadNew(StartServoTask, NULL, &ServoTask_attributes);

  /* creation of UartWiFiTask */
  UartWiFiTaskHandle = osThreadNew(StartUartWiFiTask, NULL, &UartWiFiTask_attributes);

  /* creation of CanDriversTask */
  CanDriversTaskHandle = osThreadNew(StartCanDriversTask, NULL, &CanDriversTask_attributes);

  /* creation of CanTask */
  CanTaskHandle = osThreadNew(StartCanTask, NULL, &CanTask_attributes);

  /* creation of MissionsTask */
  MissionsTaskHandle = osThreadNew(StartMissionsTask, NULL, &MissionsTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

