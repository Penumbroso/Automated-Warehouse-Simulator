#include "Robot.h"
#include "Util.h"

USING_NS_CC;

bool Robot::init()
{
	if (!Sprite::init())
		return false;
	
	stopwatch = Stopwatch::create();
	this->addChild(stopwatch);
	return true;
}

void Robot::move(float dt)
{
	if (!this->path.empty())
	{
		auto next_position = this->path.back();
		this->complete_path.push_back(next_position);
		this->path.pop_back();
		this->grid_coord = next_position;
	}
}

void Robot::updateState()
{
	if (grid_coord == this->package)
		this->state = FULL;
	
	if (grid_coord == this->delivery_point)
		this->state = EMPTY;
}

bool Robot::isParked()
{
	return grid_coord == start;
}

bool Robot::isAtDeliverty()
{
	return grid_coord == delivery_point;
}

bool Robot::isFull()
{
	return state == FULL;
}

bool Robot::isAtPackage()
{	
	return grid_coord == package;
}

bool Robot::isInThe(vector<Point> path)
{
	return Util::contains<Point>(&path, this->grid_coord);
}

