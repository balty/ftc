#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Motor,  motorA,          clawTop,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          clawBottom,    tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     leftWheels,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     rightWheels,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    wrist,                tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "event.h"

task main()
{
	eventengine_t engine;
	event_t event;
	eventengine_init(&engine);

	engine.eventNone = true;
	engine.joysticks = false;
	engine.controller1 = true;
	engine.controller2 = false;

	int controllerPov;
	int conveyorSpeed = 0;
	int wristRotation = servo[wrist];

	while (true)
	{
		// Set wheel speeds
		motor[leftWheels] = (float) controllerValue(1, 1, 2) * .5;
		motor[rightWheels] = (float) controllerValue(1, 2, 2) * .5;

		// Move arm up/down and wrist up/down
		controllerPov = joystick.joy1_TopHat;
		nxtDisplayTextLine(0, "%d", controllerPov);
		if (controllerPov == DPAD_UP)
			motor[arm] = 30;
		else if (controllerPov == DPAD_DOWN)
			motor[arm] = -30;
		else
			motor[arm] = 0;

		if (controllerPov == DPAD_LEFT)
			wristRotation--;
		else if (controllerPov == DPAD_RIGHT)
			wristRotation++;
		servo[wrist] = wristRotation;

		// Conveyor claw and wrist controlls
		pollEvent(&engine, &event);
		switch (event.type)
		{
		case EVENT_TYPE_CONTROLLER_1_BUTTON_DOWN:
			if (event.data == CONTROLLER_L1)
				conveyorSpeed = 100;
			else if (event.data == CONTROLLER_R1)
				conveyorSpeed = -100;
			break;
		case EVENT_TYPE_CONTROLLER_1_BUTTON_UP:
			if (event.data == CONTROLLER_L1
				|| event.data == CONTROLLER_R1)
				conveyorSpeed = 0;
			break;
		}
		motor[clawTop] = conveyorSpeed;
		motor[clawBottom] = conveyorSpeed;
	}

}
