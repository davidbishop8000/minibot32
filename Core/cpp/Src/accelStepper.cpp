#include "accelStepper.h"

void AccelStepper::moveTo(long absolute)
{
    if (_targetPos != absolute)
    {
		_targetPos = absolute;
		computeNewSpeed();
		// compute new n?
    }
}

void AccelStepper::move(long relative)
{
    moveTo(_currentPos + relative);
}

// Implements steps according to the current step interval
// You must call this at least once per step
// returns true if a step occurred
uint8_t AccelStepper::runSpeed()
{
    // Dont do anything unless we actually have a step interval
    if (!_stepInterval)
	return 0;

    unsigned long time = (unsigned long)micros();
    unsigned long nextStepTime = _lastStepTime + _stepInterval;
    // Gymnastics to detect wrapping of either the nextStepTime and/or the current time
    if (((nextStepTime >= _lastStepTime) && ((time >= nextStepTime) || (time < _lastStepTime)))
	|| ((nextStepTime < _lastStepTime) && ((time >= nextStepTime) && (time < _lastStepTime))))
    {
		if (_direction == DIRECTION_CW)
		{
			// Clockwise
			_currentPos += 1;
		}
		else
		{
			// Anticlockwise
			_currentPos -= 1;
		}
		step(_currentPos);
		_lastStepTime = time;
		return 1;
    }
    else
    {
    	return 0;
    }
}

void AccelStepper::rotate()
{
	step(1);
}

long AccelStepper::distanceToGo()
{
    return _targetPos - _currentPos;
}

long AccelStepper::targetPosition()
{
    return _targetPos;
}

long AccelStepper::currentPosition()
{
    return _currentPos;
}

// Useful during initialisations or after initial positioning
// Sets speed to 0
void AccelStepper::setCurrentPosition(long position)
{
    _targetPos = _currentPos = position;
    _n = 0;
    _stepInterval = 0;
}

void AccelStepper::computeNewSpeed()
{
    long distanceTo = distanceToGo(); // +ve is clockwise from curent location

    long stepsToStop = (long)((_speed * _speed) / (2.0f * _acceleration)); // Equation 16
    HAL_GPIO_WritePin(_enPort, _enPin, GPIO_PIN_RESET);

    if (distanceTo == 0 && stepsToStop <= 1)
    {
		// We are at the target and its time to stop
		_stepInterval = 0;
		_speed = 0.0;
		_n = 0;
		HAL_GPIO_WritePin(_enPort, _enPin, GPIO_PIN_SET);
		return;
    }

    if (distanceTo > 0)
    {
		// We are anticlockwise from the target
		// Need to go clockwise from here, maybe decelerate now
		if (_n > 0)
		{
			// Currently accelerating, need to decel now? Or maybe going the wrong way?
			if ((stepsToStop >= distanceTo) || _direction == DIRECTION_CCW)
			_n = -stepsToStop; // Start deceleration
		}
		else if (_n < 0)
		{
			// Currently decelerating, need to accel again?
			if ((stepsToStop < distanceTo) && _direction == DIRECTION_CW)
			_n = -_n; // Start accceleration
		}
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(_dirPort, _dirPin, GPIO_PIN_SET);
    }
    else if (distanceTo < 0)
    {
		// We are clockwise from the target
		// Need to go anticlockwise from here, maybe decelerate
		if (_n > 0)
		{
			// Currently accelerating, need to decel now? Or maybe going the wrong way?
			if ((stepsToStop >= -distanceTo) || _direction == DIRECTION_CW)
			_n = -stepsToStop; // Start deceleration
		}
		else if (_n < 0)
		{
			// Currently decelerating, need to accel again?
			if ((stepsToStop < -distanceTo) && _direction == DIRECTION_CCW)
			_n = -_n; // Start accceleration
		}
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(_dirPort, _dirPin, GPIO_PIN_RESET);
    }

    // Need to accelerate or decelerate
    if (_n == 0)
    {
		// First step from stopped
		_cn = _c0;
		_direction = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
    }
    else
    {
		// Subsequent step. Works for accel (n is +_ve) and decel (n is -ve).
		_cn = _cn - ((2.0f * _cn) / ((4.0f * _n) + 1)); // Equation 13
		_cn = fmax(_cn, _cmin);
    }
    _n++;
    _stepInterval = _cn;
    _speed = 1000000.0f / _cn;
    if (_direction == DIRECTION_CCW)
    {
    	_speed = -_speed;
    }
}

// Run the motor to implement speed and acceleration in order to proceed to the target position
// You must call this at least once per step, preferably in your main loop
// If the motor is in the desired position, the cost is very small
// returns true if the motor is still running to the target position.
uint8_t AccelStepper::run()
{
    if (runSpeed())
	computeNewSpeed();
    return _speed != 0.0f || distanceToGo() != 0;
}

