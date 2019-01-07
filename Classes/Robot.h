#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "cocos2d.h"

class Robot : public cocos2d::Sprite
{
public:

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Robot);
};

#endif