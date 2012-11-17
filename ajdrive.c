#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorNormal, openLoop)

#include "JoystickDriver.c"
#include "lib52.h"

task main()
{
  float movementConstant = 1.0;

  while (true)
  {
    updateJoysticks();

    if (joy1Btn(CONTROLLER_A))
    {
      movementConstant = SLOW_CONSTANT;
    }
    else
    {
      movementConstant = 1.0;
    }

    // Motors D and E are actually on the right side - use joysticks[1]
    // F and G are on the left - use joysticks[0]
    motor[motorD] = joysticks[1].y * movementConstant;
    motor[motorE] = joysticks[1].y * movementConstant;
    motor[motorF] = joysticks[0].y * movementConstant;
    motor[motorG] = joysticks[0].y * movementConstant;
  }
}
