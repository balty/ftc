#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorNormal, openLoop)

#include "JoystickDriver.c"

#define CONTROLLER_A 01

const float SLOW_CONSTANT = 0.35;

typedef struct {
	int x;
	int y;
} joystick_t;


task main()
{
	float movementConstant = 1.0;

  while (true)
  {
  	getJoystickSettings(joystick);

  	joystick_t joysticks[2]; // 0: Left   1: Right
    joysticks[1].x = joystick.joy1_x1 * 100.0 / 128.0;
  	joysticks[1].y = joystick.joy1_y1 * 100.0 / 128.0;
  	joysticks[0].x = joystick.joy1_x2 * 100.0 / 128.0;
  	joysticks[0].y = joystick.joy1_y2 * 100.0 / 128.0;

  	if (joy1Btn(CONTROLLER_A))
  	{
  		movementConstant = SLOW_CONSTANT;
  	}
  	else
  	{
  		movementConstant = 1.0;
  	}

  	motor[motorD] = joysticks[0].y * movementConstant;
  	motor[motorE] = joysticks[0].y * movementConstant;
  	motor[motorF] = joysticks[1].y * movementConstant;
  	motor[motorG] = joysticks[1].y * movementConstant;
	}
}
