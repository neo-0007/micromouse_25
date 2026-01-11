#define BASE_SPEED    140     // motor speed
#define STOP_DISTANCE 12      // cm from wall to stop

#include "config.h"
#include "motors.h"
#include "sensors.h"

Motor leftMotor(IN2, IN1, ENA);
Motor rightMotor(IN3, IN4, ENB);

UltrasonicSensor frontSensor(FRONT_TRIG, FRONT_ECHO);

void setup()
{
    Serial.begin(9600);

    leftMotor.init();
    rightMotor.init();
    frontSensor.init();

    Serial.println("Front wall stop test");
}

void loop()
{
    // Always update the sensor FSM
    frontSensor.update();

    long front = frontSensor.getDistance();

    if (front > 0 && front <= STOP_DISTANCE) {
        leftMotor.stop();
        rightMotor.stop();
    } 
    else {
        leftMotor.moveForward(BASE_SPEED);
        rightMotor.moveForward(BASE_SPEED);
    }

    // Debug output (optional)
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 200) {
        lastPrint = millis();
        Serial.print("Front distance: ");
        Serial.println(front);
    }
}
