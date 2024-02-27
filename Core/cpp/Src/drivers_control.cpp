/*
 * drivers_control.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: user
 */

#include "drivers_control.h"
#include "lkTechDriver.h"

extern GlobDataTypeDef globData;
extern MinibotConfigTypeDef minibotConfig;

void StartCanDriversTask(void *argument)
{
	LkTechDriver lkTechDriverFB(DRIVER1_LKTECH_ID);
	LkTechDriver lkTechDriverLR(DRIVER1_LKTECH_ID);


	CanDataSendTypeDef canDataSend;
	int driver_keya_axis1_en = 0;
	int driver_keya_axis2_en = 0;
	for(;;)
	{
		if (globData.current_move_driver == MOVE_MOTOR_FB)
		{
			canDataSend.canExtId = DRIVER1_LKTECH_ID + 0x06000000;
			canDataSend.canRTR = CAN_RTR_DATA;
		}
		else if (globData.current_move_driver == MOVE_MOTOR_LR)
		{
			canDataSend.canExtId = DRIVER_KEYA_ID + 0x06000000;
			canDataSend.canRTR = CAN_RTR_DATA;
			if (!driver_keya_axis1_en) {
				driver_keya_axis1_en = 1;
				canDataSend.canData[0] = 0x23;
				canDataSend.canData[1] = 0x0D;
				canDataSend.canData[2] = 0x20;
				canDataSend.canData[3] = 0x01;
				canDataSend.canData[4] = 0x00;
				canDataSend.canData[5] = 0x00;
				canDataSend.canData[6] = 0x00;
				canDataSend.canData[7] = 0x00;
				CanMsgSend(&canDataSend);
				osDelay(10);
			}
			if (!driver_keya_axis2_en) {
				driver_keya_axis2_en = 1;
				canDataSend.canData[0] = 0x23;
				canDataSend.canData[1] = 0x0D;
				canDataSend.canData[2] = 0x20;
				canDataSend.canData[3] = 0x02;
				canDataSend.canData[4] = 0x00;
				canDataSend.canData[5] = 0x00;
				canDataSend.canData[6] = 0x00;
				canDataSend.canData[7] = 0x00;
				CanMsgSend(&canDataSend);
				osDelay(10);
			}

		}
	}
	osDelay(1);
}
