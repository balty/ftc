#pragma config(Sensor, S1,     accel,          sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-accelerometer.h"

// Sensor setup
// Port 1: HiTechnic Acceleration sensor

task main()
{
	while (true)
	{
		// Read data
		int x, y, z;
		HTACreadAllAxes(accel, x, y, z);

		nxtDisplayTextLine(0, "accel:");
		nxtDisplayTextLine(1, "(%.3d, %.3d, %.3d)", x, y, z);
	}
}
