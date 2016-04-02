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
	checkSteps();
	if (targetAzimuth != -1) {
		if (abs(targetAzimuth - compass->getAzimuth()) >= e)
			rotateTo(targetAzimuth);
		//else if (!azimuthLock) targetAzimuth = -1;
	}
}

unsigned long lastTele = 0;//TODO: check if millis() is 0
void Chassis::telemetry() {
	if (millis() > lastTele + 3000) {
		lastTele = millis();
		Serial.print(F("\r\nFreeMem.onStartTele: ")); Serial.println(freeMemory());
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
}

void Chassis::countStepsR() {
	motorRight->handleStep();
	Serial.print("stepsRight = "); Serial.println(motorRight->getSteps());
}

unsigned long lastCheckSteps = 0;//TODO: check if millis() is 0
uint8_t minPwm = 0;
void Chassis::checkSteps() {
	if (millis() > lastCheckSteps + 500) {
		lastCheckSteps = millis();
		int stepsL = motorLeft->getSteps();
		int stepsR = motorRight->getSteps();
		int pwmL = motorLeft->getPwm();
		int pwmR = motorRight->getPwm();
		if (stepsL > stepsR) {
			motorLeft->setPwm((pwmL + map(stepsR, -1, stepsL, minPwm, 255))/2);
			motorRight->setPwm(255);
		} else if (stepsL < stepsR) {
			motorRight->setPwm((pwmR + map(stepsL, -1, stepsR, minPwm, 255))/2);
			motorLeft->setPwm(255);
		}
		motorLeft->setSteps(0);
		motorRight->setSteps(0);
	}
}

void Chassis::test() {
	int d = 3000;
	motorLeft->setPwm(255);
	Serial.println(F("Left forward:"));  motorLeft->setDir(1); delay(d);
	Serial.println(F("Left backward:")); motorLeft->setDir(-1); delay(d);
	motorLeft->setDir(0);

	motorRight->setPwm(255);
	Serial.println(F("Right forward:"));  motorRight->setDir(1); delay(d);
	Serial.println(F("Right backward:")); motorRight->setDir(-1); delay(d);
	motorRight->setDir(0);

	for (int dir = 0; dir <=1; dir ++) {
		motorLeft->setDir(dir > 0 ? 1 : -1);
		motorRight->setDir(dir > 0 ? 1 : -1);
		for (int pwm = 0; pwm <= 255; pwm++) {
			motorLeft->setPwm(pwm);
			motorRight->setPwm(pwm);
			Serial.println(pwm);
			delay(100);
		}
	}

	stop();
}

void Chassis::stop() {
	motorLeft->stop();
	motorRight->stop();
}

void Chassis::move(int diraction) {
	stop();
	motorLeft->setPwm(255);
	motorRight->setPwm(255);
	motorLeft->setDir(diraction == FORWARD ? 1 : -1);
	motorRight->setDir(diraction == FORWARD ? 1 : -1);
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
	motorLeft->setPwm(255);
	motorRight->setPwm(255);
	motorLeft->setDir(diraction == LEFT ? -1 : 1);
	motorRight->setDir(diraction == LEFT ? 1 : -1);
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
