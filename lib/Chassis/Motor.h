#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
  public:
    Motor(int pinDir, int pwmPin);
    void handleStep();

    int getPwm();
    void setPwm(int pwm);

    int getSteps();
    void setSteps(int steps);
  private:
    int pwm;

  	int pinDir;
  	int pinPwm;

    int steps;
    int stepTime;
  	int stepLastMillis;
};

#endif
