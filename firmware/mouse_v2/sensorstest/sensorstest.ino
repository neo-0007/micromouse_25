// ---- SENSOR PINS ----

// FRONT SENSOR
#define FRONT_TRIG 7
#define FRONT_ECHO A2

// LEFT SENSOR
#define LEFT_TRIG A4
#define LEFT_ECHO A5

// RIGHT SENSOR
#define RIGHT_TRIG 6
#define RIGHT_ECHO 12


long readDistanceCM(int trigPin, int echoPin) {
  // Ensure clean trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10us pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo (timeout 30ms ≈ 5m)
  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) return -1;  // no echo

  return duration * 0.034 / 2;
}


long readFront() {
  return readDistanceCM(FRONT_TRIG, FRONT_ECHO);
}

long readLeft() {
  return readDistanceCM(LEFT_TRIG, LEFT_ECHO);
}

long readRight() {
  return readDistanceCM(RIGHT_TRIG, RIGHT_ECHO);
}


void setup() {
  Serial.begin(9600);

  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);

  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);

  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);

  digitalWrite(FRONT_TRIG, LOW);
  digitalWrite(LEFT_TRIG, LOW);
  digitalWrite(RIGHT_TRIG, LOW);

  Serial.println("=== HC-SR04 ALL SENSOR TEST ===");
}


void loop() {

  long leftDist = readLeft();
  delay(35);   // prevent ultrasonic crosstalk

  long frontDist = readFront();
  delay(35);

  long rightDist = readRight();
  delay(35);

  Serial.print("Left: ");
  Serial.println(leftDist);
  Serial.print(" cm | Front: ");
  Serial.print(frontDist);
  Serial.print(" cm | Right: ");
  Serial.println(rightDist);

  Serial.println("----------------------------");
  delay(300);
}
