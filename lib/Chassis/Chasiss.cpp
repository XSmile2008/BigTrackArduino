#include "Chassis.h"

int Chassis::stepsL = 0;
int Chassis::stepsR = 0;
int Chassis::motorLF = 0;
int Chassis::motorLB = 0;
int Chassis::motorRF = 0;
int Chassis::motorRB = 0;

int Chassis::azimuth = 0;
bool Chassis::azimuthLock = false;

float e = 3;

HMC5883L* Chassis::compass;

Chassis::Chassis(int motorLF, int motorLB, int motorRF, int motorRB) {
	Chassis::motorLF = motorLF;
	Chassis::motorLB = motorLB;
	Chassis::motorRF = motorRF;
	Chassis::motorRB = motorRB;
	pinMode(motorLF, OUTPUT);
	pinMode(motorLB, OUTPUT);
	pinMode(motorRF, OUTPUT);
	pinMode(motorRB, OUTPUT);

	attachInterrupt(0, Chassis::countStepsL, RISING);
	attachInterrupt(1, Chassis::countStepsR, RISING);

	compass = new HMC5883L();
	//compass.setScale(0.88);
	//compass.setMeasurementMode(Measurement_Continuous);//TODO: create more constructors
}

void Chassis::task() {
	compass->getAzimuth();//TODO: remove
	if (azimuth != -1) {
		if (abs(azimuth - compass->getAzimuth()) >= e)
			rotateTo(azimuth);
		else if (!azimuthLock) azimuth = -1;
	}
}

void Chassis::test() {
	stepsL = 1000;
	stepsR = 1000;
	int d = 3000;
	Serial.println("motorLF");
	digitalWrite(motorLF, HIGH);
	delay(d);
	digitalWrite(motorLF, LOW);

	Serial.println("motorLB");
	digitalWrite(motorLB, HIGH);
	delay(d);
	digitalWrite(motorLB, LOW);

	Serial.println("motorRF");
	digitalWrite(motorRF, HIGH);
	delay(d);
	digitalWrite(motorRF, LOW);

	Serial.println("motorRB");
	digitalWrite(motorRB, HIGH);
	delay(d);
	digitalWrite(motorRB, LOW);

	stop();
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

void Chassis::stop() {
	stepsL = 0;
	stepsR = 0;
	digitalWrite(motorLF, LOW);
	digitalWrite(motorLB, LOW);
	digitalWrite(motorRF, LOW);
	digitalWrite(motorRB, LOW);
}

void Chassis::move(int diraction) {
	stop();
	if (diraction == FORWARD) {
		digitalWrite(motorLF, HIGH);
		digitalWrite(motorRF, HIGH);
	} else {
		digitalWrite(motorLB, HIGH);
		digitalWrite(motorRB, HIGH);
	}
}

void Chassis::moveSteps(int diraction, int steps) {
	stop();
	stepsR = steps;
	stepsL = steps;
	move(diraction);
}

void Chassis::setAzimuth(int azimuth, bool lock) {
	Chassis::azimuth = azimuth;
	Chassis::azimuthLock = lock;
}

void Chassis::rotate(int diraction) {
	if (diraction == LEFT) {
		digitalWrite(motorLB, HIGH);
		digitalWrite(motorRF, HIGH);
	} else if (diraction == RIGHT) {
		digitalWrite(motorRB, HIGH);
		digitalWrite(motorLF, HIGH);
	}
}

void Chassis::rotateTo(int azimuth) {
	int currAzimuth = (int) compass->getAzimuth();
	azimuth += 360 - currAzimuth;
	if (azimuth <= 180) {
		rotate(RIGHT);
	} else {
		rotate(LEFT);
	}
}
