#ifndef UTIL_H
#define UTIL_H

#include "JoystickDriver.c"
#include "drivers/hitechnic-accelerometer.h"

#define CONTROLLER_A 02
#define CONTROLLER_X 01
#define CONTROLLER_B 03
#define CONTROLLER_Y 04
#define CONTROLLER_R1 06
#define CONTROLLER_R2 08
#define CONTROLLER_L1 05
#define CONTROLLER_L2 07
#define CONTROLLER_JOYCLICK_LEFT 11
#define CONTROLLER_JOYCLICK_RIGHT 12

#define DPAD_UP 0
#define DPAD_DOWN 4
#define DPAD_LEFT 6
#define DPAD_RIGHT 2

#define SPEED 40
#define SLOW_CONSTANT 0.35
#define COLOR_BLACK 0

#define degreesToRadians(x) (x * PI / 180)
#define radiansToDegrees(x) (x * (float) 180 / PI)

float angleFromGyro(tSensors sensor)
// Function to grab the angle of the
// acceleration sensor, presumable mounted
// on the arm.
// Return Value:
// - the angle, in degrees, of the arm from the horizontal
// Parameters:
// - sensor: a pointer to the sensor to read the
//           acceleration values from
//
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
// Grab a joystick value from the controller state
// Return Value:
// - the requested state value
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
			if (axis == 1) {
				val = joystick.joy1_x1;
				} else {
				val = joystick.joy1_y1;
			}
		}
		else {
			if (axis == 1) {
				val = joystick.joy1_x2;
				} else {
				val = joystick.joy1_y2;
			}
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

	val = (float) val * 100.0 / 128.0;

	if (val > -10
		&& val < 10)
	val = 0;

	return val;
}

void forward(float coefficient = 1.0)
// Move the robot forward
// Parameters:
// - coefficient: the multiplier for the speed
{
	motor[leftWheels] = SPEED * coefficient;
	motor[rightWheels] = SPEED * coefficient;
}

void rotateLeft(float coefficient = 1.0)
// Rotate the robot left
// Parameters:
// - coefficient: the multiplier for the speed
{
	motor[leftWheels] = (float) -SPEED * coefficient;
	motor[rightWheels] = (float) SPEED * coefficient;
}

void rotateRight(float coefficient = 1.0)
// Rotate the robot right
// Parameters:
// - coefficient: the multiplier for the speed
{
	motor[leftWheels] = (float) SPEED * coefficient;
	motor[rightWheels] = (float) -SPEED * coefficient;
}

void stopMovement()
// Stop the robot
{
	motor[leftWheels] = 0;
	motor[rightWheels] = 0;
}

int getIR(int which)
// Get values from the ir sensors
// Return value:
// - the requested zone value from the sensor
// Parameters:
// - which: which sensor to read from
//          If this value is zero, it will read from the
//          sensor labeled 'IRSensor.'
//          Otherwise, it will read from the sensor labeled
//          'IRSensor2.'
{
	if (!which)
		return SensorValue[IRSensor];
	else
		return SensorValue[IRSensor2];
}

int getIRSum()
// Get the sum of both IR sensor values
{
	return getIR(0) + getIR(1);
}

void unpackArm()
// Reset encoder values, and unpack the arm
{
	// Start by axing values and resetting to zero
	bFloatDuringInactiveMotorPWM = false; // the motors will NOT coast when power is not applied
	nMotorEncoder[motorARM1] = 0; // reset the Motor Encoder of Motor ARM 1
	nMotorEncoder[motorARM2] = 0; // reset the Motor Encoder of Motor ARM 2
	servo[clawservo1] = 180; // set servo to zero
	servo[clawservo2] = 180; // set servo to zero
	wait1Msec(500);

	// Set up the arm for the middle row
	motor[motorARM1] = 100;
	motor[motorARM2] = 100;
	servo[clawservo1] = 110;
	servo[clawservo2] = 110;
	while (true) {
		if (motor[motorARM1] == 0
			&& motor[motorARM2] == 0)
		break;

		if (nMotorEncoder[motorARM1] >= 4700)
			motor[motorARM1] = 0;
		if (nMotorEncoder[motorARM2] >= 1925)
			motor[motorARM2] = 0;
	}
}

#endif // UTIL_H
