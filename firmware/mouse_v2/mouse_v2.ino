//#include "motors.h"
//#include "encoders.h"
//#include "motion.h"
//#include "config.h"
//#include "sensors.h"
//
//// Create motor objects
//Motor leftMotor(IN2, IN1, ENA);
//Motor rightMotor(IN3, IN4, ENB);
//
//// Create encoder objects
//Encoder leftEncoder(LEFT_ENC_A, LEFT_ENC_B);
//Encoder rightEncoder(RIGHT_ENC_A, RIGHT_ENC_B);
//
//// Sensor objects
//UltrasonicSensor frontSensor(FRONT_TRIG, FRONT_ECHO);
//UltrasonicSensor leftSensor(LEFT_TRIG, LEFT_ECHO);
//UltrasonicSensor rightSensor(RIGHT_TRIG, RIGHT_ECHO);
//
//enum State {
//  IDLE,
//  FORWARD,
//  TURNRIGHT,
//  TURNLEFT,
//  TURNAROUND,
//  BACKWARD,
//  COMPLETE,
//  STOP
//};
//
//State currentState = IDLE;
//
//// Base motor speed
//int baseSpeed = 100;
//
//// Current motor speeds
//int leftMotorSpeed = baseSpeed;
//int rightMotorSpeed = baseSpeed;
//
//// Encoder counts
//int leftEncoderCount = 0;
//int rightEncoderCount = 0;
//
//// PID constants
//// Only P controller used for now , have to tune
//const float Kp = 6;
//const float Ki = 0.2;
//const float Kd = 0.6;
//
//const float Kp_turn = 2.2;
//const int TURN_BASE_SPEED = 120;
//
//const int MAX_SPEED = 255;
//const int MIN_SPEED = 100;
//
//void printState(const char* msg) {
//  Serial.print("[STATE] ");
//  Serial.println(msg);
//}
//
//void setup() {
//  Serial.begin(9600);
//
//  leftMotor.init();
//  rightMotor.init();
//
//  leftEncoder.init();
//  rightEncoder.init();
//
//  frontSensor.init();
//  leftSensor.init();
//  rightSensor.init();
//
//  leftEncoder.reset();
//  rightEncoder.reset();
//
//  delay(100); 
//
//  currentState = FORWARD;
//  Serial.println("[INIT] Starting in FORWARD state");
//}
//
//void loop() {
//
//  switch (currentState) {
//
//    case FORWARD:
//      printState("FORWARD");
//      moveForward();
//      break;
//
//    case TURNLEFT:
//      printState("TURNLEFT");
//      turnLeft90();
//      break;
//
//    case TURNRIGHT:
//      printState("TURNRIGHT");
//      turnRight90();
//      break;
//
//    case TURNAROUND:
//      printState("TURNAROUND");
//      turn180();
//      break;
//
//    case BACKWARD:
//      printState("BACKWARD");
//      moveBackwardShort();
//      break;
//
//    case STOP:
//    case COMPLETE:
//    default:
//      printState("STOP");
//      leftMotor.stop();
//      rightMotor.stop();
//      break;
//  }
//}
//
//
//void moveForward() {
//
//  int frontDist = frontSensor.getDistance();
//  delay(35);
//  int leftDist  = leftSensor.getDistance();
//  delay(35);
//  int rightDist = rightSensor.getDistance();
//  delay(35);
//
//  Serial.print("[SENSORS] F:");
//  Serial.print(frontDist);
//  Serial.print(" L:");
//  Serial.print(leftDist);
//  Serial.print(" R:");
//  Serial.println(rightDist);
//
//  leftEncoderCount = leftEncoder.getTicks();
//  rightEncoderCount = rightEncoder.getTicks();
//
//  Serial.print("[ENC] L:");
//  Serial.print(leftEncoderCount);
//  Serial.print(" R:");
//  Serial.println(rightEncoderCount);
//
//  int error = leftEncoderCount - rightEncoderCount;
//  int correction = error * Kp;
//
//  leftMotorSpeed  = baseSpeed - correction;
//  rightMotorSpeed = baseSpeed + correction;
//
//  leftMotorSpeed  = constrain(leftMotorSpeed,  MIN_SPEED, MAX_SPEED);
//  rightMotorSpeed = constrain(rightMotorSpeed, MIN_SPEED, MAX_SPEED);
//
//  leftMotor.moveForward(leftMotorSpeed);
//  rightMotor.moveForward(rightMotorSpeed);
//
//  if (frontDist <= FRONT_WALL_DIST  && frontDist > 0) {
//
//    Serial.println("[EVENT] Front wall detected");
//
//    leftMotor.stop();
//    rightMotor.stop();
//
//    leftDist  = leftSensor.getDistance();
//    delay(35);
//    rightDist = rightSensor.getDistance();
//
//    if (leftDist <= SIDE_WALL_DIST && leftDist > 0 &&
//        rightDist <= SIDE_WALL_DIST && rightDist > 0 ) {
//
//      Serial.println("[DECISION] Dead end STOP");
//      currentState = COMPLETE;
//      return;
//    }
//
//    leftEncoder.reset();
//    rightEncoder.reset();
//
//    if (leftDist < SIDE_WALL_DIST && leftDist > 0 ) {
//      Serial.println("[DECISION] Turn RIGHT");
//      currentState = TURNRIGHT;
//    }
//    else if (rightDist < SIDE_WALL_DIST && rightDist > 0 ) {
//      Serial.println("[DECISION] Turn LEFT");
//      currentState = TURNLEFT;
//    }
//    else {
//      Serial.println("[DECISION] Turn AROUND");
//      currentState = TURNAROUND;
//    }
//  }
//}
//
//
//
//void turnRight90() {
//
//  leftEncoderCount  = leftEncoder.getTicks();
//  rightEncoderCount = rightEncoder.getTicks();
//
//  Serial.print("[TR90 ENC] L:");
//  Serial.print(leftEncoderCount);
//  Serial.print(" R:");
//  Serial.println(rightEncoderCount);
//
//  int leftTicks  = abs(leftEncoderCount);
//  int rightTicks = abs(rightEncoderCount);
//
//  int error = rightTicks - leftTicks;
//  int correction = error * Kp_turn;
//
//  int leftSpeed  = TURN_BASE_SPEED + correction;
//  int rightSpeed = TURN_BASE_SPEED - correction;
//
//  leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
//  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
//
//  leftMotor.moveForward(leftSpeed);
//  rightMotor.moveBackward(rightSpeed);
//
//  if (rightTicks >= TURN_90_TICKS) {
//    Serial.println("[EVENT] Right turn complete");
//    leftMotor.stop();
//    rightMotor.stop();
//    leftEncoder.reset();
//    rightEncoder.reset();
//    currentState = BACKWARD;
//  }
//}
//
//void turnLeft90() {
//
//  leftEncoderCount  = leftEncoder.getTicks();
//  rightEncoderCount = rightEncoder.getTicks();
//
//  Serial.print("[TL90 ENC] L:");
//  Serial.print(leftEncoderCount);
//  Serial.print(" R:");
//  Serial.println(rightEncoderCount);
//
//  int leftTicks  = abs(leftEncoderCount);
//  int rightTicks = abs(rightEncoderCount);
//
//  int error = leftTicks - rightTicks;
//  int correction = error * Kp_turn;
//
//  int leftSpeed  = TURN_BASE_SPEED - correction;
//  int rightSpeed = TURN_BASE_SPEED + correction;
//
//  leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
//  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
//
//  leftMotor.moveBackward(leftSpeed);
//  rightMotor.moveForward(rightSpeed);
//
//  if (leftTicks >= TURN_90_TICKS) {
//    Serial.println("[EVENT] Left turn complete");
//    leftMotor.stop();
//    rightMotor.stop();
//    leftEncoder.reset();
//    rightEncoder.reset();
//    currentState = BACKWARD;
//  }
//}
//
//void turn180() {
//
//  leftEncoderCount  = leftEncoder.getTicks();
//  rightEncoderCount = rightEncoder.getTicks();
//
//  Serial.print("[T180 ENC] L:");
//  Serial.print(leftEncoderCount);
//  Serial.print(" R:");
//  Serial.println(rightEncoderCount);
//
//  int leftTicks  = abs(leftEncoderCount);
//  int rightTicks = abs(rightEncoderCount);
//
//  int error = leftTicks - rightTicks;
//  int correction = error * Kp_turn;
//
//  int leftSpeed  = TURN_BASE_SPEED - correction;
//  int rightSpeed = TURN_BASE_SPEED + correction;
//
//  leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
//  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
//
//  leftMotor.moveForward(leftSpeed);
//  rightMotor.moveBackward(rightSpeed);
//
//  if (leftTicks >= TURN_180_TICKS) {
//    Serial.println("[EVENT] Turn-around complete");
//    leftMotor.stop();
//    rightMotor.stop();
//    leftEncoder.reset();
//    rightEncoder.reset();
//    currentState = BACKWARD;
//  }
//}
//
//void moveBackwardShort() {
//
//  leftEncoderCount  = abs(leftEncoder.getTicks());
//  rightEncoderCount = abs(rightEncoder.getTicks());
//
//  Serial.print("[BACK ENC] L:");
//  Serial.print(leftEncoderCount);
//  Serial.print(" R:");
//  Serial.println(rightEncoderCount);
//
//  leftMotor.moveBackward(baseSpeed);
//  rightMotor.moveBackward(baseSpeed);
//
//  if (leftEncoderCount >= BACK_TICKS ||
//      rightEncoderCount >= BACK_TICKS) {
//
//    Serial.println("[EVENT] Backward correction complete");
//    leftMotor.stop();
//    rightMotor.stop();
//    leftEncoder.reset();
//    rightEncoder.reset();
//    currentState = FORWARD;
//  }
//}




