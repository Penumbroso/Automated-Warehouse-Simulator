#include "Gap.h"
#include "Globals.h"

using namespace cocos2d;

void Gap::onEnter()
{
	Sprite::onEnter();
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Gap::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Gap::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

Rect Gap::getRect()
{
	auto s = getTexture()->getContentSize();
	return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool Gap::containsTouchLocation(Touch* touch)
{
	return getRect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool Gap::onTouchBegan(Touch* touch, Event* event)
{
	//CCLOG("Paddle::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);

	if (!containsTouchLocation(touch)) return false;

	return true;
}

bool Gap::onTouchEnded(Touch* touch, Event* event)
{
	switch (this->state) {
	case EMPTY:
		this->setColor(Color3B::GRAY);
		this->state = FILLED;
		//CCLOG("x: %f y: %f", this->gridLocation.x, this->gridLocation.y);
		g_packages.push_back(this->gridLocation);
		//for (auto i = g_packages.begin(); i != g_packages.end(); ++i) {
		//	CCLOG("x: %f y:%f", i->x, i->y);
		//}
		break;

	case FILLED:
		this->setColor(Color3B::ORANGE);
		this->state = START;
		g_start = this->gridLocation;
		g_startPosition = this->getPosition();
		break;

	case START:
		this->setColor(Color3B::MAGENTA);
		this->state = END;
		g_end = this->gridLocation;
		break;
	case END:
		this->setColor(Color3B::WHITE);
		this->state = EMPTY;

		break;
	}
	
	return true;
}

