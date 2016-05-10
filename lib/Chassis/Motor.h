#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
  public:
    Motor(uint8_t pinDir, uint8_t pwmPin);
    void handleStep();

    void stop();

    int8_t getDir();
    void setDir(int8_t dir);

    uint8_t getPwm();
    void setPwm(uint8_t pwm);

    uint16_t getSteps();
    void setSteps(uint16_t steps);

    uint16_t getStepTime();
    void setStepTime(uint16_t stepTime);
  private:
    void changeSpeed();

    uint8_t pwm;
    int8_t dir;//Might be 1 (forward), 0 (stop) or -1 (backward)

  	uint8_t pinDir;
  	uint8_t pinPwm;

    uint16_t steps;
    uint16_t stepTime;
  	uint32_t stepLastMillis;
};

#endif
