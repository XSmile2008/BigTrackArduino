#ifndef Sonar_h
#define Sonar_h
#include "Arduino.h"
#include "NewPing.h"

class Sonar {
  public:
    Sonar(uint8_t trig0, uint8_t echo0, uint8_t trig1, uint8_t echo1);
  private:
    NewPing* sonar0;
    NewPing* sonar1;
};

#endif
