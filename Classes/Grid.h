#ifndef __GRID_H__
#define __GRID_H__

#include "cocos2d.h"
#include "Square.h"

USING_NS_CC;

class Grid : public cocos2d::Layer
{
public:
	virtual bool init();

	std::map<std::pair<int, int>, Square*> squares;

	Menu * menu;

	void setCallback(ccMenuCallback callback);
	
	CREATE_FUNC(Grid);

};

#endif