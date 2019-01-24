#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "cocos2d.h"
#include <vector>
#include "AStar.hpp"

class Robot : public cocos2d::Sprite
{
public:

	virtual bool init();
	
	std::vector<AStar::Vec2i> path;

	cocos2d::Point grid_position;

	CREATE_FUNC(Robot);
};

#endif