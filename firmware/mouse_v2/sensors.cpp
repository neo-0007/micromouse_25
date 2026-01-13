#include "sensors.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trigPin, uint8_t echoPin)
{
    this->trigPin = trigPin;
    this->echoPin = echoPin;
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
    // Non blocking
    // TODO
}

long UltrasonicSensor::getDistance()
{
    return getDistanceBlocking(); // Will upgrade later
}

long UltrasonicSensor::getDistanceBlocking()
{
    // Ensure clean trigger
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Send 10us pulse
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Read echo (timeout 30ms ≈ 5m)
    unsigned long duration = pulseIn(echoPin, HIGH, 30000);
    
    if (duration == 0) {
        return -1; 
    }
    
    // Calculate distance in cm
    distanceCm = duration * 0.034 / 2;
    return distanceCm;
}
