#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class UltrasonicSensor
{
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin);

    void init();
    void update();          // call every loop
    long getDistance();     // latest distance in cm (-1 if invalid)

private:
    enum State {
        IDLE,
        TRIGGER_PULSE,
        WAIT_RISE,
        WAIT_FALL,
        COOLDOWN
    };

    uint8_t trigPin;
    uint8_t echoPin;

    State state;

    unsigned long tMicros;   // for µs timing
    unsigned long tMillis;   // for ms cooldown
    unsigned long echoStart;

    long distanceCm;
};

#endif
