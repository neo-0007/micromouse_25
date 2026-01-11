#include "motors.h"

Motor::Motor(uint8_t forwardMovePin, uint8_t backwardMovePin, uint8_t speedPwmPin)
{
  this->speedPwmPin = speedPwmPin;
  this->forwardMovePin = forwardMovePin;
  this->backwardMovePin = backwardMovePin;
}

void Motor::init()
{
  pinMode(speedPwmPin,OUTPUT);
  pinMode(forwardMovePin,OUTPUT);
  pinMode(backwardMovePin,OUTPUT);
}

void Motor::moveForward(uint8_t speed)
{
  analogWrite(speedPwmPin, speed);
  digitalWrite(forwardMovePin, HIGH);
  digitalWrite(backwardMovePin, LOW);
}

void Motor::moveBackward(uint8_t speed)
{
  analogWrite(speedPwmPin, speed);
  digitalWrite(forwardMovePin, LOW);
  digitalWrite(backwardMovePin, HIGH); 
}

void Motor::stop()
{
  digitalWrite(forwardMovePin, HIGH);
  digitalWrite(backwardMovePin, HIGH); 
  analogWrite(speedPwmPin, 255);
}

void Motor::setSpeed(int16_t speed)
{
    speed = constrain(speed, -255, 255);

    if (speed > 0)
        moveForward((uint8_t)speed);
    else if (speed < 0)
        moveBackward((uint8_t)(-speed));
    else
        stop();
}