AccelStepper::AccelStepper(GPIO_TypeDef* pulse_port, uint16_t pulse_pin, GPIO_TypeDef* dir_port, uint16_t dir_pin, GPIO_TypeDef* en_port, uint16_t en_pin)
{
	_pulsePort = pulse_port;
	_pulsePin = pulse_pin;
	_dirPort = dir_port;
	_dirPin = dir_pin;
	_enPort = en_port;
	_enPin = en_pin;
    _currentPos = 0;
    _targetPos = 0;
    _speed = 0.0;
    _maxSpeed = 1.0;
    _acceleration = 0.0;
    _sqrt_twoa = 1.0;
    _stepInterval = 0;
    _minPulseWidth = 1;
    _lastStepTime = 0;

    // NEW
    _n = 0;
    _c0 = 0.0;
    _cn = 0.0;
    _cmin = 1.0;
    _direction = DIRECTION_CCW;

    setAcceleration(1);
}

void AccelStepper::setMaxSpeed(float speed)
{
    if (_maxSpeed != speed)
    {
	_maxSpeed = speed;
	_cmin = 1000000.0f / speed;
	// Recompute _n from current speed and adjust speed if accelerating or cruising
	if (_n > 0)
	{
	    _n = (long)((_speed * _speed) / (2.0f * _acceleration)); // Equation 16
	    computeNewSpeed();
	}
    }
}

float   AccelStepper::maxSpeed()
{
    return _maxSpeed;
}

void AccelStepper::setAcceleration(float acceleration)
{
    if (acceleration == 0.0f)
    {
    	return;
    }
    if (_acceleration != acceleration)
    {
		// Recompute _n per Equation 17
		_n = _n * (_acceleration / acceleration);
		// New c0 per Equation 7, with correction per Equation 15
		_c0 = 0.676f * sqrt(2.0f / acceleration) * 1000000.0f; // Equation 15
		_acceleration = acceleration;
		computeNewSpeed();
    }
}

void AccelStepper::setSpeed(float speed)
{
    if (speed == _speed)
        return;
    speed = constrain(speed, -_maxSpeed, _maxSpeed);
    if (speed == 0.0f)
	_stepInterval = 0;
    else
    {
	_stepInterval = fabs(1000000.0f / speed);
	_direction = (speed > 0.0f) ? DIRECTION_CW : DIRECTION_CCW;
    }
    _speed = speed;
}

float AccelStepper::speed()
{
    return _speed;
}

// 1 pin step function (ie for stepper drivers)
// This is passed the current step number (0 to 7)
// Subclasses can override
void AccelStepper::step(long step)
{   
	HAL_GPIO_WritePin(_dirPort, _dirPin, (GPIO_PinState)_direction);
	//delay_us(_minPulseWidth);
	HAL_GPIO_WritePin(_pulsePort, _pulsePin, GPIO_PIN_SET);

    delay_us(_minPulseWidth);
    HAL_GPIO_WritePin(_pulsePort, _pulsePin, GPIO_PIN_RESET);
    
}

void    AccelStepper::disableOutputs()
{   

}

void    AccelStepper::enableOutputs()
{

}

void AccelStepper::setMinPulseWidth(unsigned int minWidth)
{
    _minPulseWidth = minWidth;
}

// Blocks until the target position is reached and stopped
void AccelStepper::runToPosition()
{
    while (run());
}

uint8_t AccelStepper::runSpeedToPosition()
{
    if (_targetPos == _currentPos)
	return 0;
    if (_targetPos >_currentPos)
	_direction = DIRECTION_CW;
    else
	_direction = DIRECTION_CCW;
    return runSpeed();
}

// Blocks until the new target position is reached
void AccelStepper::runToNewPosition(long position)
{
    moveTo(position);
    runToPosition();
}

void AccelStepper::stop()
{
    if (_speed != 0.0f)
    {    
	long stepsToStop = (long)((_speed * _speed) / (2.0f * _acceleration)) + 1; // Equation 16 (+integer rounding)
	if (_speed > 0)
	    move(stepsToStop);
	else
	    move(-stepsToStop);
    }
}

void AccelStepper::microDelay(uint16_t delay)
{
	//__HAL_TIM_SET_COUNTER(&htim1, 0);
	//  while (__HAL_TIM_GET_COUNTER(&htim1) < delay);
	delay_us(delay);
}

float AccelStepper::constrain(float x, float a, float b)
{
	if (x < a)
	{
		return a;
	}
	else if (b < x)
	{
		return b;
	}
	else return x;
}
