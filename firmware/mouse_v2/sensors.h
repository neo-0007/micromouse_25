#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class UltrasonicSensor
{
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin);

    void init();
    void update();          // Not used now
    long getDistance();
    long UltrasonicSensor::getDistanceBlocking();    

private:
    uint8_t trigPin;
    uint8_t echoPin;
    long distanceCm;
};

#endif
