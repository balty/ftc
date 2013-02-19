#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IRSensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     IRSensor2,      sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     leftWheels,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     rightWheels,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorARM1,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorARM2,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    clawservo1,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    clawservo2,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    sweeper,              tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "event.h"

#define ARM_SPEED_LOWER 100
#define ARM_SPEED_UPPER 50

// Angle definitions, copied from Teleop12002.c
const int SERVO_ANGLE_4 = 76;			// Servo angle for button 4 yellow 							base 100
const int SERVO_ANGLE_1 = 130;			// Servo angle for button 1 blue								base 130
const int SERVO_ANGLE_2 = 101;			// Servo angle for button 2 green								base 145
const int SERVO_ANGLE_3 = 60;				// Servo angle for button 3 red									base 30
const int SERVO_ANGLE_6 = 225;			// Servo angle for home position								base 225
const int SERVO_ANGLE_5 = 168;				// Servo angle for sweep position

const int ARM_1_COUNT_4 = 11215;		// arm 1 encoder for button 4 Top Rack yellow		base 13000
const int ARM_1_COUNT_1 = 5804;			// arm 1 encoder for button 1 Middle Rack blue	base 4700
const int ARM_1_COUNT_2 = 340;			// arm 1 encoder for button 2 Low Rack green		base 1000
const int ARM_1_COUNT_3 = 9357;			// arm 1 encoder for button 3 Floor red					base 8000
const int ARM_1_COUNT_6 = 0;				// arm 1 encoder for button 3 home position			base 0
const int ARM_1_COUNT_5 = 8976; 			// arm 1 encoder for joyclick - rack sweep position

const int ARM_2_COUNT_4 = 4115;			// arm 2 encoder for button 4 Top Rack yellow		base 4500
const int ARM_2_COUNT_1 = 2362;			// arm 2 encoder for button 1 Middle Rack blue	base 2250
const int ARM_2_COUNT_2 = 1434;			// arm 2 encoder for button 2 Low Rack green		base 1000
const int ARM_2_COUNT_3	= 6227;			// arm 2 encoder for button 3 Floor red					base 7000
const int ARM_2_COUNT_6	= 0;				// arm 2 encoder for button 6 home position			base 0
const int ARM_2_COUNT_5 = 4279;			// arm 2 encoder for joyclick - rack sweep position

task safeGuard();

