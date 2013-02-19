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

#include "util.h"

task main()
{
	//waitForStart();

	servo[sweeper] = 169;
	servo[clawservo1] = 240;
	servo[clawservo2] = 240;
	wait1Msec(1000);

	forward(.6);
	wait1Msec(600);
	stopMovement();

	servo[sweeper] = 0;
	wait1Msec(1000);

	rotateRight();
	wait1Msec(300);
	forward();
	wait1Msec(300);
	rotateLeft();
	wait1Msec(300);
	forward();
	wait1Msec(1400);
	rotateLeft();
	wait1Msec(300);
	forward();
	wait1Msec(300);
	stopMovement();

	forward(.5);
	wait1Msec(500);
	while (true)
	{
		forward(-0.5);
		wait1Msec(1000);
		forward(0.63);
		wait1Msec(1000);
	}
}
