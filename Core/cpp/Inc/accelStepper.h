#ifndef INC_ACCEL_STEPPER_H_
#define INC_ACCEL_STEPPER_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "micros.h"
#include "math.h"

class AccelStepper
{
public:
    AccelStepper(GPIO_TypeDef* pulse_port, uint16_t pulse_pin, GPIO_TypeDef* dir_port, uint16_t dir_pin, GPIO_TypeDef* en_port, uint16_t en_pin);

    void moveTo(long absolute);

    void move(long relative);

    uint8_t run();

    uint8_t runSpeed();

    void rotate();

    void setMaxSpeed(float speed);

    float maxSpeed();

    void setAcceleration(float acceleration);

    void setSpeed(float speed);

    float speed();

    long distanceToGo();

    /// The most recently set target position.
    /// \return the target position
    /// in steps. Positive is clockwise from the 0 position.
    long targetPosition();

    long currentPosition();

    void setCurrentPosition(long position);

    void  runToPosition();

    uint8_t runSpeedToPosition();

    void runToNewPosition(long position);

    void stop();

    virtual void disableOutputs();

    virtual void enableOutputs();

    void setMinPulseWidth(unsigned int minWidth);

    void microDelay(uint16_t delay);

    float constrain(float x, float a, float b);

protected:

    /// \brief Direction indicator
    /// Symbolic names for the direction the motor is turning
    typedef enum
    {
    	DIRECTION_CCW = 0,  ///< Clockwise
        DIRECTION_CW  = 1   ///< Counter-Clockwise
    } Direction;

    void computeNewSpeed();

    virtual void step(long step);

private:
    /// The current absolution position in steps.
    long _currentPos;    // Steps

    /// The target position in steps. The AccelStepper library will move the
    /// motor from the _currentPos to the _targetPos, taking into account the
    /// max speed, acceleration and deceleration
    long _targetPos;     // Steps

    /// The current motos speed in steps per second
    /// Positive is clockwise
    float _speed;         // Steps per second

    /// The maximum permitted speed in steps per second. Must be > 0.
    float _maxSpeed;

    /// The acceleration to use to accelerate or decelerate the motor in steps
    /// per second per second. Must be > 0
    float _acceleration;
    float _sqrt_twoa; // Precomputed sqrt(2*_acceleration)

    /// The current interval between steps in microseconds.
    /// 0 means the motor is currently stopped with _speed == 0
    unsigned long _stepInterval;

    /// The last step time in microseconds
    unsigned long _lastStepTime;

    /// The minimum allowed pulse width in microseconds
    unsigned int _minPulseWidth;

    uint16_t _pulsePin;
    GPIO_TypeDef* _pulsePort;

    uint16_t _dirPin;
    GPIO_TypeDef* _dirPort;

    /// Enable pin for stepper driver, or 0xFF if unused.
    uint16_t _enPin;
	GPIO_TypeDef* _enPort;

    /// The step counter for speed calculations
    long _n;

    /// Initial step size in microseconds
    float _c0;

    /// Last step size in microseconds
    float _cn;

    /// Min step size in microseconds based on maxSpeed
    float _cmin; // at max speed

    /// Current direction motor is spinning in
    uint8_t _direction; // 1 == CW

};

#endif /* INC_ACCEL_STEPPER_H_ */
