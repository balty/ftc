#pragma config(Sensor, S1,     IRSeeker,       sensorHiTechnicIRSeeker1200)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	while (1 == 1)
	{
		int dir = SensorValue[IRSeeker];
		nxtDisplayTextLine(1, "%d", dir);
	}
}