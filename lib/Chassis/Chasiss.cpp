#include "Chassis.h"

int Chassis::targetAzimuth = -1;
bool Chassis::azimuthLock = false;

HMC5883L* Chassis::compass;
Motor* Chassis::motorLeft;
Motor* Chassis::motorRight;

Chassis::Chassis(int pinDirLeft, int pinDirRight, int pinPwmLeft, int pinPwmRight) {
	motorLeft = new Motor(pinDirLeft, pinPwmLeft);
	motorRight = new Motor(pinDirRight, pinPwmRight);

	stop();

	attachInterrupt(0, Chassis::countStepsL, RISING);
	attachInterrupt(1, Chassis::countStepsR, RISING);

	compass = new HMC5883L();
}

float e = 3;
void Chassis::task() {
	// telemetry();
	if (targetAzimuth != -1) {
		if (abs(targetAzimuth - compass->getAzimuth()) >= e)
			rotateTo(targetAzimuth);
		//else if (!azimuthLock) targetAzimuth = -1;
	}
}

unsigned long lastTele = 0;//TODO: check if millis() is 0
void Chassis::telemetry() {
	if (millis() > lastTele + 3000) {
		Serial.print(F("\r\nFreeMem.onStartTele: ")); Serial.println(freeMemory());
		lastTele = millis();
		float currAzimuth = compass->getAzimuth();

		// Serial.print(F("azimuth = ")); Serial.print(currAzimuth);
		// Serial.print(F(" | target = ")); Serial.println(targetAzimuth);

		Command* telemetry = new Command('T');
		telemetry->getArguments()->add(new Argument('a', sizeof(currAzimuth), &currAzimuth));
		telemetry->serialize();
		delete telemetry;
		Serial.print(F("FreeMem.onEndTele: ")); Serial.println(freeMemory());
	}
}

void Chassis::countStepsL() {
	motorLeft->handleStep();
	Serial.print("stepsLeft = "); Serial.println(motorLeft->getSteps());
	checkSteps();
}

void Chassis::countStepsR() {
	motorRight->handleStep();
	Serial.print("stepsRight = "); Serial.println(motorRight->getSteps());
	checkSteps();
}

void Chassis::checkSteps() {
	// Serial.print("stepTimeLeft = "); Serial.println(stepTimeLeft);
	// Serial.print("stepTimeRight = "); Serial.println(stepTimeRight);
	// if (stepTimeLeft > stepTimeRight) {
	// 	setMotorLeft(map(stepTimeRight, 0, stepTimeLeft, 0, 255) * (pwmLeft == 0 ? 0 : pwmLeft > 0 ? 1 : -1));
	// 	setMotorRight(pwmRight == 0 ? 0 : (pwmRight > 0 ? 255 : -255));
	// } else {
	// 	setMotorRight(map(stepTimeLeft, 0, stepTimeRight, 0, 255) * (pwmRight == 0 ? 0 : pwmRight > 0 ? 1 : -1));
	// 	setMotorLeft(pwmLeft == 0 ? 0 : (pwmLeft > 0 ? 255 : -255));
	// }
}

void Chassis::test() {
	int d = 3000;
	Serial.println(F("Left forward:"));
	motorLeft->setPwm(255);
	delay(d);
	Serial.println(F("Left backward:"));
	motorLeft->setPwm(-255);
	delay(d);
	motorLeft->setPwm(0);

	Serial.println(F("Right forward:"));
	motorRight->setPwm(255);
	delay(d);
	Serial.println(F("Right backward:"));
	motorRight->setPwm(-255);
	delay(d);
	motorRight->setPwm(0);

	for (int pwm = -255; pwm <= 255; pwm++) {
		motorLeft->setPwm(pwm);
		motorRight->setPwm(pwm);
		Serial.println(pwm);
		delay(100);
	}

	stop();
}

void Chassis::stop() {
	motorLeft->setPwm(0);
	motorRight->setPwm(0);
}

void Chassis::move(int diraction) {
	stop();
	if (diraction == FORWARD) {
		motorLeft->setPwm(255);
		motorRight->setPwm(255);
	} else {
		motorLeft->setPwm(-255);
		motorRight->setPwm(-255);
	}
}

void Chassis::moveSteps(int diraction, int steps) {//TODO:
	stop();
	move(diraction);
}

void Chassis::setAzimuth(int azimuth, bool lock) {
	Serial.print(F("new azimuth = ")); Serial.println(azimuth);
	Chassis::targetAzimuth = azimuth;
	Chassis::azimuthLock = lock;
}

void Chassis::rotate(int diraction) {
	if (diraction == LEFT) {
		motorLeft->setPwm(-255);
		motorRight->setPwm(255);
	} else if (diraction == RIGHT) {
		motorLeft->setPwm(255);
		motorRight->setPwm(-255);
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
