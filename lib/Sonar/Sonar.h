#ifndef Sonar_h
#define Sonar_h
#include "Arduino.h"
#include "Servo.h"
#include "NewPing.h"
#include "MemoryFree.h"
#include "CircularBuffer.h"

struct SonarData {
  int16_t angle;
  int16_t distance;
  int16_t captureTime;
};

class Sonar {
  public:
    Sonar(uint8_t servoPin, uint8_t trig0, uint8_t echo0, uint8_t trig1, uint8_t echo1);
    void task();
    void step();
    void test();
  private:
    void setAngle(uint16_t angle);
    SonarData scan(NewPing* sonar);

    static const uint16_t SERVO_MIN = 544;
    static const uint16_t SERVO_MAX = 2400;
    static const int16_t ANGLE_MIN = 5;
    static const int16_t ANGLE_MAX = 180;
    static const int16_t ANGLE_OFFSET = 90;

    static const uint8_t ANGLE_STEP = 10;
    static const uint8_t PAUSE_SCAN = 15;
    static const uint8_t PAUSE_STEP = 250;

    static const uint8_t MAX_SCAN_DISTANCE = 250;

    Servo servo;
    NewPing* sonar0;
    NewPing* sonar1;
};

#endif
