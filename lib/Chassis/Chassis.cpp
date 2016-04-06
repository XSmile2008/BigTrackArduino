#include "Chassis.h"
#include "CircularBuffer.cpp"//To stack overflow

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
	checkMotorsSpeed();
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

/*vvvvvvvvvvvvvvvvvvvvvvvvvvvv
 * Motor control section start
 */

void Chassis::countStepsL() {
	motorLeft->handleStep();
	// Serial.print("left = "); Serial.println(motorLeft->getStepTime());
	// Serial.print("stepsLeft = "); Serial.println(motorLeft->getSteps());
}

void Chassis::countStepsR() {
	motorRight->handleStep();
	// Serial.print(" right = "); Serial.println(motorRight->getStepTime());
	// Serial.print("stepsRight = "); Serial.println(motorRight->getSteps());
}

uint8_t maxOffset = 50;
int8_t pwmOffset = -20;
void Chassis::checkMotorsSpeed() {
	if (motorLeft->getDir() == 0 || motorRight->getDir() == 0) return;
	if (motorLeft->getSteps() > 0 && motorRight->getSteps() > 0) {
		int16_t delta =  motorLeft->getStepTime() - motorRight->getStepTime();
		uint16_t slowerStepTime = delta > 0 ? motorLeft->getStepTime() : motorRight->getStepTime();
		int8_t percents = (float) delta / (float) slowerStepTime * 100;

		pwmOffset -= percents;
		if (pwmOffset > maxOffset) pwmOffset = maxOffset;
		else if (pwmOffset < -maxOffset) pwmOffset = -maxOffset;

		motorLeft->setPwm(pwmOffset > 0 ? 255 - pwmOffset : 255);
		motorRight->setPwm(pwmOffset < 0 ? 255 + pwmOffset : 255);
		motorLeft->setSteps(0);
		motorRight->setSteps(0);

		Serial.print(F("left = ")); Serial.print(motorLeft->getStepTime()); Serial.print(F(" right = ")); Serial.println(motorRight->getStepTime());
		Serial.print(F("delta = ")); Serial.print(delta); Serial.print(F("percents = ")); Serial.print(percents); Serial.print(F(" offset = ")); Serial.println(pwmOffset);
	} else {
		//TODO:
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

	for (int8_t dir = 0; dir <=1; dir ++) {
		motorLeft->setDir(dir > 0 ? 1 : -1);
		motorRight->setDir(dir > 0 ? 1 : -1);
		for (uint8_t pwm = 0; pwm <= 255; pwm++) {
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

void Chassis::rotate(int diraction) {
	motorLeft->setPwm(255);
	motorRight->setPwm(255);
	motorLeft->setDir(diraction == LEFT ? -1 : 1);
	motorRight->setDir(diraction == LEFT ? 1 : -1);
}

/*
 * Motor control section end
 *^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void Chassis::setAzimuth(int azimuth, bool lock) {
	Serial.print(F("new azimuth = ")); Serial.println(azimuth);
	Chassis::targetAzimuth = azimuth;
	Chassis::azimuthLock = lock;
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
