#include "Infobar.h"

bool Infobar::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto top_bar = DrawNode::create();

	top_bar->drawSolidRect(
		Vec2(0, visibleSize.height),
		Vec2(visibleSize.width, visibleSize.height - 30),
		Color4F(0.3f, 0.3f, 0.3f, 1));

	this->addChild(top_bar);

	return true;
}