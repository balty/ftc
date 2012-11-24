#ifndef LIB52_H
#define LIB52_H

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

typedef struct {
  int x;
  int y;
} joystick_t;

// Joystick variables:
// 0: left		1: right
// Value out of 100
joystick_t joysticks[2];

void updateJoysticks() {
	getJoystickSettings(joystick);
	joysticks[0].x = joystick.joy1_x1 * 100.0 / 128.0;
  joysticks[0].y = joystick.joy1_y1 * 100.0 / 128.0;
  joysticks[1].x = joystick.joy1_x2 * 100.0 / 128.0;
  joysticks[1].y = joystick.joy1_y2 * 100.0 / 128.0;

  for (int i = 0; i < 2; i++)
  {
  	if (abs(joysticks[i].y) < 5)
  		joysticks[i].y = 0;
  	if (abs(joysticks[i].x) < 5)
  		joysticks[i].x = 0;
  }
}

float angleFromGyro(tSensors sensor)
// See diagrams: angle = arctan(z/x),
// where gyro sensor faces up, with white part
// is furthest from locus of movement
{
	int x, y, z;
	float angle; // in radians
	HTACreadAllAxes(sensor, x, y, z);
	angle = atan((float) z / (float) x);
	return radiansToDegrees(angle);
}

// Here I will be building a state engine.
// The purpose of this is to centralize code between
// autonomous and manual programs
// You will put the engine run call at the beginning/end of
// the run loop
// Parameters will be stored inside the engine struct, which acts
// as a non-opaque handle
typedef struct {

} engine_t;

void engine_fire(engine_t *engine)
{

}

#endif
