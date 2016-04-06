#ifndef Chassis_h
#define Chassis_h
#include "Arduino.h"
#include "HMC5883L.h"
#include "Motor.h"
#include "Command.h"
#include "CircularBuffer.h"
//#include "Commands.h"

#define LEFT 'l'
#define RIGHT 'r'
#define FORWARD 'f'
#define BACKWARD 'b'

class Chassis {
public:
	Chassis(int pinDirL, int pinDirR, int pinPwmL, int pinPwmR);
	void task();
	void telemetry();

	void test();
	void stop();
	void move(int diraction);
	void rotate(int diraction);

	void setAzimuth(int azimuth, bool lock);
	void rotateTo(int azimuth);

private:
//methods
	static void countStepsL();
	static void countStepsR();
	static void checkMotorsSpeed();

//variables
	static int targetAzimuth;
	static bool azimuthLock;

	static Motor* motorLeft;
	static Motor* motorRight;
	static HMC5883L* compass;
};

#endif
