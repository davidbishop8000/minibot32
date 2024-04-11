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
	_speed = 0;
	return KeyaLKTechDriver::sendData();
}
uint8_t KeyaLKTechDriver::stopHold()
{
	_speed = 0;
	KeyaLKTechDriver::setSpeed(_speed);
	return 0;
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

uint32_t KeyaLKTechDriver::getId()
{

	if (_canTxHeader.ExtId)
	{
		return  _canTxHeader.ExtId;
	}
	return  _canTxHeader.StdId;
}

uint8_t KeyaLKTechDriver::readEnc()
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
		_canData[0] = 0x9C;
		_canData[1] = 0x00;
		_canData[2] = 0x00;
		_canData[3] = 0x00;
		_canData[4] = 0x00;
		_canData[5] = 0x00;
		_canData[6] = 0x00;
		_canData[7] = 0x00;
	}
	return CanMsgSend(&_canTxHeader, _canData);
}

int32_t KeyaLKTechDriver::getPos()
{
	return _enc;
}

int32_t KeyaLKTechDriver::getSpeed()
{
	return _speed;
}

int8_t KeyaLKTechDriver::getTemp()
{
	return _temp;
}

void KeyaLKTechDriver::setTemp(int8_t temp)
{
	_temp = temp;
}

void KeyaLKTechDriver::setOffset()
{
	_enc_offset = _enc;
}

uint8_t KeyaLKTechDriver::setPos(int32_t pos)
{
	if (!_canTxHeader.ExtId)
	{
		KeyaLKTechDriver::readEnc();
		osDelay(5);
		//KeyaLKTechDriver::enable();
		if (pos < _enc - POS_TOLERANCE)
		{
			_speed = -LK_MAX_SPEED;
			KeyaLKTechDriver::setSpeed(_speed);
		}
		else if (pos > _enc + POS_TOLERANCE)
		{
			_speed = LK_MAX_SPEED;
			KeyaLKTechDriver::setSpeed(_speed);
		}
		else
		{
			KeyaLKTechDriver::stop();
		}
		osDelay(5);
	}
	return 0;
}

void KeyaLKTechDriver::setEnc(int32_t enc)
{
	_enc = KeyaLKTechDriver::UnwrapEncoder(enc, &_prevEnc) - _enc_offset;
}

int32_t KeyaLKTechDriver::UnwrapEncoder(uint16_t in, int32_t *prev)
{
    int32_t c32 = (int32_t)in - LK_ENC_HALF_PERIOD;
    int32_t dif = (c32-*prev);

    int32_t mod_dif = ((dif + LK_ENC_HALF_PERIOD) % LK_ENC_ONE_PERIOD) - LK_ENC_HALF_PERIOD;
    if(dif < -LK_ENC_HALF_PERIOD) {
        mod_dif += LK_ENC_ONE_PERIOD;
    }
    int32_t unwrapped = *prev + mod_dif;
    *prev = unwrapped;

    return unwrapped + LK_ENC_HALF_PERIOD;
}

uint8_t KeyaLKTechDriver::sendData()
{
	return CanMsgSend(&_canTxHeader, _canData);
}
