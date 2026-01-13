#include "motors.h"
#include "encoders.h"
#include "motion.h"
#include "config.h"
#include "sensors.h"

// Create motor objects
Motor leftMotor(IN2, IN1, ENA);
Motor rightMotor(IN3, IN4, ENB);

// Create encoder objects
Encoder leftEncoder(LEFT_ENC_A, LEFT_ENC_B);
Encoder rightEncoder(RIGHT_ENC_A, RIGHT_ENC_B);

// Sensor objects
UltrasonicSensor frontSensor(FRONT_TRIG, FRONT_ECHO);
UltrasonicSensor leftSensor(LEFT_TRIG, LEFT_ECHO);
UltrasonicSensor rightSensor(RIGHT_TRIG, RIGHT_ECHO);

enum State {
  IDLE,
  FORWARD,
  TURNRIGHT,
  TURNLEFT,
  TURNAROUND,
  BACKWARD,
  COMPLETE,
  STOP
};

State currentState = IDLE;

// Base motor speed
int baseSpeed = 100;

// Current motor speeds
int leftMotorSpeed = baseSpeed;
int rightMotorSpeed = baseSpeed;

// Encoder counts
int leftEncoderCount = 0;
int rightEncoderCount = 0;

// PID constants
// Only P controller used for now , have to tune
const float Kp = 6;
const float Ki = 0.2;
const float Kd = 0.6;

const float Kp_turn = 2.2;
const int TURN_BASE_SPEED = 120;

const int MAX_SPEED = 255;
const int MIN_SPEED = 100;

void printState(const char* msg) {
  Serial.print("[STATE] ");
  Serial.println(msg);
}

void setup() {
  Serial.begin(9600);

  leftMotor.init();
  rightMotor.init();

  leftEncoder.init();
  rightEncoder.init();

  fronSensor.init();
  leftSensor.init();
  rightSensor.init();

  leftEncoder.reset();
  rightEncoder.reset();

  delay(100); 

  currentState = FORWARD;
  Serial.println("[INIT] Starting in FORWARD state");
}

void loop() {

  switch (currentState) {

    case FORWARD:
      printState("FORWARD");
      moveForward();
      break;

    case TURNLEFT:
      printState("TURNLEFT");
      turnLeft90();
      break;

    case TURNRIGHT:
      printState("TURNRIGHT");
      turnRight90();
      break;

    case TURNAROUND:
      printState("TURNAROUND");
      turn180();
      break;

    case BACKWARD:
      printState("BACKWARD");
      moveBackwardShort();
      break;

    case STOP:
    case COMPLETE:
    default:
      printState("STOP");
      leftMotor.stop();
      rightMotor.stop();
      break;
  }
}


void moveForward() {

  int frontDist = fronSensor.getDistance();
  delay(35);
  int leftDist  = leftSensor.getDistance();
  delay(35);
  int rightDist = rightSensor.getDistance();
  delay(35);

  Serial.print("[SENSORS] F:");
  Serial.print(frontDist);
  Serial.print(" L:");
  Serial.print(leftDist);
  Serial.print(" R:");
  Serial.println(rightDist);

  leftEncoderCount = leftEncoder.getTicks();
  rightEncoderCount = rightEncoder.getTicks();

  Serial.print("[ENC] L:");
  Serial.print(leftEncoderCount);
  Serial.print(" R:");
  Serial.println(rightEncoderCount);

  int error = leftEncoderCount - rightEncoderCount;
  int correction = error * Kp;

  leftMotorSpeed  = baseSpeed - correction;
  rightMotorSpeed = baseSpeed + correction;

  leftMotorSpeed  = constrain(leftMotorSpeed,  MIN_SPEED, MAX_SPEED);
  rightMotorSpeed = constrain(rightMotorSpeed, MIN_SPEED, MAX_SPEED);

  leftMotor.moveForward(leftMotorSpeed);
  rightMotor.moveForward(rightMotorSpeed);

  if (frontDist <= FRONT_WALL_DIST  && frontDist > 0) {

    Serial.println("[EVENT] Front wall detected");

    leftMotor.stop();
    rightMotor.stop();

    if (leftDist <= SIDE_WALL_DIST && leftDist > 0 &&
        rightDist <= SIDE_WALL_DIST && rightDist > 0 ) {

      Serial.println("[DECISION] Dead end STOP");
      currentState = COMPLETE;
      return;
    }

    leftEncoder.reset();
    rightEncoder.reset();

    if (leftDist > SIDE_WALL_DIST && leftDist > 0 ) {
      Serial.println("[DECISION] Turn LEFT");
      currentState = TURNLEFT;
    }
    else if (rightDist > SIDE_WALL_DIST rightDist > 0 ) {
      Serial.println("[DECISION] Turn RIGHT");
      currentState = TURNRIGHT;
    }
    else {
      Serial.println("[DECISION] Turn AROUND");
      currentState = TURNAROUND;
    }
  }
}



