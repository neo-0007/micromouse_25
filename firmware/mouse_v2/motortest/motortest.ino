
// LEFT MOTOR
#define ENA 10
#define IN1 8      // LEFT BACK
#define IN2 11     // LEFT FRONT

// RIGHT MOTOR
#define ENB 9
#define IN3 A0     // RIGHT FRONT
#define IN4 A1     // RIGHT BACKBACK


void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void moveForward(int speedPWM) {
  // Left motor forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedPWM);
  analogWrite(ENB, speedPWM);
}


void moveBackward(int speedPWM) {
  // Left motor backward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, speedPWM);
  analogWrite(ENB, speedPWM);
}

void moveLeft(int speedPWM) {
  // Left motor backward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedPWM);
  analogWrite(ENB, speedPWM);
}

void moveRight(int speedPWM) {
  // Left motor forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, speedPWM);
  analogWrite(ENB, speedPWM);
}

void stopMotors() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}



void loop() {
  moveForward(150);
  delay(3000);

  stopMotors();
  delay(1000);

  moveBackward(150);
  delay(3000);

  stopMotors();
  delay(1000);

  moveLeft(150);
  delay(2000);

  stopMotors();
  delay(1000);

  moveRight(150);
  delay(2000);

  stopMotors();
  delay(2000);
}
