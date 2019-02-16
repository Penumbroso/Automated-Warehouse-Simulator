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
	
	vector<Point> path;
	Point package;

	double speed;

	Point grid_position;
	Point end;

	void move(float dt);
	void run();

	CREATE_FUNC(Robot);
};

#endif