#pragma config(Sensor, S1,     gyro,           sensorI2CHiTechnicGyro)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	int gyroVal;
	int adjVal;
	int calibConst = SensorValue[gyro] - 620;

	while (true)
	{
		gyroVal = SensorValue[gyro];
		adjVal = gyroVal - 620 - calibConst;
		nxtDisplayTextLine(0, "G:%d", gyroVal);
		nxtDisplayTextLine(1, "G-adj:%d", adjVal);
		nxtDisplayTextLine(2, "t-90:%f", 90.0 / (float) adjVal);
	}
}
