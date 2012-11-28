#include "event.h"

task main()
{
	eventengine_t engine;
	event_t event;

	engine.eventNone = false;

	while (true) {
		pollEvent(&engine, &event);
		if (event.type == EVENT_TYPE_NONE)
			PlaySound(soundShortBlip);
		else
			PlaySound(soundBlip);
	}
}
