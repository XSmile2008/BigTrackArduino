#include "Chassis.h"

int Chassis::stepsL = 0;
int Chassis::stepsR = 0;
int Chassis::pinDirLeft = 0;
int Chassis::pinDirRight = 0;
int Chassis::pinPwmLeft = 0;
int Chassis::pinPwmRight = 0;

int Chassis::targetAzimuth = -1;
bool Chassis::azimuthLock = false;

float e = 3;

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

long lastTele = 0;//TODO: check if millis() is 0
void Chassis::task() {
	if (millis() > lastTele + 1000) {
		lastTele = millis();
		Serial.print("azimuth = ");
		Serial.print(compass->getAzimuth());
		Serial.print(" | target = ");
		Serial.println(targetAzimuth);
	}
	if (targetAzimuth != -1) {
		if (abs(targetAzimuth - compass->getAzimuth()) >= e)
			rotateTo(targetAzimuth);
		//else if (!azimuthLock) targetAzimuth = -1;
	}
}

void Chassis::setMotorLeft(int pwm) {
	if (pwm >= 0) {
		digitalWrite(pinDirLeft, LOW);
		analogWrite(pinPwmLeft, pwm);
	} else {
		digitalWrite(pinDirLeft, HIGH);
		analogWrite(pinPwmLeft, 255 + pwm);
	}
}

void Chassis::setMotorRight(int pwm) {
	if (pwm >= 0) {
		digitalWrite(pinDirRight, LOW);
		analogWrite(pinPwmRight, pwm);
	} else {
		digitalWrite(pinDirRight, HIGH);
		analogWrite(pinPwmRight, 255 + pwm);
	}
}

void Chassis::countStepsL() {
	stepsL--;
	Serial.print("stepsL = ");
	Serial.println(stepsL);
	// if (stepsL <= 0) {
	// 	digitalWrite(motorLF, LOW);
 //    	digitalWrite(motorLB, LOW);
	// }
}

void Chassis::countStepsR() {
	stepsR--;
	Serial.print("stepsR = ");
	Serial.println(stepsR);
	// if (stepsR <= 0) {
	// 	digitalWrite(motorRF, LOW);
 //    	digitalWrite(motorRB, LOW);
	// }
}

void Chassis::test() {
	stepsL = 1000;//TODO: fix it
	stepsR = 1000;
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
	Serial.println("stop");
	stepsL = 0;
	stepsR = 0;
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

void Chassis::moveSteps(int diraction, int steps) {
	stop();
	stepsR = steps;
	stepsL = steps;
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
	if (millis() > lastTele + 1000) {
		Serial.print("targetAzimuth - currAzimuth = "); Serial.println(targetAzimuth - currAzimuth - ((currAzimuth - targetAzimuth) > 180 ? 360 : 0) );
	}
	if (targetAzimuth - currAzimuth >= 0) {
		//Serial.println("rotate RIGHT");
		rotate(RIGHT);
	} else {
		//Serial.println("rotate LEFT");
		rotate(LEFT);
	}
}
