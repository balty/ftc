#ifndef LIB12_H
#define LIB12_H
#define radiansToDegrees(x) (x * (float) 180 / PI)

/* where gyro sensor point up, with white part
is furthest from locus of movement*/

float angleFromAccel(tSensors sensor)
	{
	int x, y, z;				// integer variable x,y,z
	float angle; 				// floating variable angle
	HTACreadAllAxes(sensor, x, y, z);	// read sensor raw value

	angle = atan((float) z / (float) x);	// calculate angle

	//angle = acos((float) z / 200);	// same calc different formula

	return radiansToDegrees(angle);	// convert to degrees
	}

#endif
