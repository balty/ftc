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

#define MAX_EVENTS 100

// This enumeration houses the numerical codes for all
// the different types of events.  Notice that EVENT_TYPE_NONE
// is 0.  Please do not ever change this, it allows for
// convenience and if changed may result in some code being broken.
enum {
	EVENT_TYPE_NONE = 0,
	EVENT_TYPE_CONTROLLER_1_BUTTON_DOWN = 1,
	EVENT_TYPE_CONTROLLER_1_BUTTON_UP = 2,
	EVENT_TYPE_CONTROLLER_2_BUTTON_DOWN = 11,
	EVENT_TYPE_CONTROLLER_2_BUTTON_UP = 12,
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
// - next: for internal use only
struct {
	short type;
	int data;
	void *next;
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
// - joysticks: if set to 'true,' pollEvent will probe for
//              joystick events on the given controllers.
//              see controller1 & controller2
// - controller1: if set to 'true,' pollEvent will probe for
//                button events on controller 1
// - controller2: if set to 'true,' pollEvent will probe for
//                button events on controller 2
typedef struct {
	event_t eventPool[MAX_EVENTS];
	event_t *first; // pointer to the current first event

	bool eventNone;
	bool joysticks;
	bool controller1;
	bool controller2;
} eventengine_t;


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// Here lies the code for the "event queue"
// Here is how it works:
// eventengine_init: this zero's out all events in the pool and
//                   links them all together in a chain, with the
//                   last one being linked to NULL
// adding an event: find the first unused event (usually not the
//                  last one) and modify the values inside
// removing an event: update the pointer to the list start to
//                    point to the second event.  Set the consumed
//                    event's 'next' field to NULL so that when put
//                    on the back of the queue, it will be identified
//                    as the last.  Set the type to zero so that it
//                    will be identified as empty.  Add the consumed
//                    event to the end of the list.

void eventengine_init(eventengine_t *engine)
// Initialize the event engine so that we don't have a bunch
// of garbage values that mess things up.  This also links
// the events in the engine's event pool together in a chain
{
	// Zero-out all member variables of the engine
	engine->eventNone = false;
	engine->controller1 = false;
	engine->controller2 = false;

	// Zero-out all the events in the engine
	for (int i = 0; i < MAX_EVENTS; i++) {
		engine->eventPool[i].type = 0;
		engine->eventPool[i].data = 0;
		engine->eventPool[i].next = NULL;
	}

	// Link the events in a chain
	for (int i = 0; i < MAX_EVENTS - 1; i++)
		engine->eventPool[i].next = &(engine->eventPool[i + 1]);

	// Set up the pointer to the first element
	engine->first = &(engine->eventPool[0]);
}

event_t *lastEvent(eventengine_t *engine)
// Returns the event that has no 'next' link
{
	event_t *ret = engine->first;
	while (ret->next != 0)
		ret = ret->next;
	return ret;
}

event_t *lastUsedEvent(eventengine_t *engine)
// Return Value: the last event that contains a valid type code
//    If the return value is NULL, that means that the last unused
//    event is engine->first, which means that there are no events
{
	event_t *ret = engine->first;
	while (ret->type != 0)
		ret = ret->next;

	if (ret == engine->first)
	{
		// This means that the first event is not taken
		return NULL;
	}

	return ret;
}

void addEvent(eventengine_t *engine, const event_t &event)
// Add an event to the list.
// Here is how it works:
//     It finds the first unused event.  If the function 'lastUsedEvent()'
//     returns NULL, then the first unused event is the engine->first pointer.
//     It then simply modifies the values in the first unused event.
{
	// Find the first unused event and copy values into there
	// Note that there is no segfault guard
	event_t *parent = lastUsedEvent(engine);
	event_t *next = (event_t *) parent->next;
	if (parent == NULL) // if the first event isnt taken
		next = engine->first;
	next->type = event.type;
	next->data = event.data;

	nxtDisplayTextLine(2, "add: %d", event.type);
}

void addEvent_replace(eventengine_t *engine, const event_t &event)
// This function is used for joystick events.  Instead of inserting
// a brand-new event onto the stack, we have to see if there is
// another joystick event there because joystick events are
// generated much faster than they can be consumed.
{
	// search for an event of the same type in the queue
	// we don't have to search all the events, stop when you
	// hit one with a type of 0 because they will all be the same
	// afterwards
	for (event_t *ev = engine->first;
		ev->type != 0;
		ev = ev->next)
	{
		if (ev->type == event.type) {
			ev->data = event.type;
			break;
		}
	}

	// If we got here, that means that we couldn't find an event of
	// the same type
	addEvent(engine, event);
}

void consumeEvent(eventengine_t *engine, event_t *ret)
// This function deletes the first event from the list
// and recycles it to the back of the list, returning the
// values of the deleted event inside 'ret'
{

	// Consume the event, destroying it and adding it
	// to the recycle list
	event_t *consumed = engine->first;
	engine->first = consumed->next;

	// Copy the values
	ret->type = consumed->type;
	ret->data = consumed->data;

	// Blank the consumed event
	consumed->next = NULL;
	consumed->type = 0;
	lastEvent(engine)->next = consumed;

	nxtDisplayTextLine(3, "consume:%d", ret->type);
}


// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------
// Here lies the code for the pollEvent cycle, including
// state variables.

// This is a private array needed by the pollEvent
// cycle to keep track of button states
bool buttonState1[13] = {
	true, false, false,
	false, false, false,
	false, false, false,
	false, false, false,
	false
};

bool buttonState2[13] = {
	false, false, false,
	false, false, false,
	false, false, false,
	false, false, false,
	false
};

// state for controller 1
// format: [joystick][axis]
int c1State[2][2] = {
	1, 1,
	1, 1
};

// state for controller 2
// format: [joystick][axis]
int c2State[2][2] = {
	1, 1,
	1, 1
};

void pollEvent(eventengine_t *engine, event_t *event)
// This is pretty much the main function of the engine :)
// It reads the options from the 'engine' parameter and then
// searches for events.  It will always return events in the
// order in which it receives them.  Note that not all joystick
// events will be reported.  This is because the fingers are capable
// of generating far too many (much more than can be consumed by the program).
// Instead of recording each joystick move, the function just finds the most
// recent joystick event of the same type and then modifies the 'data'
// member field.
//
// Return Value:
// - DISCARD THIS RETURN VALUE!  I put it here b/c I may use it in the future
// Parameters:
// - engine: a pointer to the eventengine_t struct containing the engine options
// - event: a pointer to the event_t struct whose data we will be modifying
{
	// I have to do this loop because the C
	// implementation does not allow recursion
	while (true)
	{
		bool foundEvent = false;
		event_t newEvent; // temp event to be copied onto the stack

		if (engine->controller1)
		{
			for (int i = 0; i < sizeof(buttonState1) / sizeof(bool); i++)
			{
				// Button press checking here
				// TODO: Do I need to shift each button code +1?
				if (joy1Btn(i))
				{
					if (!buttonState1[i])
					{
						buttonState1[i] = true;
						newEvent.type = EVENT_TYPE_CONTROLLER_1_BUTTON_DOWN;
						newEvent.data = i;
						foundEvent = true;
						addEvent(engine, newEvent);
					}
				}
				else
				{
					if (buttonState1[i])
					{
						buttonState1[i] = false;
						newEvent.type = EVENT_TYPE_CONTROLLER_1_BUTTON_UP;
						newEvent.data = i;
						foundEvent = true;
						addEvent(engine, newEvent);
					}
				}
			}
		}

		if (engine->controller2)
		{
			for (int i = 0; i < sizeof(buttonState2) / sizeof(bool); i++)
				// Button press checking here
			// TODO: Do I need to shift each button code +1?
			{
				if (joy2Btn(i))
				{
					if (!buttonState2[i])
					{
						buttonState2[i] = true;
						newEvent.type = EVENT_TYPE_CONTROLLER_2_BUTTON_DOWN;
						newEvent.data = i;
						foundEvent = true;
						addEvent(engine, newEvent);

					}
				}
				else
				{
					if (buttonState2[i])
					{
						buttonState2[i] = false;
						newEvent.type = EVENT_TYPE_CONTROLLER_2_BUTTON_UP;
						newEvent.data = i;
						foundEvent = true;
						addEvent(engine, newEvent);

					}
				}
			}
		}

		// Joystick events
		if (joysticks)
		{
			// [joystick][axis]
			// set to '1' so I can debug it
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
							if (axis == 0) {
								newEvent.type = EVENT_TYPE_CONTROLLER_1_JOYSTICK_1_CHANGE_X;
								} else {
								newEvent.type = EVENT_TYPE_CONTROLLER_1_JOYSTICK_1_CHANGE_Y;
							}
						}
						else { // (joy == 1)
							if (axis == 0) {
								newEvent.type = EVENT_TYPE_CONTROLLER_1_JOYSTICK_2_CHANGE_X;
								} else {
								newEvent.type = EVENT_TYPE_CONTROLLER_1_JOYSTICK_2_CHANGE_Y;
							}
						}

						c1State[joy][axis] = c1Now[joy][axis];
						newEvent.data = c1Now[joy][axis];
						addEvent_replace(engine, newEvent);
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
					addEvent_replace(engine, newEvent);
				}
			}
		}

		if (lastUsedEvent(engine) != NULL)
		{
			// This means we can return an event
			consumeEvent(engine, event);
			break;
		}
		else
		{
			// There are no events to return, now decide what to do
			if (engine->eventNone) {
				event->type = EVENT_TYPE_NONE;
				event->data = 0;
				break;
			}
			else {
				continue;
			}
		}
	}
}

#endif // EVENT_H