//////// TUNER CODE


//#include "motors.h"
//#include "encoders.h"
//#include "config.h"
//#include "sensors.h"
//
//// Create motor objects
//Motor leftMotor(IN2, IN1, ENA);
//Motor rightMotor(IN3, IN4, ENB);
//
//// Create encoder objects
//Encoder leftEncoder(LEFT_ENC_A, LEFT_ENC_B);
//Encoder rightEncoder(RIGHT_ENC_A, RIGHT_ENC_B);
//
//// Create sensor objects
//UltrasonicSensor frontSensor(FRONT_TRIG, FRONT_ECHO);
//
//// Test parameters - ADJUST THESE VALUES
//int TEST_TURN_TICKS = 200;      // Change this to tune your turns
//int TEST_BACK_TICKS = 50;       // Change this to tune backward distance
//
//// PID constants
//const float Kp_turn = 2.2;
//const int TURN_BASE_SPEED = 120;
//const int MAX_SPEED = 255;
//const int MIN_SPEED = 100;
//
//// Encoder counts
//int leftEncoderCount = 0;
//int rightEncoderCount = 0;
//
//bool testComplete = false;
//
//void setup() {
//  leftMotor.init();
//  rightMotor.init();
//  
//  leftEncoder.init();
//  rightEncoder.init();
//  
//  frontSensor.init();
//  
//  leftEncoder.reset();
//  rightEncoder.reset();
//  
//  delay(2000);  // 2 second delay before starting - time to place robot
//  
//  // ===== UNCOMMENT ONE TEST AT A TIME =====
//  
//  // testRightTurn90();
//  // testLeftTurn90();
//  // testTurn180();
//  // testForwardDistance();
//  // testBackwardDistance();
//   testWallStopDistance();
//  
//  // ========================================
//}
//
//void loop() {
//  // Robot stops after test completes
//  if (testComplete) {
//    leftMotor.stop();
//    rightMotor.stop();
//  }
//}
//
//// ===== TEST FUNCTIONS =====
//
//void testRightTurn90() {
//  leftEncoder.reset();
//  rightEncoder.reset();
//  
//  while (!testComplete) {
//    leftEncoderCount = abs(leftEncoder.getTicks());
//    rightEncoderCount = abs(rightEncoder.getTicks());
//    
//    int error = rightEncoderCount - leftEncoderCount;
//    int correction = error * Kp_turn;
//    
//    int leftSpeed = TURN_BASE_SPEED + correction;
//    int rightSpeed = TURN_BASE_SPEED - correction;
//    
//    leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
//    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
//    
//    leftMotor.moveForward(leftSpeed);
//    rightMotor.moveBackward(rightSpeed);
//    
//    if (rightEncoderCount >= TEST_TURN_TICKS) {
//      testComplete = true;
//      leftMotor.stop();
//      rightMotor.stop();
//    }
//    
//    delay(10);
//  }
//}
//
//void testLeftTurn90() {
//  leftEncoder.reset();
//  rightEncoder.reset();
//  
//  while (!testComplete) {
//    leftEncoderCount = abs(leftEncoder.getTicks());
//    rightEncoderCount = abs(rightEncoder.getTicks());
//    
//    int error = leftEncoderCount - rightEncoderCount;
//    int correction = error * Kp_turn;
//    
//    int leftSpeed = TURN_BASE_SPEED - correction;
//    int rightSpeed = TURN_BASE_SPEED + correction;
//    
//    leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
//    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
//    
//    leftMotor.moveBackward(leftSpeed);
//    rightMotor.moveForward(rightSpeed);
//    
//    if (leftEncoderCount >= TEST_TURN_TICKS) {
//      testComplete = true;
//      leftMotor.stop();
//      rightMotor.stop();
//    }
//    
//    delay(10);
//  }
//}
//
//void testTurn180() {
//  leftEncoder.reset();
//  rightEncoder.reset();
//  
//  while (!testComplete) {
//    leftEncoderCount = abs(leftEncoder.getTicks());
//    rightEncoderCount = abs(rightEncoder.getTicks());
//    
//    int error = leftEncoderCount - rightEncoderCount;
//    int correction = error * Kp_turn;
//    
//    int leftSpeed = TURN_BASE_SPEED - correction;
//    int rightSpeed = TURN_BASE_SPEED + correction;
//    
//    leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
//    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
//    
//    leftMotor.moveForward(leftSpeed);
//    rightMotor.moveBackward(rightSpeed);
//    
//    if (leftEncoderCount >= TEST_TURN_TICKS) {
//      testComplete = true;
//      leftMotor.stop();
//      rightMotor.stop();
//    }
//    
//    delay(10);
//  }
//}
//
//void testForwardDistance() {
//  leftEncoder.reset();
//  rightEncoder.reset();
//  
//  int baseSpeed = 120;
//  
//  while (!testComplete) {
//    leftEncoderCount = abs(leftEncoder.getTicks());
//    rightEncoderCount = abs(rightEncoder.getTicks());
//    
//    leftMotor.moveForward(baseSpeed);
//    rightMotor.moveForward(baseSpeed);
//    
//    if (leftEncoderCount >= TEST_TURN_TICKS || 
//        rightEncoderCount >= TEST_TURN_TICKS) {
//      testComplete = true;
//      leftMotor.stop();
//      rightMotor.stop();
//    }
//    
//    delay(10);
//  }
//}
//
//void testBackwardDistance() {
//  leftEncoder.reset();
//  rightEncoder.reset();
//  
//  int baseSpeed = 120;
//  
//  while (!testComplete) {
//    leftEncoderCount = abs(leftEncoder.getTicks());
//    rightEncoderCount = abs(rightEncoder.getTicks());
//    
//    leftMotor.moveBackward(baseSpeed);
//    rightMotor.moveBackward(baseSpeed);
//    
//    if (leftEncoderCount >= TEST_BACK_TICKS || 
//        rightEncoderCount >= TEST_BACK_TICKS) {
//      testComplete = true;
//      leftMotor.stop();
//      rightMotor.stop();
//    }
//    
//    delay(10);
//  }
//}
//
//// ===== HELPER FUNCTIONS FOR MANUAL TESTING =====
//
//void stopMotors() {
//  leftMotor.stop();
//  rightMotor.stop();
//}
//
//void resetEncoders() {
//  leftEncoder.reset();
//  rightEncoder.reset();
//}
//
//void testWallStopDistance() {
//  // This test drives forward until it detects a wall at FRONT_WALL_DIST
//  // Place robot facing a wall and measure actual stopping distance
//  
//  leftEncoder.reset();
//  rightEncoder.reset();
//  
//  int baseSpeed = 100;  // Use same speed as your main code
//  const float Kp = 6;   // Use same PID as your main code
//  
//  while (!testComplete) {
//    int frontDist = frontSensor.getDistance();
//    delay(35);
//    
//    leftEncoderCount = leftEncoder.getTicks();
//    rightEncoderCount = rightEncoder.getTicks();
//    
//    // PID correction to go straight
//    int error = leftEncoderCount - rightEncoderCount;
//    int correction = error * Kp;
//    
//    int leftSpeed = baseSpeed - correction;
//    int rightSpeed = baseSpeed + correction;
//    
//    leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
//    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
//    
//    leftMotor.moveForward(leftSpeed);
//    rightMotor.moveForward(rightSpeed);
//    
//    // Stop when wall detected at FRONT_WALL_DIST
//    if (frontDist <= FRONT_WALL_DIST && frontDist > 0) {
//      testComplete = true;
//      leftMotor.stop();
//      rightMotor.stop();
//      // Now measure actual distance from wall!
//      // If too close, increase FRONT_WALL_DIST in config.h
//      // If too far, decrease FRONT_WALL_DIST in config.h
//    }
//    
//    delay(10);
//  }
//}


