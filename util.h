#ifndef UTIL_H
#define UTIL_H

#include "JoystickDriver.c"
#include "drivers/hitechnic-accelerometer.h"

#define CONTROLLER_A 02
#define CONTROLLER_X 01
#define CONTROLLER_R1 06
#define CONTROLLER_R2 08
#define CONTROLLER_L1 05
#define CONTROLLER_L2 07

#define DPAD_UP 0
#define DPAD_DOWN 4
#define DPAD_LEFT 6
#define DPAD_RIGHT 2

#define SLOW_CONSTANT 0.35
#define COLOR_BLACK 0

#define degreesToRadians(x) (x * PI / 180)
#define radiansToDegrees(x) (x * (float) 180 / PI)

float angleFromGyro(tSensors sensor)
// See diagrams: angle = arctan(z/x),
// where gyro sensor faces up, with white part
// furthest from locus of movement
{
	int x, y, z;
	float angle; // in radians
	HTACreadAllAxes(sensor, x, y, z);
	angle = atan((float) z / (float) x);
	return radiansToDegrees(angle);
}

int controllerValue(int controller, int joy, int axis)
// Parameters:
// - controller: controller 1 or 2
// - joy: joystick 1 or 2
// - axis: 1 = x, 2 = y
{
	int val = 0;
	getJoystickSettings(joystick);

	if (controller == 1)
	{
		if (joy == 1) {
			if (axis == 1)
				val = joystick.joy1_x1;
			else
				val = joystick.joy1_y1;
		}
		else {
			if (axis == 2)
				val = joystick.joy1_x2;
			else
				val = joystick.joy1_y2;
		}
	}
	else if (controller == 2)
	{
		if (joy == 1) {
			if (axis == 1)
				val = joystick.joy2_x1;
			else
				val = joystick.joy2_y1;
		}
		else {
			if (axis == 1)
				val = joystick.joy2_x2;
			else
				val = joystick.joy2_y2;
		}
	}
	else
	{
		PlaySound(soundException);
		StopAllTasks();
	}

	if (val > -5
		&& val < 5)
		val = 0;

	return val;
}

#endif // UTIL_H
