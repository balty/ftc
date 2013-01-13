#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S3,     IRSensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     IRSensor2,      sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     leftWheels,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     rightWheels,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorARM1,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorARM2,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    clawservo1,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    clawservo2,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)

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

	while (true)
	{
		// Set wheel speeds
		motor[leftWheels] = (float) controllerValue(1, 1, 2) * .5;
		motor[rightWheels] = (float) controllerValue(1, 2, 2) * .5;
	}

}