#include "motors.h"
#include "encoders.h"
#include "motion.h"
#include "config.h"
#include "sensors.h"

// ================= OBJECTS =================
Motor leftMotor(IN2, IN1, ENA);
Motor rightMotor(IN3, IN4, ENB);

Encoder leftEncoder(LEFT_ENC_A, LEFT_ENC_B);
Encoder rightEncoder(RIGHT_ENC_A, RIGHT_ENC_B);

UltrasonicSensor frontSensor(FRONT_TRIG, FRONT_ECHO);
UltrasonicSensor leftSensor(LEFT_TRIG, LEFT_ECHO);
UltrasonicSensor rightSensor(RIGHT_TRIG, RIGHT_ECHO);

// ================= PARAMETERS =================
int baseSpeed = 100;

int leftEncoderCount = 0;
int rightEncoderCount = 0;

int frontDist, leftDist, rightDist;

// ---- Straight motion control ----
const float Kp = 17.0;    // Tune this first
const float Ki = 0.08;    // Then this
const float Kd = 0.8;    // Then this

int prevError = 0;
int integralError = 0;
const int INTEGRAL_MAX = 150;

const float RIGHT_MOTOR_SCALE = 1.4;

// ---- Turn control ----
const float Kp_turn = 2.2;
const int TURN_BASE_SPEED = 120;

