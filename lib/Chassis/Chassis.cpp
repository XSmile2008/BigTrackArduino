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

int8_t pinBumpL = A0, pinBumpR = A1;//TODO: bump

Chassis::Chassis(int pinDirLeft, int pinDirRight, int pinPwmLeft, int pinPwmRight) {
	compass = new HMC5883L();
	motorLeft = new Motor(pinDirLeft, pinPwmLeft);
	motorRight = new Motor(pinDirRight, pinPwmRight);

	stop();

	attachInterrupt(0, Chassis::countStepsL, RISING);
	attachInterrupt(1, Chassis::countStepsR, RISING);

	pinMode(pinBumpL, INPUT_PULLUP);
	pinMode(pinBumpR, INPUT_PULLUP);}

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

const uint16_t TELEMETRY_PAUSE = 50;
unsigned long lastTele;//TODO: check if millis() is 0
void Chassis::telemetry() {
	if (millis() > lastTele + TELEMETRY_PAUSE) {
		lastTele = millis();
		int freemem = freeMemory();
		// printf_P(PSTR("FreeMem.onStartTele: %d\n"), freemem);
		float currAzimuth = compass->getAzimuth();
		// Serial.print(F("azimuth = ")); Serial.print(currAzimuth);
		// Serial.print(F(" | target = ")); Serial.println(targetAzimuth);

		Command* telemetry = new Command('T');
		telemetry->getArguments()->add(new Argument('a', sizeof(currAzimuth), &currAzimuth));
		telemetry->getArguments()->add(new Argument('t', sizeof(unsigned long), &lastTele));
		telemetry->getArguments()->add(new Argument('m', sizeof(int), &freemem));
		telemetry->toSerial();
		delete telemetry;
		// printf_P(PSTR("FreeMem.onEndTele: %d\n"), freeMemory());
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
	if (!digitalRead(pinBumpL) && l > 0) motorLeft->stop();//TODO: bump
	if (!digitalRead(pinBumpR) && r > 0) motorRight->stop();//TODO: bump
	if (motorLeft->getDir() == 0 || motorRight->getDir() == 0) return;
	if (motorLeft->getSteps() > 0 && motorRight->getSteps() > 0) {
		offsetY -= percents(motorLeft->getStepTime(), motorRight->getStepTime());
		offsetY = constrain(offsetY, -maxOffset, maxOffset);

		int16_t pwmL = abs(l) - offsetY/2;
		int16_t pwmR = abs(r) + offsetY/2;

		fill(pwmL, pwmR, 0, 255);

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

void Chassis::fill(int16_t& l, int16_t& r, int16_t min, int16_t max) {
	int16_t belowL = l > min ? 0 : l - min;
	int16_t aboveL = l < max ? 0 : l - max;
	int16_t belowR = r > min ? 0 : r - min;
	int16_t aboveR = r < max ? 0 : r - max;

	int16_t correction = - belowL - aboveL - belowR - aboveR;
	l = constrain(l + correction, min, max);
	r = constrain(r + correction, min, max);
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
	if (!digitalRead(pinBumpL) && l > 0) l = 0;//TODO: bump
	if (!digitalRead(pinBumpR) && r > 0) r = 0;//TODO: bump
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
