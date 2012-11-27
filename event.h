#ifndef EVENT_H
#define EVENT_H

#include "util.h"

enum {
	EVENT_TYPE_BUTTON_DOWN = 1,
	EVENT_TYPE_BUTTON_UP = 2,
	EVENT_TYPE_JOYSTICK_1_Y_CHANGE = 3,
	EVENT_TYPE_JOYSTICK_1_X_CHANGE = 4
};

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

void pollEvent(event_t *event) {
	for (int i = 0; i < sizeof(buttonState) / sizeof(bool); i++)
		// Button press checking here
		// TODO: Do I need to shift each button code +1?
	{
		if (joy1Btn(i)) {
			if (!buttonState[i]) {
				buttonState[i] = true;
				event->type = EVENT_TYPE_BUTTON_DOWN;
				event->data = i;
				return;
			}
		} else {
			if (buttonState[i]) {
				buttonState[i] = false;
				event->type = EVENT_TYPE_BUTTON_UP;
				event->data = i;
				return;
			}
		}
	}
}

#endif // EVENT_H
