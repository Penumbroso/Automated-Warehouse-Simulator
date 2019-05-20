#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

using std::vector;

class Robot : public cocos2d::Sprite
{
public:

	virtual bool init();

	// TODO: change to boolean instead of state
	enum State
	{
		FULL,
		EMPTY
	};

	State state = EMPTY;

	vector<Point> grid_path;
	vector<Point> screen_path;
	vector<Point> complete_grid_path;

	Point grid_position;

	Point grid_destination;
	Point screen_destination;
	Point screen_package;
	Point grid_start;
	Point screen_start;
	Point screen_delivery_point;

	void move();
	void stop();
	bool isParked();
	bool isAtDeliverty();
	bool isFull();
	bool isAtPackage();
	bool isInThe(vector<Point> path);

	CREATE_FUNC(Robot);

private:
	Sequence* movement;

	void finishedMovement();
	void updateState();
	void updateGridPosition();

};

#endif