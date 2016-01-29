#ifndef Chassis_h
#define Chassis_h
#include "Arduino.h"
#include "HMC5883L.h"

#define LEFT 0
#define RIGHT 1
#define FORWARD 2
#define BACKWARD 3

class Chassis {
public:
	Chassis(int motorLF, int motorLB, int motorRF, int motorRB);
	void task();
	void test();
	void stop();
	void move(int diraction);
	void moveSteps(int diraction, int steps);
	void moveDistance(int diraction, int distance);
	void setAzimuth(int azimuth, bool lock);
	void rotate(int diraction);
	void rotateTo(int azimuth);

private:
	static void countStepsL();
	static void countStepsR();

	static int stepsL;
	static int stepsR;

	static int motorLF;
	static int motorLB;
	static int motorRF;
	static int motorRB;

	static int azimuth;
	static bool azimuthLock;

	static HMC5883L* compass;
};

#endif
