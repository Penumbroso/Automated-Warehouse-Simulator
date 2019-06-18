#include "Sensor.h"

bool Sensor::init()
{
	if (!Sensor::init())
		return false;

	return true;
}

void Sensor::signal()
{
	EventCustom event("change_path");
	event.setUserData(this);
	_eventDispatcher->dispatchEvent(&event);
}
