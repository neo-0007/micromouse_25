// ===== MOTOR + ENCODER TEST =====

// ---- Motor pins ----
#define ENA 10
#define IN1 8
#define IN2 11

#define ENB 6
#define IN3 A0
#define IN4 A1

// ---- Encoder pins ----
#define LEFT_ENC_A 2
#define LEFT_ENC_B 13

#define RIGHT_ENC_A 3
#define RIGHT_ENC_B A3

volatile long leftTicks = 0;
volatile long rightTicks = 0;

// ---- Encoder ISRs ----
void leftEncoderISR() {
  if (digitalRead(LEFT_ENC_B))
    leftTicks--;
  else
    leftTicks++;
}

void rightEncoderISR() {
  if (digitalRead(RIGHT_ENC_B))
    rightTicks++;
  else
    rightTicks--;
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

  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,HIGH);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,HIGH);

  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}


void setup() {
  Serial.begin(9600);

  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Encoder pins
  pinMode(LEFT_ENC_A, INPUT_PULLUP);
  pinMode(LEFT_ENC_B, INPUT_PULLUP);
  pinMode(RIGHT_ENC_A, INPUT_PULLUP);
  pinMode(RIGHT_ENC_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_A), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_A), rightEncoderISR, RISING);

  Serial.println("Motor + Encoder test started");
}

void loop() {
  leftTicks = 0;
  rightTicks = 0;

  Serial.println("Moving forward...");
  delay(2000);

  moveForward(255);
  delay(5000);

  stopMotors();

  Serial.print("Left ticks: ");
  Serial.print(leftTicks);
  Serial.print(" | Right ticks: ");
  Serial.println(rightTicks);

  Serial.println("------------------");
  delay(3000);
}
