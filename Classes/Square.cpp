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
	auto s = getTexture()->getContentSize();
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
	switch (g_currentTool)
	{
	case Tool::PACKAGE:
		this->setColor(Color3B::GRAY);
		this->state = FILLED;
		g_packages.push_back(this->gridLocation);
		break;
	case Tool::BEGIN:
		this->state = START;
		g_start = this->gridLocation;
		this->setColor(Color3B::BLUE);
		break;
	case Tool::END:
		this->setColor(Color3B::MAGENTA);
		this->state = END;
		g_end = this->gridLocation;
		break;
	}
	
	return true;
}

