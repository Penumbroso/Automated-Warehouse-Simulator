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

	Point grid_coord;
	Point destination;
	Point package;
	Point end;

	void move(float dt);
	void updateState();

	CREATE_FUNC(Robot);
};

#endif