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

	enum State
	{
		FULL,
		EMPTY
	};

	State state = EMPTY;

	Sequence* movement;
	
	vector<Point> grid_path;
	vector<Point> screen_path;
	vector<Point> complete_path;

	Point grid_coord;

	Point grid_destination;
	Point screen_destination;
	Point screen_package;
	Point grid_start;
	Point screen_start;
	Point screen_delivery_point;

	void move();
	void stop();
	void updateState();
	bool isParked();
	bool isAtDeliverty();
	bool isFull();
	bool isAtPackage();
	bool isInThe(vector<Point> path);
	void updateGridPosition();
	void finishedMovement();

	CREATE_FUNC(Robot);
};

#endif