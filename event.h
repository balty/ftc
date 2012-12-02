// This file houses the logic for the event engine
// To create and use an instance of the event engine:
// - eventengine_t engine;
// - event_t event;
// - eventengine_init(&engine);
// - (set engine parameters using member variables)
// - pollEvent(&engine, &event);
//
// Here is a rundown of the code above:
// eventengine_t ---- the "handle" of an engine instance.
//                    You also use this struct to control the
//                    runtime of the engine through member
//                    variables (options).
//                    You can have any number of these in
//                    an application.
// event_t ---------- the "handle" of an event that the engine
//                    will return.  This struct contains the type
//                    and value of the event.
// eventengine_init - this initializes an engine structure to all
//                    zeros to eliminate any garbage data that
//                    can interfere with the cycle
// ...parameters... - Here is where you set the options for the engine
//                    Look at the documentation of the eventengine_t
//                    struct if you want to see a list of options and
//                    what each one means.  For example:
//                        engine.eventNone = true;
//                            this tells the engine that it is okay
//                            to return an empty event instead of waiting
//                            for something to happen
// pollEvent(...) --- This initiates the engine logic.  The engine will
//                    search for events based on the parameters that
//                    you set.  If engine.eventNone == true, the engine
//                    will not wait for an event even if nothing has happened
//                    since the last call; it will return an empty event.
//                    This function will return the detected event.


#ifndef EVENT_H
#define EVENT_H

#include "util.h"

// This enumeration houses the numerical codes for all
// the different types of events.  Notice that EVENT_TYPE_NONE
// is 0.  Please do not ever change this, it allows for
// convenience and if changed may result in some code being broken.
enum {
	EVENT_TYPE_NONE = 0,
	EVENT_TYPE_BUTTON_DOWN = 1,
	EVENT_TYPE_BUTTON_UP = 2,
	EVENT_TYPE_CONTROLLER_1_JOYSTICK_1_CHANGE_X = 3,
	EVENT_TYPE_CONTROLLER_1_JOYSTICK_1_CHANGE_Y = 4,
	EVENT_TYPE_CONTROLLER_1_JOYSTICK_2_CHANGE_X = 5,
	EVENT_TYPE_CONTROLLER_1_JOYSTICK_2_CHANGE_Y = 6,
	EVENT_TYPE_CONTROLLER_2_JOYSTICK_1_CHANGE_X = 7,
	EVENT_TYPE_CONTROLLER_2_JOYSTICK_1_CHANGE_Y = 8,
	EVENT_TYPE_CONTROLLER_2_JOYSTICK_2_CHANGE_X = 9,
	EVENT_TYPE_CONTROLLER_2_JOYSTICK_2_CHANGE_Y = 10,
};

// This is the structure for a raw event
// Members:
// - type: the numerical type of the event.
//         see the above EVENT_TYPE enumeration
// - data: the value associated with the event.
//         Example: you move the joystick to the top.
//         event.data will contain the value '100.'
typedef struct {
	short type;
	int data;
} event_t;

// TODO: move all state information into here
// This struct contains the options for
// the event engine
// Member variables:
// - eventStack: contains the stack of events that
//               were detected.  This is for internal
//               use only and should not be changed
//               by the program.
// - eventNone: if set to 'true,' pollEvent will not wait
//              for another event to occur if it could not
//              find one; it will simply return an empty
//              event.
// - controller1: if set to 'true,' pollEvent will probe for
//                joystick and button events on controller 1
// - controller2: if set to 'true,' pollEvent will probe for
//                joystick and button events on controller 2
typedef struct {
	event_t eventStack[100];
	bool eventNone;
	bool controller1;
	bool controller2;
} eventengine_t;

// This is a private array needed by the pollEvent
// cycle to keep track of button states
bool buttonState[13] = {
	false, false, false,
	false, false, false,
	false, false, false,
	false, false, false,
	false
};
int c1State[2][2] = {
	0, 0,
	0, 0
}; // state for controller 1
// format: [joystick][axis]
int c2State[2][2] = {
	0, 0,
	0, 0
}; // state for controller 2
// format: [joystick][axis]

