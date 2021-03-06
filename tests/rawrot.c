#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S3,     IRSensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     gyro,      sensorI2CHiTechnicGyro)
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

#include "../util.h"

task main()
{
	//rotateLeft(.75);
	motor[rightWheels] = 75;
	wait1Msec(5000);
	stopMovement();
}
