#include "Square.h"
#include "Globals.h"
#include <algorithm>

using namespace cocos2d;

void Square::onEnter()
{
	Sprite::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Square::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Square::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

Rect Square::getRect()
{
	auto s = this->getContentSize();
	return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool Square::containsTouchLocation(Touch* touch)
{
	return getRect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool Square::onTouchBegan(Touch* touch, Event* event)
{
	if (!containsTouchLocation(touch)) return false;
	return true;
}

bool Square::onTouchEnded(Touch* touch, Event* event)
{
	switch (g_current_tool)
	{
	case Tool::PACKAGE:
		this->setColor(Color3B::GRAY);
		this->state = FILLED;
		g_packages.push_back(this->gridLocation);
		break;
	case Tool::BEGIN:
		this->state = START;
		g_start.push_back(this->gridLocation);
		this->setColor(Color3B::BLUE);
		break;
	case Tool::END:
		this->setColor(Color3B::MAGENTA);
		this->state = END;
		g_end.push_back(this->gridLocation);
		break;
	case Tool::ERASE:
		std::vector<Point>* vector;
		switch(this->state)
		{
		case FILLED:
			vector = &g_packages;
			break;
		case START:
			vector = &g_start;
			break;
		case END:
			vector = &g_end;
			break;
		}
		auto it = std::find(vector->begin(), vector->end(), this->gridLocation);
		if (it != vector->end()) vector->erase(it);
		this->state = EMPTY;
		this->setColor(Color3B::WHITE);
	}
	
	return true;
}

