#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
  public:
    Motor(int pinDir, int pwmPin);
    void handleStep();

    void stop();

    int getDir();
    void setDir(int Dir);

    int getPwm();
    void setPwm(int pwm);

    int getSteps();
    void setSteps(int steps);

    int getStepTime();
    void setStepTime(int stepTime);
  private:
    void changeSpeed();

    uint8_t pwm;
    int8_t dir;//Might be 1 (forward), 0 (stop) or -1 (backward)

  	int pinDir;
  	int pinPwm;

    int steps;
    int stepTime;
  	int stepLastMillis;
};

#endif
