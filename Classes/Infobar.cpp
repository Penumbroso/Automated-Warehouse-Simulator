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

	time_label = Label::createWithTTF("0:0:0", "fonts/arial.ttf", 20);
	time_label->setColor(Color3B::GRAY);
	time_label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 15));
	this->addChild(time_label);

	speed_factor_label = Label::createWithTTF("Speed: x1", "fonts/arial.ttf", 20);
	speed_factor_label->setColor(Color3B::GRAY);
	speed_factor_label->setPosition(Vec2(visibleSize.width / 2 - 200, visibleSize.height - 15));
	this->addChild(speed_factor_label);

	this->schedule(CC_SCHEDULE_SELECTOR(Infobar::updateClock), 0.001f);

	return true;
}

void Infobar::updateSpeed(float speed)
{
	std::string speed_mult = std::to_string(speed);
	speed_factor_label->setString("Speed: x" + speed_mult.substr(0, 5));
}

void Infobar::updateClock(float dt)
{
	if (time) 
	{
		time_label->setString(*time);
	}

}
