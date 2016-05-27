#ifndef Sonar_h
#define Sonar_h
#include "Arduino.h"
#include "Servo.h"
#include "NewPing.h"
#include "MemoryFree.h"
#include "CircularBuffer.h"
// #include "CircularBuffer.cpp"

struct SonarData {
  int16_t angle;
  int16_t distance;
  uint32_t captureTime;
};

class Sonar {
  public:
    Sonar(uint8_t servoPin, uint8_t trig0, uint8_t echo0, uint8_t trig1, uint8_t echo1);
    void task();
    void step();
    void test();
    CircularBuffer<SonarData*>* getData();
  private:
    SonarData* scan(NewPing* sonar, int16_t offset);

    static const uint16_t SERVO_MIN = 544;
    static const uint16_t SERVO_MAX = 2400;
    static const int16_t ANGLE_MIN = 5;
    static const int16_t ANGLE_MAX = 180;
    static const int16_t ANGLE_OFFSET = 90;

    static const uint8_t ANGLE_STEP = 10;
    static const uint8_t PAUSE_SCAN = 15;
    static const uint8_t PAUSE_STEP = 250;
    // static const uint8_t PAUSE_SCAN = 250;
    // static const uint16_t PAUSE_STEP = 1000;

    static const uint8_t MAX_SCAN_DISTANCE = 250;

    Servo servo;
    NewPing** sonars;
};

#endif
