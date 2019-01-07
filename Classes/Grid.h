#ifndef __GRID_H__
#define __GRID_H__

#include "cocos2d.h"

class Grid : public cocos2d::Layer
{
public:
	//static cocos2d::Scene* createLayer();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Grid);
};

#endif