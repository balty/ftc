#include "event.h"

task main()
{
	eventengine_t engine;
	event_t event;

	eventengine_init(engine);
	engine.eventNone = false;
	engine.controller1 = false;
	engine.controller2 = false;

	while (true) {
		pollEvent(&engine, &event);

		// Problem: we arent receiving the event code properly
		// probably problem with the stack
		switch (event.type)
		{
			case EVENT_TYPE_BUTTON_DOWN:
				PlaySound(soundDownwardTones);
				break;
			case EVENT_TYPE_BUTTON_UP:
				PlaySound(soundUpwardTones);
				break;
			case EVENT_TYPE_NONE:
				// do nothing
				break;
			default:
				PlaySound(soundException);
				StopAllTasks();
				break;
		}
	}
}
