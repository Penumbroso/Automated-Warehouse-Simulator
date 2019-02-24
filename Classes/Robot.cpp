#include "Robot.h"

USING_NS_CC;

bool Robot::init()
{
	if (!Sprite::init())
		return false;
	
	return true;
}

void Robot::move(float dt)
{

	EventCustom event("prevent_collision");
	event.setUserData(this);
	_eventDispatcher->dispatchEvent(&event);

	if (!this->path.empty())
	{
		auto next_position = this->path.back();
		this->path.pop_back();
		this->grid_position = next_position;
	}

	this->updateState();
}

void Robot::updateState()
{
	if (grid_position == destination)
	{
		state = (state == FULL) ? EMPTY : FULL;
	}
}

void Robot::run()
{
	this->schedule(CC_SCHEDULE_SELECTOR(Robot::move), 0.5f);
}

