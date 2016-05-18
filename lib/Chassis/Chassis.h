#ifndef Chassis_h
#define Chassis_h
#include "Arduino.h"
#include "HMC5883L.h"
#include "Motor.h"
#include "Command.h"
#include "CircularBuffer.h"
#include "Squircle.h"
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
	void move(int16_t x, int16_t y);

	void setAzimuth(int azimuth, bool lock);
	void rotateTo(int azimuth);

private:
//methods
	static void countStepsL();
	static void countStepsR();
	static void checkMotorsSpeed();
	static void fill(int16_t& l, int16_t& r, int16_t minL, int16_t maxL, int16_t minR, int16_t maxR);
	static int8_t percents(int16_t l, int16_t r);
	static void xy2lr(int16_t x, int16_t y, int16_t& l, int16_t& r);

//variables
	static int targetAzimuth;
	static bool azimuthLock;

	static Motor* motorLeft;
	static Motor* motorRight;
	static HMC5883L* compass;
};

#endif
