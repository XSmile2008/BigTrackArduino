#include "Motor.h"

Motor::Motor(uint8_t pinDir, uint8_t pinPwm) {
  pinMode(pinDir, OUTPUT);
	pinMode(pinPwm, OUTPUT);

  this->pinDir = pinDir;
  this->pinPwm = pinPwm;

  this->pwm = 0;
  this->steps = 0;
  this->stepTime = 0;
  this->stepLastMillis = 0;
}

void Motor::changeSpeed() {
  if (dir > 0) {
		digitalWrite(pinDir, LOW);
		analogWrite(pinPwm, pwm);
	} else if (dir < 0) {
		digitalWrite(pinDir, HIGH);
		analogWrite(pinPwm, 255 - pwm);
	} else {
    digitalWrite(pinDir, LOW);
    digitalWrite(pinPwm, LOW);
  }
}

void Motor::handleStep() {
  steps++;
	if (stepLastMillis > 0) {
		uint32_t stepCurrMillis = millis();
		stepTime = stepCurrMillis - stepLastMillis;
		stepLastMillis = stepCurrMillis;
	} else stepLastMillis = millis();
}

void Motor::stop() {
  dir = 0;
  pwm = 0;
  digitalWrite(pinDir, LOW);
  digitalWrite(pinPwm, LOW);
}

int8_t Motor::getDir() {
  return dir;
}

void Motor::setDir(int8_t dir) {
  this->dir = dir;
  changeSpeed();
}

uint8_t Motor::getPwm() {
  return pwm;
}

void Motor::setPwm(uint8_t pwm) {
	// Serial.print("PWM"); Serial.print((int)this); Serial.print(" = "); Serial.println(pwm);
	this->pwm = pwm;
  changeSpeed();
}

uint16_t Motor::getSteps() {
  return steps;
}

void Motor::setSteps(uint16_t steps) {
  this->steps = steps;
}

uint16_t Motor::getStepTime() {
  return stepTime;
}

void Motor::setStepTime(uint16_t stepTime) {
  this->stepTime = stepTime;
}
