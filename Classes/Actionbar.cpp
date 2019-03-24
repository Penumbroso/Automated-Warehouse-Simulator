#include "Actionbar.h"

bool Actionbar::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// TODO: create a second callback here that changes the icon after pressed the run button to the pause button.

	runItem = MenuItemImage::create(
		"RunBtn.png",
		"RunBtn_pressed.png");

	resetItem = MenuItemImage::create(
		"resetBtn.png",
		"resetBtn_pressed.png");

	exportItem = MenuItemImage::create(
		"Timer.png",
		"Timer_pressed.png");

	speedUpItem = MenuItemImage::create(
		"SpeedUpBtn.png",
		"SpeedUpBtn.png");

	slowDownItem = MenuItemImage::create(
		"SlowDownBtn.png",
		"SlowDownBtn.png");


	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);


	buttons.push_back(runItem);
	buttons.push_back(resetItem);
	buttons.push_back(exportItem);
	buttons.push_back(speedUpItem);
	buttons.push_back(slowDownItem);

	int offset = 30;
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setPosition(Vec2(15, visibleSize.height - 15 * i * 2 - offset));
		menu->addChild(buttons[i]);
	}

	this->addChild(menu, 1);

	return true;
}