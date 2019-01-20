#ifndef __GRID_H__
#define __GRID_H__

#include "cocos2d.h"
#include "Square.h"

USING_NS_CC;

class Grid : public cocos2d::Layer
{
public:
	virtual bool init();

	Point getPositionOf(Point gridLocation);
	
	CREATE_FUNC(Grid);

private:
	Square* squares[100][100];
};

#endif