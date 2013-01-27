#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S3,     IRSensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     gyro,      sensorAnalogInactive)
#pragma config(Motor,  mtr_S1_C1_1,     leftWheels,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     rightWheels,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorARM1,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorARM2,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    clawservo1,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    clawservo2,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "util.h"
#include "drivers/hitechnic-gyro.h"

float fabs(float in)
{
	if (in >= 0)
		return in;
	else
		return -in;
}

task main()
{
	// Drive forward until we are perpendicular
	// TODO: check to see if this works on both sides (may not)
	forward(.5);
	while (SensorValue[IRSensor] != 2
		&& SensorValue[IRSensor] != 8)
		;
	stopMovement();
	wait1Msec(1000);

	float heading = 0;
	float rotSpeed = 0;

	HTGYROstartCal(gyro);

	rotateLeft(.5);
	time1[T1] = 0;
	while (true)
	{
		while (time1[T1] < 20)
			;

		time1[T1] = 0;

		rotSpeed = HTGYROreadRot(gyro);
		heading += rotSpeed * 20 / 1000; // magic offset value
		if (heading >= 85
			|| heading <= -85)
			break;
	}
	stopMovement();

	unpackArm();
	forward(.5);
	wait1Msec(2500);
}