void eventengine_init(eventengine_t *engine)
// This function initializes an engine so
// that it contains zeros instead of
// garbage
{
	memset(engine, 0, sizeof(eventengine_t));
}

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

	retVal->type = stack[0].type;
	retVal->data = stack[0].data;
	memset(&(stack[0]), 0, sizeof(event_t));

	// Move the rest of the events down
	int i = 0;
	int j = 1;
	while (i < count)
	{
		stack[i] = stack[j];
		memset(&(stack[j]), 0, sizeof(event_t));
		i++;
		j++;
	}
}

bool pollEvent(eventengine_t *engine, event_t *event) {
	// I have to do this loop because the C
	// implementation does not allow recursion
	while (true) {
		bool foundEvent = false;
		event_t newEvent; // temp event to be copied onto the stack

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
					pushEvent(newEvent, engine->eventStack);

				}
				} else {
				if (buttonState[i]) {
					buttonState[i] = false;
					newEvent.type = EVENT_TYPE_BUTTON_UP;
					newEvent.data = i;
					foundEvent = true;
					pushEvent(newEvent, engine->eventStack);

				}
			}
		}

		// Joystick events
		{
			int c1Now[2][2] = { 0, 0, 0, 0 };
			int c2Now[2][2] = { 0, 0, 0, 0 };

			if (engine->controller1) {
				c1Now[0][0] = controllerValue(1, 1, 1);
				c1Now[0][1] = controllerValue(1, 1, 2);
				c1Now[1][0] = controllerValue(1, 2, 1);
				c1Now[1][1] = controllerValue(1, 2, 2);
			}

			if (engine->controller2) {
				c2Now[0][0] = controllerValue(2, 1, 1);
				c2Now[0][1] = controllerValue(2, 1, 2);
				c2Now[1][0] = controllerValue(2, 2, 1);
				c2Now[1][1] = controllerValue(2, 2, 2);
			}

			// Check controller 1
			for (int joy = 0; engine->controller1 && joy < 2; joy++)
			{
				for (int axis = 0; axis < 2; axis++)
				{
					if (c1Now[joy][axis] != c1State[joy][axis])
					{
						// Find which event and push it onto the stack
						// Then update joystick state
						foundEvent = true;

						if (joy == 0) {
							if (axis == 0)
								newEvent.type = EVENT_TYPE_CONTROLLER_1_JOYSTICK_1_CHANGE_X;
							else
								newEvent.type = EVENT_TYPE_CONTROLLER_1_JOYSTICK_1_CHANGE_Y;
						}
						else { // (joy == 1)
							if (axis == 0)
								newEvent.type = EVENT_TYPE_CONTROLLER_1_JOYSTICK_2_CHANGE_X;
							else
								newEvent.type = EVENT_TYPE_CONTROLLER_1_JOYSTICK_2_CHANGE_Y;
						}

						c1State[joy][axis] = c1Now[joy][axis];
						newEvent.data = c1Now[joy][axis];
						pushEvent(newEvent, engine->eventStack);
					}
				}
			}

			// Check controller 2
			for (int joy = 0; engine->controller2 && joy < 2; joy++)
				for (int axis = 0; axis < 2; axis++) {
				if (c2Now[joy][axis] != c2State[joy][axis])
				{
					// Find which event and push it
					// Then update joystick state
					foundEvent = true;

					if (joy == 0) {
						if (axis == 0)
							newEvent.type = EVENT_TYPE_CONTROLLER_2_JOYSTICK_1_CHANGE_X;
						else
							newEvent.type = EVENT_TYPE_CONTROLLER_2_JOYSTICK_1_CHANGE_Y;
					}
					else { // (joy == 1)
						if (axis == 0)
							newEvent.type = EVENT_TYPE_CONTROLLER_2_JOYSTICK_2_CHANGE_X;
						else
							newEvent.type = EVENT_TYPE_CONTROLLER_2_JOYSTICK_2_CHANGE_Y;
					}

					c2State[joy][axis] = c2Now[joy][axis];
					newEvent.data = c2Now[joy][axis];
					pushEvent(newEvent, engine->eventStack);
				}
			}
		}

		if (foundEvent) {
			// Find the last event on the stack and return it
			popBottomEvent(engine->eventStack, event);
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