const int MAX_SPEED = 255;
const int MIN_SPEED = 100;


// ================= SETUP =================
void setup()
{
  Serial.begin(9600);

  leftMotor.init();
  rightMotor.init();

  leftEncoder.init();
  rightEncoder.init();

  frontSensor.init();
  leftSensor.init();
  rightSensor.init();

  leftEncoder.reset();
  rightEncoder.reset();
}

// ================= LOOP =================
void loop()
{
  moveUntilWall();
//simpleMoveForward(); 
}

// ================= SIMPLE FORWARD (FOR TUNING) =================
void simpleMoveForward()
{
  leftEncoder.reset();
  rightEncoder.reset();
  prevError = 0;
  integralError = 0;

  while (true)
  {
    frontDist = frontSensor.getDistance();

    // Read encoders
    int leftTicks  = leftEncoder.getTicks();
    int rightTicks = rightEncoder.getTicks();

    // Calculate error
    int error = leftTicks - rightTicks;

    // Integral term (with windup protection)
    integralError += error;
//    integralError = constrain(integralError, -INTEGRAL_MAX, INTEGRAL_MAX);

    // Derivative term
    int derivative = error - prevError;
    prevError = error;

    // PID correction
    int correction = (Kp * error) + (Ki * integralError) + (Kd * derivative);

    // Apply correction
    int leftSpeed  = baseSpeed - correction;
    int rightSpeed = (baseSpeed + correction) * RIGHT_MOTOR_SCALE;

    // Constrain speeds
    leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

    // Move forward
    leftMotor.moveForward(leftSpeed);
    rightMotor.moveForward(rightSpeed);

    // Debug output
    Serial.print("Error: "); Serial.print(error);
    Serial.print(" | L: "); Serial.print(leftSpeed);
    Serial.print(" | R: "); Serial.print(rightSpeed);
    Serial.print(" | Front: "); Serial.println(frontDist);

    // Stop at wall
    if (frontDist > 0 && frontDist <= FRONT_WALL_DIST)
    {
      leftMotor.stop();
      rightMotor.stop();
      
      Serial.println("WALL DETECTED - STOPPED");
      delay(5000);  // Wait 5 seconds
      
      // Reset and continue
      leftEncoder.reset();
      rightEncoder.reset();
      prevError = 0;
      integralError = 0;
    }

    delay(5);
  }
}

