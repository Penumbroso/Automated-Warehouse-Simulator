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
	//Vector<FiniteTimeAction*> movements;

	//auto origin = this->getPosition();

	//for (auto destination : path)
	//{
	//	//get the distance between the destination position and the node's position
	//	double distance = sqrt(pow((destination.x - origin.x), 2.0) + pow((destination.y - origin.y), 2.0));

	//	//calculate your new duration based on the distance
	//	float moveDuration = 0.001*distance;

	//	auto moveToNextSquare = MoveTo::create(moveDuration, destination);
	//	movements.pushBack(moveToNextSquare);

	//	origin = destination;
	//}

	//this->runAction(Sequence::create(movements));

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

