// The purpose of this file is to test the event
// engine using audio output

#include "event.h"

task main()
{
	eventengine_t engine;
	event_t event;

	eventengine_init(engine);
	engine.eventNone = false;
	engine.controller1 = true;
	engine.controller2 = false;

	while (true) {
		pollEvent(&engine, &event);
		nxtDisplayTextLine(0, "%d", event.type);
		switch (event.type)
		{
		case EVENT_TYPE_BUTTON_DOWN:
			PlaySound(soundDownwardTones);
			break;
		case EVENT_TYPE_BUTTON_UP:
			PlaySound(soundUpwardTones);
			break;
		case EVENT_TYPE_CONTROLLER_1_JOYSTICK_1_CHANGE_X:
			PlaySound(soundBeepBeep);
			break;
		case EVENT_TYPE_CONTROLLER_1_JOYSTICK_2_CHANGE_X:
			PlaySound(soundLowBuzz);
			break;
		default:

			break;
		}
	}
}
