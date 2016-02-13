#include "Chassis.h"

int Chassis::stepsLeft = 0;
int Chassis::stepsRight = 0;

int Chassis::stepLastMillisLeft = 0;
int Chassis::stepLastMillisRight = 0;

int Chassis::stepTimeLeft = 0;
int Chassis::stepTimeRight = 0;

int Chassis::pwmLeft = 0;
int Chassis::pwmRight = 0;

int Chassis::pinDirLeft = 0;
int Chassis::pinDirRight = 0;
int Chassis::pinPwmLeft = 0;
int Chassis::pinPwmRight = 0;

int Chassis::targetAzimuth = -1;
bool Chassis::azimuthLock = false;

HMC5883L* Chassis::compass;

Chassis::Chassis(int pinDirLeft, int pinDirRight, int pinPwmLeft, int pinPwmRight) {
	Chassis::pinDirLeft = pinDirLeft;
	Chassis::pinDirRight = pinDirRight;
	Chassis::pinPwmLeft = pinPwmLeft;
	Chassis::pinPwmRight = pinPwmRight;

	pinMode(pinDirLeft, OUTPUT);
	pinMode(pinDirRight, OUTPUT);
	pinMode(pinPwmLeft, OUTPUT);
	pinMode(pinPwmRight, OUTPUT);
	stop();

	attachInterrupt(0, Chassis::countStepsL, RISING);
	attachInterrupt(1, Chassis::countStepsR, RISING);

	compass = new HMC5883L();
}

float e = 3;
unsigned long lastTele = 0;//TODO: check if millis() is 0
void Chassis::task() {
	if (millis() > lastTele + 3000) {
		lastTele = millis();
		Serial.print("azimuth = "); Serial.print(compass->getAzimuth());
		Serial.print(" | target = "); Serial.println(targetAzimuth);
	}
	if (targetAzimuth != -1) {
		if (abs(targetAzimuth - compass->getAzimuth()) >= e)
			rotateTo(targetAzimuth);
		//else if (!azimuthLock) targetAzimuth = -1;
	}
}

void Chassis::setMotorLeft(int pwm) {
	pwmLeft = pwm;
	if (pwm >= 0) {
		digitalWrite(pinDirLeft, LOW);
		analogWrite(pinPwmLeft, pwm);
	} else {
		digitalWrite(pinDirLeft, HIGH);
		analogWrite(pinPwmLeft, 255 + pwm);
	}
}

void Chassis::setMotorRight(int pwm) {
	pwmRight = pwm;
	if (pwm >= 0) {
		digitalWrite(pinDirRight, LOW);
		analogWrite(pinPwmRight, pwm);
	} else {
		digitalWrite(pinDirRight, HIGH);
		analogWrite(pinPwmRight, 255 + pwm);
	}
}

void Chassis::countStepsL() {
	stepsLeft++;
	Serial.print("stepsLeft = "); Serial.println(stepsLeft);

	if (stepLastMillisLeft > 0) {
		int stepCurrMillisLeft = millis();
		stepTimeLeft = stepCurrMillisLeft - stepLastMillisLeft;
		stepLastMillisLeft = stepCurrMillisLeft;
		checkSteps();
	} else stepLastMillisLeft = millis();
}

void Chassis::countStepsR() {
	stepsRight++;
	Serial.print("stepsRight = "); Serial.println(stepsRight);

	if (stepLastMillisRight > 0) {
		int stepCurrMillisLeft = millis();
		stepTimeLeft = stepCurrMillisLeft - stepLastMillisLeft;
		stepLastMillisLeft = stepCurrMillisLeft;
		checkSteps();
	} else stepLastMillisRight = millis();
}

void Chassis::checkSteps() {
	if (stepTimeLeft > stepTimeRight) {
		setMotorLeft(map(stepTimeRight, 0, stepTimeLeft, 0, 255) * (pwmLeft = 0 ? 0 : pwmLeft > 0 ? 1 : -1));
		setMotorRight(pwmRight == 0 ? 0 : (pwmRight > 0 ? 255 : -255));
	} else {
		setMotorRight(map(stepTimeLeft, 0, stepTimeRight, 0, 255) * (pwmRight = 0 ? 0 : pwmRight > 0 ? 1 : -1));
		setMotorLeft(pwmLeft == 0 ? 0 : (pwmLeft > 0 ? 255 : -255));
	}
}

void Chassis::test() {
	int d = 3000;
	Serial.println(F("Left forward:"));
	setMotorLeft(255);
	delay(d);
	Serial.println(F("Left backward:"));
	setMotorLeft(-255);
	delay(d);
	setMotorLeft(0);

	Serial.println(F("Right forward:"));
	setMotorRight(255);
	delay(d);
	Serial.println(F("Right backward:"));
	setMotorRight(-255);
	delay(d);
	setMotorRight(0);

	for (int pwm = -255; pwm <= 255; pwm++) {
		setMotorLeft(pwm);
		setMotorRight(pwm);
		Serial.println(pwm);
		delay(100);
	}

	stop();
}

void Chassis::stop() {
	setMotorLeft(0);
	setMotorRight(0);
}

void Chassis::move(int diraction) {
	stop();
	if (diraction == FORWARD) {
		setMotorLeft(255);
		setMotorRight(255);
	} else {
		setMotorLeft(-255);
		setMotorRight(-255);
	}
}

void Chassis::moveSteps(int diraction, int steps) {//TODO:
	stop();
	move(diraction);
}

void Chassis::setAzimuth(int azimuth, bool lock) {
	Serial.print("new azimuth = "); Serial.println(azimuth);
	Chassis::targetAzimuth = azimuth;
	Chassis::azimuthLock = lock;
}

void Chassis::rotate(int diraction) {
	if (diraction == LEFT) {
		setMotorLeft(-255);
		setMotorRight(255);
	} else if (diraction == RIGHT) {
		setMotorLeft(255);
		setMotorRight(-255);
	}
}

void Chassis::rotateTo(int targetAzimuth) {
	int currAzimuth = (int) compass->getAzimuth();
	int deviation = targetAzimuth - currAzimuth;
	deviation += ((deviation <= -180) ? 360 : (deviation >= 180 ? -360 : 0));
	if (deviation >= 0) {
		rotate(RIGHT);
	} else {
		rotate(LEFT);
	}
}
