#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// =====================================================
//                    MOTOR PINS
// =====================================================

// LEFT MOTOR
#define ENA 10
#define IN1 8      // LEFT BACK
#define IN2 11     // LEFT FRONT

// RIGHT MOTOR
#define ENB 6
#define IN3 A0     // RIGHT FRONT
#define IN4 A1     // RIGHT BACK


// =====================================================
//                   ENCODER PINS
// =====================================================

// LEFT ENCODER
#define LEFT_ENC_A 2      // INT0
#define LEFT_ENC_B 13

// RIGHT ENCODER
#define RIGHT_ENC_A 3     // INT1
#define RIGHT_ENC_B A3


// =====================================================
//                 ULTRASONIC SENSORS
// =====================================================

// FRONT SENSOR
#define FRONT_TRIG 7
#define FRONT_ECHO A2

// LEFT SENSOR
#define LEFT_TRIG  A4
#define LEFT_ECHO  A5

// RIGHT SENSOR
#define RIGHT_TRIG 5
#define RIGHT_ECHO 12


#endif // CONFIG_H
