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

	Point grid_position;

	CREATE_FUNC(Robot);
};

#endif