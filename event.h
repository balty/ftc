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

int eventCount(event_t *stack)
{
	for (int i = 0; i < 100; i++)
	{
		if (stack[i].type == 0
			|| stack[i].type == EVENT_TYPE_NONE)
			return i;
	}

	return 100;
}

void pushEvent(const event_t &event, event_t *stack)
{
	int count = eventCount(stack);

	stack[count] = event;
}

// Pops from the bottom of the stack, not the top
void popBottomEvent(const event_t *stack, event_t *retVal)
{
	int count = eventCount(stack);

	*retVal = stack[0];

	// Move the rest of the events down
	int i = 0;
	int j = 1;
	while (i < count - 1)
	{
		stack[i] = stack[j];
		i++;
		j++;
	}
}

bool pollEvent(eventengine_t *engine, event_t *event) {
	// I have to do this loop because the C
	// implementation does not allow recursion
	while (true) {
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
			popBottomEvent(eventStack, event);
			return true;
		} else {
			if (engine->eventNone) {
				event->type = EVENT_TYPE_NONE;
				event->data = 0;
				return true;
			} else {
				continue;
			}
		}
	}
}

#endif // EVENT_H
