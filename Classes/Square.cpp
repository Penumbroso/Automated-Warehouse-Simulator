#include "Square.h"
#include "Globals.h"
#include <algorithm>

using namespace cocos2d;

void Square::onEnter()
{
	Sprite::onEnter();
	// Register Touch Event
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
	switch (this->state) {
	case EMPTY:
		this->setColor(Color3B::GRAY);
		this->state = FILLED;
		g_packages.push_back(this->gridLocation);
		break;

	case FILLED:
		if (g_start.x == -1)
		{
			this->state = START;
			g_start = this->gridLocation;
			this->setColor(Color3B::ORANGE);
			g_packages.pop_back();
		}
		else if (g_end.x == -1) {
			this->setColor(Color3B::MAGENTA);
			this->state = END;
			g_end = this->gridLocation;
			g_packages.pop_back();
		}
		else
		{
			this->setColor(Color3B::WHITE);
			this->state = EMPTY;
			std::remove(g_packages.begin(), g_packages.end(), this->gridLocation);
		}
		break;

	case START:
		this->setColor(Color3B::WHITE);
		this->state = EMPTY;
		g_start = Point(-1, -1);
		break;
	case END:
		this->setColor(Color3B::WHITE);
		this->state = EMPTY;
		g_end = Point(-1, -1);
		break;
	}
	
	return true;
}

