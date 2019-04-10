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
	Vector<FiniteTimeAction*> movements;

	auto origin = this->getPosition();
	reverse(screen_path.begin(), screen_path.end());
	for (auto destination : screen_path)
	{
		//get the distance between the destination position and the node's position
		double distance = sqrt(pow((destination.x - origin.x), 2.0) + pow((destination.y - origin.y), 2.0));

		//calculate your new duration based on the distance
		float moveDuration = 0.01*distance;

		auto moveToNextSquare = MoveTo::create(moveDuration, destination);
		movements.pushBack(moveToNextSquare);

		origin = destination;
	}

	auto callbackJump = CallFunc::create(CC_CALLBACK_0(Robot::test, this));

	auto sequence_of_movements = Sequence::create(movements);
	auto seq = Sequence::create(sequence_of_movements, callbackJump, nullptr);
	this->runAction(seq);
}

void Robot::updateState()
{
	if (this->getPosition() == this->screen_package)
		this->state = FULL;
	
	if (this->getPosition() == this->screen_delivery_point)
		this->state = EMPTY;
}

bool Robot::isParked()
{
	return grid_coord == grid_start;
}

bool Robot::isAtDeliverty()
{
	return grid_coord == grid_delivery_point;
}

bool Robot::isFull()
{
	return state == FULL;
}

bool Robot::isAtPackage()
{	
	return grid_coord == grid_package;
}

bool Robot::isInThe(vector<Point> path)
{
	return Util::contains<Point>(&path, this->grid_coord);
}

void Robot::test()
{
	log("Finished Action!");
	this->grid_path.clear();
	this->screen_path.clear();
	updateState();
}

