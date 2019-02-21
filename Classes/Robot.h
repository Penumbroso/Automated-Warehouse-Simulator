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
	
	vector<Point> path;

	// TODO: add a point to this vector every time the robot moves.
	vector<Point> complete_path;

	Point package;

	double speed;

	Point grid_position;
	Point end;

	// TODO: change destination everytime an objective is complete.
	Point destination;

	void move(float dt);
	void run();
	bool isDelivering();

	CREATE_FUNC(Robot);
};

#endif