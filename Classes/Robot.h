#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "cocos2d.h"
#include <vector>
#include "Stopwatch.h"

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
	
	vector<Point> grid_path;
	vector<Point> screen_path;
	vector<Point> complete_path;

	Point grid_coord;

	Point grid_destination;
	Point screen_destination;

	Point grid_package;
	Point screen_package;

	Point grid_start;
	Point screen_start;

	Point grid_delivery_point;
	Point screen_delivery_point;

	Stopwatch* stopwatch;

	void move(float dt);
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