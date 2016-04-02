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

void Motor::handleStep() {
  steps++;
	if (stepLastMillis > 0) {
		int stepCurrMillis = millis();
		stepTime = stepCurrMillis - stepLastMillis;
		stepLastMillis = stepCurrMillis;
	} else stepLastMillis = millis();
}

void Motor::setPwm(int pwm) {
	Serial.print("PWM"); Serial.print((int)this); Serial.print(" = "); Serial.println(pwm);
	this->pwm = pwm;
	if (pwm >= 0) {
		digitalWrite(pinDir, LOW);
		analogWrite(pinPwm, pwm);
	} else {
		digitalWrite(pinDir, HIGH);
		analogWrite(pinPwm, 255 + pwm);
	}
}

int Motor::getPwm() {
  return pwm;
}

int Motor::getSteps() {
  return steps;
}
