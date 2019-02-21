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
	if (!this->path.empty())
	{
		auto next_position = this->path.back();
		this->path.pop_back();
		this->grid_position = next_position;
	}
}

void Robot::run()
{
	this->schedule(CC_SCHEDULE_SELECTOR(Robot::move), 0.2f);
}

bool Robot::isDelivering()
{
	if (this->package != NULL) return true;
	return false;
}