task main()
{
	waitForStart();

	StartTask(safeGuard);

	eventengine_t engine;
	event_t event;
	eventengine_init(&engine);

	engine.eventNone = true;
	engine.joysticks = false;
	engine.controller1 = true;
	engine.controller2 = false;

	float driveSpeed = 1.0;
	int controllerPov;

	struct {
		int arm1_target; // target rotation for arm1 motor
		int arm1_state; // current rotation for arm1 motor
		int arm1_manual_lock; // true if manual controls are locking movement
		int arm2_target;
		int arm2_state;
		int arm2_manual_lock;

		bool arm1_manual_moving;
		bool arm2_manual_moving;
	} state_t;
	state_t state;

	memset(&state, 0, sizeof(state));

	// Reset servo values
	bFloatDuringInactiveMotorPWM = false; // the motors will NOT coast when power is not applied
	nMotorEncoder[motorARM1] = 0; // reset the Motor Encoder of Motor ARM 1
	nMotorEncoder[motorARM2] = 0; // reset the Motor Encoder of Motor ARM 2
	servo[clawservo1] = 180; // set servo to zero
	servo[clawservo2] = 180; // set servo to zero
	servo[sweeper] = 0;
	wait1Msec(500);

	while (true)
	{
		// Set wheel speeds
		motor[leftWheels] = (float) controllerValue(1, 1, 2) * driveSpeed;
		motor[rightWheels] = (float) controllerValue(1, 2, 2) * driveSpeed;

		// First, update states
		state.arm1_state = nMotorEncoder[motorARM1];
		state.arm2_state = nMotorEncoder[motorARM2];
		controllerPov = joystick.joy1_TopHat;

		// Manual joint and servo controls
		{
			// Lower joint controls
			if (joy1Btn(CONTROLLER_L1))
			{
				state.arm1_manual_lock = true;
				state.arm1_target = state.arm1_state;
				motor[motorARM1] = ARM_SPEED_LOWER;
			}
			else if (joy1Btn(CONTROLLER_L2))
			{
				state.arm1_manual_lock = true;
				state.arm1_target = state.arm1_state;
				motor[motorARM1] = -ARM_SPEED_LOWER;
			}
			else
			{
				// If it is locked, this means we can set speed to zero
				// without disrupting auto-rotation
				if (state.arm1_manual_lock)
					motor[motorARM1] = 0;

				// Don't disengage the lock - we don't want auto-adjust until
				// we hit the button
			}

			// Upper joint controlls
			if (joy1Btn(CONTROLLER_R1))
			{
				state.arm2_manual_lock = true;
				state.arm2_target = state.arm2_state;
				motor[motorARM2] = ARM_SPEED_UPPER;
			}
			else if (joy1Btn(CONTROLLER_R2))
			{
				state.arm2_manual_lock = true;
				state.arm2_target = state.arm2_state;
				motor[motorARM2] = -ARM_SPEED_UPPER;
			}
			else
			{
				if (state.arm2_manual_lock)
					motor[motorARM2] = 0;
			}

			// Manual servo controls
			if (controllerPov == DPAD_UP)
			{
				servo[clawservo1] = ServoValue[clawservo1] - 2;
				servo[clawservo2] = ServoValue[clawservo2] - 2;
			}
			else if (controllerPov == DPAD_DOWN)
			{
				servo[clawservo1] = ServoValue[clawservo1] + 2;
				servo[clawservo2] = ServoValue[clawservo2] + 2;
			}
			else if (controllerPov == DPAD_LEFT)
			{
				servo[sweeper] = ServoValue[sweeper] + 3;
			}
			else if (controllerPov == DPAD_RIGHT)
			{
				servo[sweeper] = ServoValue[sweeper] - 3;
			}
		}

		// Button press events
		pollEvent(&engine, &event);
		if (event.type == EVENT_TYPE_CONTROLLER_1_BUTTON_DOWN)
		{
			if (event.data == CONTROLLER_X)
			{
				// Middle row
				state.arm1_target = ARM_1_COUNT_1;
				state.arm2_target = ARM_2_COUNT_1;

				state.arm1_manual_lock = false;
				state.arm2_manual_lock = false;

				servo[clawservo1] = SERVO_ANGLE_1;
				servo[clawservo2] = SERVO_ANGLE_1;

				PlaySound(soundBeepBeep);
			}
			else if (event.data == CONTROLLER_A)
			{
				// Bottom row
				state.arm1_target = ARM_1_COUNT_2;
				state.arm2_target = ARM_2_COUNT_2;

				state.arm1_manual_lock = false;
				state.arm2_manual_lock = false;

				servo[clawservo1] = SERVO_ANGLE_2;
				servo[clawservo2] = SERVO_ANGLE_2;
			}
			else if (event.data == CONTROLLER_Y)
			{
				// Top row
				state.arm1_target = ARM_1_COUNT_4;
				state.arm2_target = ARM_2_COUNT_4;

				state.arm1_manual_lock = false;
				state.arm2_manual_lock = false;

				servo[clawservo1] = SERVO_ANGLE_4;
				servo[clawservo2] = SERVO_ANGLE_4;
			}
			else if (event.data == CONTROLLER_B)
			{
				// Pick-up position
				state.arm1_target = ARM_1_COUNT_3;
				state.arm2_target = ARM_2_COUNT_3;

				state.arm1_manual_lock = false;
				state.arm2_manual_lock = false;

				servo[clawservo1] = SERVO_ANGLE_3;
				servo[clawservo2] = SERVO_ANGLE_3;
			}
			else if (event.data == CONTROLLER_JOYCLICK_RIGHT)
			{
				// Sweep position
				state.arm1_target = ARM_1_COUNT_5;
				state.arm2_target = ARM_2_COUNT_5;

				state.arm1_manual_lock = false;
				state.arm2_manual_lock = false;

				servo[clawservo1] = SERVO_ANGLE_5;
				servo[clawservo2] = SERVO_ANGLE_5;
			}
			else if (event.data == CONTROLLER_JOYCLICK_LEFT)
			{
				driveSpeed = driveSpeed == SLOW_CONSTANT ? 1.0 : SLOW_CONSTANT;
			}
		}

		// Refresh some values for important accuracy
		state.arm1_state = nMotorEncoder[motorARM1];
		state.arm2_state = nMotorEncoder[motorARM2];

		// Rotate to the target locations if things arent locked
		if (!state.arm1_manual_lock)
		{
			if (abs(state.arm1_target - state.arm1_state) > 100)
				motor[motorARM1] = state.arm1_target > state.arm1_state ? ARM_SPEED_LOWER : -ARM_SPEED_LOWER;
			else
				motor[motorARM1] = 0;
		}

		if (!state.arm2_manual_lock)
		{
			if (abs(state.arm2_target - state.arm2_state) > 100)
				motor[motorARM2] = state.arm2_target > state.arm2_state ? ARM_SPEED_UPPER : -ARM_SPEED_UPPER;
			else
				motor[motorARM2] = 0;
		}

		// Display encoder values on the screen
		nxtDisplayTextLine(0, "L:%d", nMotorEncoder[motorARM1]);
		nxtDisplayTextLine(1, "U:%d", nMotorEncoder[motorARM2]);
		nxtDisplayTextLine(2, "S:%d", ServoValue[clawservo1]);
		nxtDisplayTextLine(3, "S2:%d", ServoValue[sweeper]);
	}
}

task safeGuard()
{
	while (true) {
		if (nMotorEncoder[motorARM1] >= 13171)
		{
			StopTask(main);
			motor[motorARM1] = 0;
			motor[motorARM2] = 0;
			PlaySound(soundException);
			StopAllTasks();
		}
	}
}