// ================= STRAIGHT MOTION =================
void moveUntilWall()
{
  while (true)
  {
    frontDist = frontSensor.getDistance();

    // YOUR WORKING STRAIGHT-LINE LOGIC WITH INTEGRAL
    leftEncoderCount  = leftEncoder.getTicks();
    rightEncoderCount = rightEncoder.getTicks();

    int error = leftEncoderCount - rightEncoderCount;

    // Integral term - accumulate error
    integralError += error;

    // Derivative damping (does NOT destabilize)
    int derivative = error - prevError;
    prevError = error;

    // PID correction with integral term
    int correction = (Kp * error) + (Ki * integralError) + (Kd * derivative);

    int leftSpeed  = baseSpeed - correction;
    int rightSpeed = (baseSpeed + correction) * RIGHT_MOTOR_SCALE;

    leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

    leftMotor.moveForward(leftSpeed);
    rightMotor.moveForward(rightSpeed);

    // ---- WALL DETECT ----
    if (frontDist > 0 && frontDist <= FRONT_WALL_DIST)
    {
      leftMotor.stop();
      rightMotor.stop();
      delay(50);

      leftDist  = leftSensor.getDistance();
      rightDist = rightSensor.getDistance();

      delay(100);

      leftEncoder.reset();
      rightEncoder.reset();
      prevError = 0;
      integralError = 0;  // Reset integral when stopping

      if (rightDist > 0 && rightDist <= SIDE_WALL_DIST && leftDist > 0 && leftDist <= SIDE_WALL_DIST)
      {
        return;
      }
      else if (leftDist > 0 && leftDist <= SIDE_WALL_DIST)
      {
        turnRight90();
        moveBackwardShort();
        prevError = 0;
        integralError = 0;  // Reset integral after turn
        return;
      }
      else if (rightDist > 0 && rightDist <= SIDE_WALL_DIST)
      {
        turnLeft90();
        moveBackwardShort();
        prevError = 0;
        integralError = 0;  // Reset integral after turn
        return;
      }else{
         return;  
      }
    }

    delay(5);
  }
}

