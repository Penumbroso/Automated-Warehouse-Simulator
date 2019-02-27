#include "Stopwatch.h"

USING_NS_CC;

bool Stopwatch::init()
{
	Node::init();
	minutes = 0;
	seconds = 0;
	milliseconds = 0;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto text = this->toString();
	label = Label::createWithTTF(text, "fonts/arial.ttf", 20);
	label->setColor(Color3B::GRAY);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 15));

	return true;
}

void Stopwatch::start()
{
	this->schedule(CC_SCHEDULE_SELECTOR(Stopwatch::count), 0.001f);
}

void Stopwatch::stop()
{
	this->unschedule(CC_SCHEDULE_SELECTOR(Stopwatch::count));
}

int Stopwatch::getMinutes()
{
	return minutes;
}

int Stopwatch::getSeconds()
{
	return seconds;
}

int Stopwatch::getMilliseconds()
{
	return milliseconds/10;
}

void Stopwatch::reset()
{
	milliseconds = 0;
	seconds = 0;
	minutes = 0;
}

std::string Stopwatch::toString()
{
	std::string text = std::to_string(minutes) + ":" + std::to_string(seconds) + ":" + std::to_string(milliseconds/10);
	return text;
}

void Stopwatch::count(float dt)
{
	milliseconds++;

	if (milliseconds == 1000)
	{
		milliseconds = 0;
		seconds++;
	}

	if (seconds == 60)
	{
		seconds = 0;
		minutes++;
	}

	label->setString(this->toString());
}

