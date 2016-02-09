#ifndef Chassis_h
#define Chassis_h
#include "Arduino.h"
#include "HMC5883L.h"

#define LEFT 'l'
#define RIGHT 'r'
#define FORWARD 'f'
#define BACKWARD 'b'

class Chassis {
public:
	Chassis(int pinDirL, int pinDirR, int pinPwmL, int pinPwmR);
	void task();

	static void setMotorLeft(int pwm);
	static void setMotorRight(int pwm);

	void test();
	void stop();

	void move(int diraction);
	void moveSteps(int diraction, int steps);
	void moveDistance(int diraction, int distance);

	void setAzimuth(int azimuth, bool lock);
	void rotate(int diraction);
	void rotateTo(int azimuth);

private:
//methods
	static void countStepsL();
	static void countStepsR();
	static void checkSteps();

//variables
	static int stepsLeft;
	static int stepsRight;

	static int stepLastMillisLeft;
	static int stepLastMillisRight;

	static int stepTimeLeft;
	static int stepTimeRight;

	static int pwmLeft;
	static int pwmRight;

	static int pinDirLeft;
	static int pinDirRight;
	static int pinPwmLeft;
	static int pinPwmRight;

	static int targetAzimuth;
	static bool azimuthLock;

	static HMC5883L* compass;
};

#endif
