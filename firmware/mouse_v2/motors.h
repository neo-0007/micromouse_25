#ifndef MOTORS_H
#define MOTORS_H

#include<Arduino.h>

class Motor
{
   private:
      uint8_t speedPwmPin;
      uint8_t forwardMovePin;
      uint8_t backwardMovePin;
            
   public:

      Motor(uint8_t forwardMovePin, uint8_t backwardMovePin, uint8_t speedPwmPin);

      void init();

      void moveForward(uint8_t speed);

      void moveBackward(uint8_t speed);

      void stop();

      
      // Will be used by PID
      void setSpeed(int16_t speed);
};

#endif
