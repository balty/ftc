#ifndef LIB52_H
#define LIB52_H

#include "JoystickDriver.c"

#define CONTROLLER_A 01
#define SLOW_CONSTANT 0.35

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

#endif
