#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IRSensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     IRSensor2,      sensorHiTechnicIRSeeker1200)
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

#include "util.h"

const int SPEED = 40;

void rotateLeft()
{
	motor[leftWheels] = -SPEED;
	motor[rightWheels] = SPEED;
}

void rotateRight()
{
	motor[leftWheels] = SPEED;
	motor[rightWheels] = -SPEED;
}

void stopRotation()
{
	motor[leftWheels] = 0;
	motor[rightWheels] = 0;
}

task main()
{
	while (true)
	{
		int val = SensorValue[IRSensor]

		if (val == 0)
		{
			rotateRight(); // just rotate
		}
		else if (val < 5)
		{
			rotateLeft();
		}
		else if (val > 5)
		{
			rotateRight();
		}
		else
		{
			stopRotation();
			PlaySound(soundShortBlip);
		}
	}
}