void turnRight90() {

  leftEncoderCount  = leftEncoder.getTicks();
  rightEncoderCount = rightEncoder.getTicks();

  Serial.print("[TR90 ENC] L:");
  Serial.print(leftEncoderCount);
  Serial.print(" R:");
  Serial.println(rightEncoderCount);

  int leftTicks  = abs(leftEncoderCount);
  int rightTicks = abs(rightEncoderCount);

  int error = rightTicks - leftTicks;
  int correction = error * Kp_turn;

  int leftSpeed  = TURN_BASE_SPEED + correction;
  int rightSpeed = TURN_BASE_SPEED - correction;

  leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

  leftMotor.moveForward(leftSpeed);
  rightMotor.moveBackward(rightSpeed);

  if (rightTicks >= TURN_90_TICKS) {
    Serial.println("[EVENT] Right turn complete");
    leftMotor.stop();
    rightMotor.stop();
    leftEncoder.reset();
    rightEncoder.reset();
    currentState = BACKWARD;
  }
}

void turnLeft90() {

  leftEncoderCount  = leftEncoder.getTicks();
  rightEncoderCount = rightEncoder.getTicks();

  Serial.print("[TL90 ENC] L:");
  Serial.print(leftEncoderCount);
  Serial.print(" R:");
  Serial.println(rightEncoderCount);

  int leftTicks  = abs(leftEncoderCount);
  int rightTicks = abs(rightEncoderCount);

  int error = leftTicks - rightTicks;
  int correction = error * Kp_turn;

  int leftSpeed  = TURN_BASE_SPEED - correction;
  int rightSpeed = TURN_BASE_SPEED + correction;

  leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

  leftMotor.moveBackward(leftSpeed);
  rightMotor.moveForward(rightSpeed);

  if (leftTicks >= TURN_90_TICKS) {
    Serial.println("[EVENT] Left turn complete");
    leftMotor.stop();
    rightMotor.stop();
    leftEncoder.reset();
    rightEncoder.reset();
    currentState = BACKWARD;
  }
}

void turn180() {

  leftEncoderCount  = leftEncoder.getTicks();
  rightEncoderCount = rightEncoder.getTicks();

  Serial.print("[T180 ENC] L:");
  Serial.print(leftEncoderCount);
  Serial.print(" R:");
  Serial.println(rightEncoderCount);

  int leftTicks  = abs(leftEncoderCount);
  int rightTicks = abs(rightEncoderCount);

  int error = leftTicks - rightTicks;
  int correction = error * Kp_turn;

  int leftSpeed  = TURN_BASE_SPEED - correction;
  int rightSpeed = TURN_BASE_SPEED + correction;

  leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

  leftMotor.moveForward(leftSpeed);
  rightMotor.moveBackward(rightSpeed);

  if (leftTicks >= TURN_180_TICKS) {
    Serial.println("[EVENT] Turn-around complete");
    leftMotor.stop();
    rightMotor.stop();
    leftEncoder.reset();
    rightEncoder.reset();
    currentState = BACKWARD;
  }
}

void moveBackwardShort() {

  leftEncoderCount  = abs(leftEncoder.getTicks());
  rightEncoderCount = abs(rightEncoder.getTicks());

  Serial.print("[BACK ENC] L:");
  Serial.print(leftEncoderCount);
  Serial.print(" R:");
  Serial.println(rightEncoderCount);

  leftMotor.moveBackward(baseSpeed);
  rightMotor.moveBackward(baseSpeed);

  if (leftEncoderCount >= BACK_TICKS ||
      rightEncoderCount >= BACK_TICKS) {

    Serial.println("[EVENT] Backward correction complete");
    leftMotor.stop();
    rightMotor.stop();
    leftEncoder.reset();
    rightEncoder.reset();
    currentState = FORWARD;
  }
}
