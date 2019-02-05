#ifndef __GRID_H__
#define __GRID_H__

#include "cocos2d.h"
#include "Square.h"

USING_NS_CC;

class Grid : public cocos2d::Layer
{
public:
	virtual bool init();

	// TODO: use point instead of pair
	std::map<Point, Square*> squares;

	Menu * menu;

	float number_of_columns;
	float number_of_lines;
	
	CREATE_FUNC(Grid);

};

#endif