// ================= RIGHT TURN =================
// ================= RIGHT TURN =================
void turnRight90()
{
  // IMPORTANT: Stop and reset BEFORE starting turn
  leftMotor.stop();
  rightMotor.stop();
  delay(100);
  
  leftEncoder.reset();
  rightEncoder.reset();
  delay(50);  // Give encoders time to reset
  
  while (true)
  {
    int leftTicks  = abs(leftEncoder.getTicks());
    int rightTicks = abs(rightEncoder.getTicks());

    // For right turn: left motor forward, right motor backward
    int error = leftTicks - rightTicks;
    int correction = error * Kp_turn;

    int leftSpeed  = TURN_BASE_SPEED - correction;
    int rightSpeed = TURN_BASE_SPEED + correction;

    leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

    leftMotor.moveForward(leftSpeed);
    rightMotor.moveBackward(rightSpeed);

    // Exit when LEFT motor reaches target (it's the one moving forward)
    if (leftTicks >= LEFT_TURN_90_TICKS)
    {
      leftMotor.stop();
      rightMotor.stop();
      delay(200);  // longer settling delay
      leftEncoder.reset();
      rightEncoder.reset();
      return;
    }
    
    delay(5);
  }
}

// ================= LEFT TURN =================
void turnLeft90()
{
  // IMPORTANT: Stop and reset BEFORE starting turn
  leftMotor.stop();
  rightMotor.stop();
  delay(100);
  
  leftEncoder.reset();
  rightEncoder.reset();
  delay(50);  // Give encoders time to reset
  
  while (true)
  {
    int leftTicks  = abs(leftEncoder.getTicks());
    int rightTicks = abs(rightEncoder.getTicks());

    // For left turn: right motor forward, left motor backward
    int error = rightTicks - leftTicks;
    int correction = error * Kp_turn;

    int leftSpeed  = TURN_BASE_SPEED + correction;
    int rightSpeed = TURN_BASE_SPEED - correction;

    leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

    leftMotor.moveBackward(leftSpeed);
    rightMotor.moveForward(rightSpeed);

    // Exit when RIGHT motor reaches target (it's the one moving forward)
    if (rightTicks >= RIGHT_TURN_90_TICKS)
    {
      leftMotor.stop();
      rightMotor.stop();
      delay(200);  // longer settling delay
      leftEncoder.reset();
      rightEncoder.reset();
      return;
    }
    
    delay(5);
  }
}


