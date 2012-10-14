#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorNormal, openLoop)

#include "JoystickDriver.c"

#define CONTROLLER_A 01

task main()
{
  float fmot1,fmot2;    // Declare float variables
  int imot1, imot2;     // Declare integer variables
  bool slow = false;

  while(true)                            // Infinite loop:
  {
    getJoystickSettings(joystick);       // This step is required for the Joystick to work

    if (joy1Btn(CONTROLLER_A)) {
    	slow = !slow;
    	continue;
  	}

    fmot2 = (float) joystick.joy1_y1 * 100. / 128.;   // Convert the 1~128 value from the y value of left joystick into 1~100
    fmot1 = (float) joystick.joy1_y2 * 100. / 128.;   // Convert the 1~128 value from the y value of right joystick into 1~100

    imot1 = (int) fmot1;   // Convert float into integer for motor
    imot2 = (int) fmot2;

    if (slow) {
    	imot1 /= 2;
    	imot2 /= 2;
  	}

    if( imot2 >= -3 && imot2 <= 3 )   // If the motor value is +-3 from 0, it is considered 0
    {
      motor[motorF] = 0;
      motor[motorG] = 0;
    }
    else
    {
      motor[motorF] = imot2;          // Else, the imot2 from line 38 is assigned
      motor[motorG] = imot2;
    }

    if( imot1 >= -3 && imot1 <= 3 )   // If the motor value is +-3 from 0, it is considered 0
    {
      motor[motorE] = 0;
      motor[motorD] = 0;
    }

    else
    {
      motor[motorE] = imot1;          // Else, the imot1 from line 37 is assigned
      motor[motorD] = imot1;
    }

  }
}
