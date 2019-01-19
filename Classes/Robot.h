#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "cocos2d.h"
#include <vector>

class Robot : public cocos2d::Sprite
{
public:

	virtual bool init();
	
	std::vector<cocos2d::Point> path;

	CREATE_FUNC(Robot);
};

#endif