void moveBackwardShort()
{
  leftEncoder.reset();
  rightEncoder.reset();
  prevError = 0;  // Reset previous error for derivative

  unsigned long startTime = millis();
  const unsigned long BACKUP_DURATION = 750;  // 2 seconds

  while (millis() - startTime < BACKUP_DURATION)
  {
    // Read encoder counts
    leftEncoderCount  = abs(leftEncoder.getTicks());
    rightEncoderCount = abs(rightEncoder.getTicks());

    // Calculate error (difference between encoders)
    int error = leftEncoderCount - rightEncoderCount;

    // Derivative term for damping
    int derivative = error - prevError;
    prevError = error;

    // PID correction (same gains as forward motion)
    int correction = (Kp * error) + (Kd * derivative);

    // Apply correction to speeds
    int leftSpeed  = baseSpeed - correction;
    int rightSpeed = baseSpeed + correction;

    // Constrain speeds to safe limits
    leftSpeed  = constrain(leftSpeed,  MIN_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

    // Move backward with corrected speeds
    leftMotor.moveBackward(leftSpeed);
    rightMotor.moveBackward(rightSpeed);

    delay(5);  // Small delay for stability
  }

  // Stop motors
  leftMotor.stop();
  rightMotor.stop();
  
  // Reset encoders and error
  leftEncoder.reset();
  rightEncoder.reset();
  prevError = 0;
}
