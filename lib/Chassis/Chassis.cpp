#include "Chassis.h"

int Chassis::targetAzimuth = -1;
bool Chassis::azimuthLock = false;

HMC5883L* Chassis::compass;
Motor* Chassis::motorLeft;
Motor* Chassis::motorRight;

const int16_t maxDelta = 255;
const int16_t maxOffset = 50;
int16_t offsetY = -20;
int16_t axisX, axisY;
int16_t l, r;

Chassis::Chassis(int pinDirLeft, int pinDirRight, int pinPwmLeft, int pinPwmRight) {
	compass = new HMC5883L();
	motorLeft = new Motor(pinDirLeft, pinPwmLeft);
	motorRight = new Motor(pinDirRight, pinPwmRight);

	stop();

	attachInterrupt(0, Chassis::countStepsL, RISING);
	attachInterrupt(1, Chassis::countStepsR, RISING);
}

float e = 3;
void Chassis::task() {
	telemetry();
	// checkMotorsSpeed();
	if (targetAzimuth != -1) {
		if (abs(targetAzimuth - compass->getAzimuth()) >= e)
			rotateTo(targetAzimuth);
		//else if (!azimuthLock) targetAzimuth = -1;
	}
}

const uint16_t TELEMETRY_PAUSE = 30;
unsigned long lastTele = 0;//TODO: check if millis() is 0
void Chassis::telemetry() {
	if (millis() > lastTele + TELEMETRY_PAUSE) {
		lastTele = millis();
		printf_P(PSTR("\r\nFreeMem.onStartTele: %d\n"), freeMemory());
		float currAzimuth = compass->getAzimuth();

		// Serial.print(F("azimuth = ")); Serial.print(currAzimuth);
		// Serial.print(F(" | target = ")); Serial.println(targetAzimuth);

		Command* telemetry = new Command('T');
		telemetry->getArguments()->add(new Argument('a', sizeof(currAzimuth), &currAzimuth));
		telemetry->getArguments()->add(new Argument('t', sizeof(unsigned long), &lastTele));
		byte* buffer;
		uint16_t length;
		telemetry->serialize(buffer, length);
		Serial.write(buffer, length);
		delete[] buffer;
		delete telemetry;
		printf_P(PSTR("FreeMem.onEndTele: %d\n"), freeMemory());
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

void Chassis::checkMotorsSpeed() {
	if (motorLeft->getDir() == 0 || motorRight->getDir() == 0) return;
	if (motorLeft->getSteps() > 0 && motorRight->getSteps() > 0 && false) {
		offsetY -= percents(motorLeft->getStepTime(), motorRight->getStepTime());
		offsetY = constrain(offsetY, -maxOffset, maxOffset);

		int16_t pwmL = l - offsetY/2;
		int16_t pwmR = r + offsetY/2;

		fill(pwmL, pwmR, 0, 255, 0, 255);

		motorLeft->setPwm(pwmL);
		motorRight->setPwm(pwmR);
		motorLeft->setSteps(0);
		motorRight->setSteps(0);

		Serial.print(F("left = ")); Serial.print(motorLeft->getStepTime()); Serial.print(F(" right = ")); Serial.println(motorRight->getStepTime());
		Serial.print(F("offsetY = ")); Serial.println(offsetY);
	} else {
		//TODO: stop worked motor and full power stoped motor
	}
}

void Chassis::fill(int16_t& l, int16_t& r, int16_t minL, int16_t maxL, int16_t minR, int16_t maxR) {
	int16_t aboveL = l - maxL;
	if (aboveL < 0) aboveL = 0;
	int16_t belowL = l - minL;
	if (belowL > 0) belowL = 0;

	int16_t aboveR = r - maxR;
	if (aboveR < 0) aboveR = 0;
	int16_t belowR = r - minR;
	if (belowR > 0) belowR = 0;

	l = constrain(l + belowR - aboveR, minL, maxL);
	r = constrain(r + belowL - aboveL, minR, maxR);
}

/*
* return positive percent value if l > r
* return negative percent value if l < r
*/
int8_t Chassis::percents(int16_t l, int16_t r) {
	return (float) (l - r) / (float) (l > r ? l : r) * 100.0;
}

void Chassis::xy2lr(int16_t x, int16_t y, int16_t &l, int16_t &r) {
	float scale = 255.0;
	float v = (scale - abs(x)) * (y / scale) + y;
	float w = (scale - abs(y)) * (x / scale) + x;
	l = (int16_t) (v - w) / 2.0;
	r = (int16_t) (v + w) / 2.0;
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

void Chassis::move(int16_t x, int16_t y) {
	// if (y >= 0) x = -x;
	xy2lr(-x, y, l, r);//TODO: x axis is inversed
	motorLeft->setPwm(abs(l));
	motorRight->setPwm(abs(r));
	motorLeft->setDir(l == 0 ? 0 : (l > 0 ? 1 : -1));
	motorRight->setDir(r == 0 ? 0 : (r > 0 ? 1 : -1));
	Serial.println("MOVE: x = " + String(x) + ", y = " + String(y) + ", l = " + String(l) + ", r = " + String(r));
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
		move(255, 0);//right
	} else {
		move(-255, 0);//left
	}
}
