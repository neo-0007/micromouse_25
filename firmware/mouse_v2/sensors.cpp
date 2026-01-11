#include "sensors.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trigPin, uint8_t echoPin)
{
    this->trigPin = trigPin;
    this->echoPin = echoPin;
    state = IDLE;
    distanceCm = -1;
}

void UltrasonicSensor::init()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
}

void UltrasonicSensor::update()
{
    unsigned long nowMicros = micros();
    unsigned long nowMillis = millis();

    switch (state)
    {
        case IDLE:
            // Start a new ping
            digitalWrite(trigPin, HIGH);
            tMicros = nowMicros;
            state = TRIGGER_PULSE;
            break;

        case TRIGGER_PULSE:
            if (nowMicros - tMicros >= 10) {   // 10 µs pulse
                digitalWrite(trigPin, LOW);
                tMicros = nowMicros;
                state = WAIT_RISE;
            }
            break;

        case WAIT_RISE:
            if (digitalRead(echoPin)) {
                echoStart = micros();
                state = WAIT_FALL;
            }
            else if (nowMicros - tMicros > 30000) { // 30 ms timeout
                distanceCm = -1;
                tMillis = nowMillis;
                state = COOLDOWN;
            }
            break;

        case WAIT_FALL:
            if (!digitalRead(echoPin)) {
                unsigned long duration = micros() - echoStart;
                distanceCm = duration * 0.034 / 2;
                tMillis = nowMillis;
                state = COOLDOWN;
            }
            else if (nowMicros - echoStart > 30000) {
                distanceCm = -1;
                tMillis = nowMillis;
                state = COOLDOWN;
            }
            break;

        case COOLDOWN:
            // Wait before next trigger (prevents crosstalk)
            if (nowMillis - tMillis >= 40) {
                state = IDLE;
            }
            break;
    }
}

long UltrasonicSensor::getDistance()
{
    return distanceCm;
}
