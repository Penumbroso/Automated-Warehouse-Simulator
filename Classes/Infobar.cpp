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

	label = Label::createWithTTF("0:0:0", "fonts/arial.ttf", 20);
	label->setColor(Color3B::GRAY);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 15));
	this->addChild(label);

	this->schedule(CC_SCHEDULE_SELECTOR(Infobar::updateClock), 0.001f);

	return true;
}

void Infobar::updateClock(float dt)
{
	
	if (time) 
	{
		label->setString(*time);
	}

}
