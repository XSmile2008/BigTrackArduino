#include "Sonar.h"

CircularBuffer<SonarData*>* datas = new CircularBuffer<SonarData*>(20);

int8_t diraction = 1;
int16_t currAngle = 0;

Sonar::Sonar(uint8_t servoPin, uint8_t trig0, uint8_t echo0, uint8_t trig1, uint8_t echo1) {
  sonar = new NewPing*[2];
  sonar[0] = new NewPing(trig0, echo0, MAX_SCAN_DISTANCE);
  sonar[1] = new NewPing(trig1, echo1, MAX_SCAN_DISTANCE);
  servo.attach(servoPin);
  currAngle = ANGLE_MIN;
  servo.writeMicroseconds(map(currAngle, ANGLE_MIN, ANGLE_MAX, SERVO_MAX, SERVO_MIN));//Reversed
}

//-1 change angle, 0 scan first sonar, 1 scan second sonar
uint32_t lastTime = 0;
int8_t nyan = -1;
void Sonar::task() {
  uint32_t newTime = millis();
  if (newTime >= lastTime + (nyan < 0 ? PAUSE_STEP : PAUSE_SCAN)) {
    // printf_P(PSTR("state[%d] - %lu | %d\n"), nyan, millis(), freeMemory());
    if (nyan == -1) {
      step();
    } else {
      datas->put(scan(sonar[nyan], 180 * nyan));//TODO: this is hack
    }
    if (++nyan > 1) nyan = -1;//TODO: one more hack
    lastTime = newTime;
  }
}

void Sonar::step() {
  currAngle = currAngle + ANGLE_STEP * diraction;
  if (currAngle > ANGLE_MAX) {
    diraction = -diraction;
    currAngle = ANGLE_MAX;
  } else if (currAngle < ANGLE_MIN) {
    diraction = -diraction;
    currAngle = ANGLE_MIN;
  }
  servo.writeMicroseconds(map(currAngle, ANGLE_MIN, ANGLE_MAX, SERVO_MAX, SERVO_MIN));//Reversed
}

SonarData* Sonar::scan(NewPing* sonar, int16_t offset) {
  int16_t angle = currAngle - offset - ANGLE_OFFSET;
  if (angle > 360) angle -= 360;
  else if (angle < 0) angle += 360;
  SonarData* data = new SonarData();
  data->angle = angle;
  data->captureTime = millis();
  data->distance = sonar->ping_cm();
  return data;
}

CircularBuffer<SonarData*>* Sonar::getData() {
  return datas;
}

void Sonar::test() {
  printf_P(PSTR("Servo min value = %d\n"), SERVO_MIN);
  servo.writeMicroseconds(SERVO_MIN);
  delay(10000);

  printf_P(PSTR("Servo middle value = %d\n"), (SERVO_MIN + SERVO_MAX) / 2);
  servo.writeMicroseconds((SERVO_MIN + SERVO_MAX) / 2);
  delay(10000);

  printf_P(PSTR("Servo max value = %d\n"), SERVO_MAX);
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
