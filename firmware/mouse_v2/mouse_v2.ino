#include "motors.h"
#include "encoders.h"
#include "motion.h"
#include "config.h"
#include "sensors.h"

Motor leftMotor(IN2, IN1, ENA);
Motor rightMotor(IN3, IN4, ENB);

Encoder leftEncoder(LEFT_ENC_A, LEFT_ENC_B);
Encoder rightEncoder(RIGHT_ENC_A, RIGHT_ENC_B);

UltrasonicSensor frontSensor(FRONT_TRIG, FRONT_ECHO);
UltrasonicSensor leftSensor(LEFT_TRIG, LEFT_ECHO);
UltrasonicSensor rightSensor(RIGHT_TRIG, RIGHT_ECHO);


int baseSpeed = 200;
int baseSpeedBack = 80;

int leftEncoderCount = 0;
int rightEncoderCount = 0;

int frontDist, leftDist, rightDist;

// ---- Straight motion control ----
const float Kp = 17.0;
const float Ki = 0.08;
const float Kd = 0.8;

int prevError = 0;
int integralError = 0;
const int INTEGRAL_MAX = 150;

const float RIGHT_MOTOR_SCALE = 1.4;

// ---- Turn control ----
const float Kp_turn = 2.2;
const int TURN_BASE_SPEED = 120;

const int MAX_SPEED = 255;
const int MIN_SPEED = 100;


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

void loop()
{
  moveUntilWall();
//simpleMoveForward(); 
}

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
//    Serial.print("Error: "); Serial.print(error);
//    Serial.print(" | L: "); Serial.print(leftSpeed);
//    Serial.print(" | R: "); Serial.print(rightSpeed);
//    Serial.print(" | Front: "); Serial.println(frontDist);

    // Stop at wall
    if (frontDist > 0 && frontDist <= FRONT_WALL_DIST)
    {
      leftMotor.stop();
      rightMotor.stop();
      
      //Serial.println("WALL DETECTED - STOPPED");
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


void moveUntilWall()
{
  while (true)
  {
    frontDist = frontSensor.getDistance();

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
      }else if (rightDist >= SIDE_WALL_DIST && leftDist >= SIDE_WALL_DIST ){
        turnRight90();
        prevError = 0;
        integralError = 0;  // Reset integral after turn
        return;      
      }
    }

    delay(5);
  }
}


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
  const unsigned long BACKUP_DURATION = 550;  // 2 seconds

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
