#ifndef EVENT_H
#define EVENT_H

#include "util.h"

enum {
	EVENT_TYPE_NONE = 0,
	EVENT_TYPE_BUTTON_DOWN = 1,
	EVENT_TYPE_BUTTON_UP = 2,
	EVENT_TYPE_JOYSTICK_1_Y_CHANGE = 3,
	EVENT_TYPE_JOYSTICK_1_X_CHANGE = 4
};

// This struct contains the options for
// the event engine
typedef struct {
	// If true, pollEvent will not hang when
	// there is no event to push
	bool eventNone;
} eventengine_t;

typedef struct {
	int type;
	int data;
} event_t;

// This is a private array needed by the pollEvent
// cycle to keep track of button states
bool buttonState[13] = {
	false, false, false,
	false, false, false,
	false, false, false,
	false, false, false,
	false
};
int joy1State[2] = { 0, 0 };
event_t eventStack[100];

void pushEvent(const event_t &event, event_t *stack)
{
	// Get the first available index
	int i = 0;
	for (;i < 100; i++) {
		if (stack[i].type == EVENT_TYPE_NONE)
			break;
	}

	stack[i] = event;
}

void popEvent(const event_t *stack, event_t *retVal)
{
	// Get the first available index
	int i = 0;
	for (;i < 100; i++) {
		if (stack[i].type == EVENT_TYPE_NONE)
			break;
	}

	if (i == 0) {
		PlaySound(soundException);
		return;
	}

	retVal->type = stack[i - 1].type;
	retVal->data = stack[i - 1].data;

	stack[i - 1].type = EVENT_TYPE_NONE;
	stack[i - 1].data = 0;
}

// TODO: implement stack usage & waiting
void pollEvent(eventengine_t *engine, event_t *event) {
	bool foundEvent = false;
	event_t newEvent; // the event to be pushed onto the stack

	for (int i = 0; i < sizeof(buttonState) / sizeof(bool); i++)
		// Button press checking here
		// TODO: Do I need to shift each button code +1?
	{
		if (joy1Btn(i)) {
			if (!buttonState[i]) {
				buttonState[i] = true;
				newEvent.type = EVENT_TYPE_BUTTON_DOWN;
				newEvent.data = i;
				foundEvent = true;
				pushEvent(newEvent, eventStack);
			}
		} else {
			if (buttonState[i]) {
				buttonState[i] = false;
				newEvent.type = EVENT_TYPE_BUTTON_UP;
				newEvent.data = i;
				foundEvent = true;
				pushEvent(newEvent, eventStack);
			}
		}
	}

	joystick_t joysticks[2];
	updateJoysticks(joysticks);
	if (joy1State[0] != joysticks[0].x) {
		joy1State[0] = joysticks[0].x;
		newEvent.type = EVENT_TYPE_JOYSTICK_1_X_CHANGE;
		newEvent.data = joysticks[0].x;
		foundEvent = true;
		pushEvent(newEvent, eventStack);
	} else if (joy1State[1] != joysticks[0].y) {
		joy1State[1] = joysticks[0].y;
		newEvent.type = EVENT_TYPE_JOYSTICK_1_Y_CHANGE;
		newEvent.data = joysticks[0].y;
		foundEvent = true;
		pushEvent(newEvent, eventStack);
	}

	if (foundEvent) {
		// Find the last event on the stack and return it
		popEvent(eventStack, event);
		return;
	} else {
		if (engine->eventNone) {
			event->type = EVENT_TYPE_NONE;
			event->data = 0;
			return;
		} else {
			pollEvent(engine, event);
		}
	}
}

#endif // EVENT_H
