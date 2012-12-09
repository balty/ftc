// The purpose of this file is to test the event
// engine using audio output

#include "event.h"

task main()
{
	eventengine_t engine;
	event_t event;

	eventengine_init(engine);
	engine.eventNone = true;
	engine.controller1 = true;
	engine.controller2 = false;

	while (true) {
		int count = 0;
		ClearTimer(T1);
		while (time1[T1] < 1000) {
			pollEvent(&engine, &event);
			count++;
		}
		nxtDisplayTextLine(0, "eps:%d", count);
		count = 0;
	}

	/*while (true) {
		pollEvent(&engine, &event);
		nxtDisplayTextLine(0, "%d", event.type);
		switch (event.type)
		{
		case EVENT_TYPE_CONTROLLER_1_BUTTON_DOWN:
			PlaySound(soundDownwardTones);
			break;
		case EVENT_TYPE_CONTROLLER_1_BUTTON_UP:
			PlaySound(soundUpwardTones);
			break;
		case EVENT_TYPE_CONTROLLER_1_JOYSTICK_1_CHANGE_X:
			PlaySound(soundBlip);
			break;
		case EVENT_TYPE_CONTROLLER_1_JOYSTICK_2_CHANGE_X:
			PlaySound(soundLowBuzz);
			break;
		default:

			break;
		}
	}*/
}
