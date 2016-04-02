#include "Motor.h"

Motor::Motor(int pinDir, int pinPwm) {
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
		int stepCurrMillis = millis();
		stepTime = (stepTime + stepCurrMillis - stepLastMillis)/2;
		stepLastMillis = stepCurrMillis;
	} else stepLastMillis = millis();
}

void Motor::stop() {
  dir = 0;
  pwm = 0;
  digitalWrite(pinDir, LOW);
  digitalWrite(pinPwm, LOW);
}

int Motor::getDir() {
  return dir;
}

void Motor::setDir(int dir) {
  this->dir = dir;
  changeSpeed();
}

int Motor::getPwm() {
  return pwm;
}

void Motor::setPwm(int pwm) {
	Serial.print("PWM"); Serial.print((int)this); Serial.print(" = "); Serial.println(pwm);
	this->pwm = pwm;
  changeSpeed();
}

int Motor::getSteps() {
  return steps;
}

void Motor::setSteps(int steps) {
  this->steps = steps;
}

int Motor::getStepTime() {
  return stepTime;
}

void Motor::setStepTime(int stepTime) {
  this->stepTime = stepTime;
}
