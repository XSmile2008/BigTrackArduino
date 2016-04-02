#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
  public:
    Motor(int pinDir, int pwmPin);
    void handleStep();
    void setPwm(int pwm);
    int getPwm();
    int getSteps();
  private:
    int pwm;

  	int pinDir;
  	int pinPwm;

    int steps;
    int stepTime;
  	int stepLastMillis;
};

#endif
