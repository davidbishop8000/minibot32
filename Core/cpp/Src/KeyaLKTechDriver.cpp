/*
 * KeyaLKTechDriver.cpp
 *
 *  Created on: Feb 27, 2024
 *      Author: user
 */

#include <KeyaLKTechDriver.h>

KeyaLKTechDriver::KeyaLKTechDriver(uint32_t extId, uint32_t axis, uint32_t stdId = 0)
{
	_axis = axis;
	_canTxHeader.ExtId = extId;
	_canTxHeader.StdId = stdId;
	_canTxHeader.RTR = CAN_RTR_DATA;
	_canTxHeader.DLC = 8;
	_canTxHeader.TransmitGlobalTime = DISABLE;
	if (extId)
	{
		_canTxHeader.IDE = CAN_ID_EXT;
	}
	else
	{
		_canTxHeader.IDE = CAN_ID_STD;
	}
}
KeyaLKTechDriver::KeyaLKTechDriver(uint32_t extId, uint32_t axis) : KeyaLKTechDriver::KeyaLKTechDriver(extId, axis, 0){}
KeyaLKTechDriver::KeyaLKTechDriver(uint32_t stdId) : KeyaLKTechDriver::KeyaLKTechDriver(0, 0, stdId){}

uint8_t KeyaLKTechDriver::setSpeed(int32_t speed)
{
	if (_axis)
	{
		_canData[0] = 0x23;
		_canData[1] = 0x00;
		_canData[2] = 0x20;
		_canData[3] = _axis;
		_canData[4] = speed >> 24;
		_canData[5] = speed >> 16;
		_canData[6] = speed >> 8;
		_canData[7] = speed;
	}
	else
	{
		_canData[0] = 0xA2;
		_canData[1] = 0x00;
		_canData[2] = 0x00;
		_canData[3] = 0x00;
		_canData[4] = speed;
		_canData[5] = speed >> 8;
		_canData[6] = speed >> 16;
		_canData[7] = speed >> 24;
		/*_canData[4] = *(uint8_t *)(&speed);
		_canData[5] = *((uint8_t *)(&speed)+1);
		_canData[6] = *((uint8_t *)(&speed)+2);
		_canData[7] = *((uint8_t *)(&speed)+3);*/
	}

	return KeyaLKTechDriver::sendData();
}

uint8_t KeyaLKTechDriver::stop()
{
	if (_axis) {
		_canData[0] = 0x23;
		_canData[1] = 0x00;
		_canData[2] = 0x20;
		_canData[3] = _axis;
		_canData[4] = 0x00;
		_canData[5] = 0x00;
		_canData[6] = 0x00;
		_canData[7] = 0x00;
	} else {
		_canData[0] = 0x81;
		_canData[1] = 0x00;
		_canData[2] = 0x00;
		_canData[3] = 0x00;
		_canData[4] = 0x00;
		_canData[5] = 0x00;
		_canData[6] = 0x00;
		_canData[7] = 0x00;
	}

	return KeyaLKTechDriver::sendData();
}

uint8_t KeyaLKTechDriver::enable()
{
	if (_axis) {
		_canData[0] = 0x23;
		_canData[1] = 0x0D;
		_canData[2] = 0x20;
		_canData[3] = _axis;
		_canData[4] = 0x00;
		_canData[5] = 0x00;
		_canData[6] = 0x00;
		_canData[7] = 0x00;
	} else {
		_canData[0] = 0x88;
		_canData[1] = 0x00;
		_canData[2] = 0x00;
		_canData[3] = 0x00;
		_canData[4] = 0x00;
		_canData[5] = 0x00;
		_canData[6] = 0x00;
		_canData[7] = 0x00;
	}

	return KeyaLKTechDriver::sendData();
}

uint8_t KeyaLKTechDriver::disable()
{
	if (_axis) {
		_canData[0] = 0x23;
		_canData[1] = 0x0C;
		_canData[2] = 0x20;
		_canData[3] = _axis;
		_canData[4] = 0x00;
		_canData[5] = 0x00;
		_canData[6] = 0x00;
		_canData[7] = 0x00;
	} else {
		_canData[0] = 0x80;
		_canData[1] = 0x00;
		_canData[2] = 0x00;
		_canData[3] = 0x00;
		_canData[4] = 0x00;
		_canData[5] = 0x00;
		_canData[6] = 0x00;
		_canData[7] = 0x00;
	}

	return KeyaLKTechDriver::sendData();
}

uint8_t KeyaLKTechDriver::sendData()
{
	return CanMsgSend(&_canTxHeader, _canData);
}


