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

	Point grid_position;
	Point destination;
	Point package;

	void move(float dt);
	void run();
	void updateState();

	CREATE_FUNC(Robot);
};

#endif