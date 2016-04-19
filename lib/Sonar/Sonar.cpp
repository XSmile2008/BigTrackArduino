#include "Sonar.h"
#include "CircularBuffer.cpp"

CircularBuffer<SonarData>* datas = new CircularBuffer<SonarData>(20);;

int8_t diraction = 1;
int16_t currAngle = 0;

uint32_t lastStepTime = 0;

Sonar::Sonar(uint8_t servoPin, uint8_t trig0, uint8_t echo0, uint8_t trig1, uint8_t echo1) {
  sonar0 = new NewPing(trig0, echo0, MAX_SCAN_DISTANCE);
  sonar0 = new NewPing(trig1, echo1, MAX_SCAN_DISTANCE);
  servo.attach(servoPin);
  currAngle = ANGLE_MIN;
}

void Sonar::task() {
  if (millis() >= lastStepTime + PAUSE_STEP) {
    step();
  }
}

void Sonar::step() {
  lastStepTime = millis();

  datas->put(scan(sonar0));
  delay(PAUSE_SCAN);
  datas->put(scan(sonar1));
  delay(PAUSE_SCAN);

  currAngle = currAngle + ANGLE_STEP * diraction;
  if (currAngle > ANGLE_MAX) {
    diraction = -diraction;
    currAngle = ANGLE_MAX;
    Serial.print("free mem: ");Serial.println(freeMemory());
  } else if (currAngle < ANGLE_MIN) {
    diraction = -diraction;
    currAngle = ANGLE_MIN;
  }

  Serial.print("angle = "); Serial.println(currAngle);
  servo.writeMicroseconds(map(currAngle, ANGLE_MIN, ANGLE_MAX, SERVO_MIN, SERVO_MAX));
}

SonarData Sonar::scan(NewPing* sonar) {
  SonarData data = SonarData();
  data.angle = currAngle;
  data.captureTime = millis();
  data.distance = sonar->ping_cm();
  return data;
}

void Sonar::test() {
  Serial.print(F("Servo min value = ")); Serial.println(SERVO_MIN);
  servo.writeMicroseconds(SERVO_MIN);
  delay(10000);

  Serial.print(F("Servo middle value = ")); Serial.println((SERVO_MIN + SERVO_MAX) / 2);
  servo.writeMicroseconds((SERVO_MIN + SERVO_MAX) / 2);
  delay(10000);

  Serial.print(F("Servo max value = ")); Serial.println(SERVO_MAX);
  servo.writeMicroseconds(SERVO_MAX);
  delay(10000);

  for (int16_t i = 0; i <= 180; i += 1) {
    Serial.println(i);
    servo.write(i);
    delay(50);
  }
  for (int16_t i = 180; i >= 0; i -= 1) {
    Serial.println(i);
    servo.write(i);
    delay(50);
  }
}
