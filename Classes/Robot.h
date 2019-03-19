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
	
	vector<Point> path;
	vector<Point> complete_path;

	Point grid_coord;
	Point destination;
	Point package;
	Point start;
	Point delivery_point;

	Stopwatch* stopwatch;

	void move(float dt);
	void updateState();
	bool isParked();
	bool isAtDeliverty();
	bool isFull();
	bool isAtPackage();
	bool isInThe(vector<Point> path);

	CREATE_FUNC(Robot);
};

#endif