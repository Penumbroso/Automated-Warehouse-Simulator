#include "Toolbar.h"
#include "Robot.h"
#include "Globals.h"
#include "Simulator.h"

bool Toolbar::init() 
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// Menu
	auto menuContainer = Sprite::create("MenuContainer.png");
	menuContainer->setPosition(Vec2(visibleSize.width / 2, 25));
	this->addChild(menuContainer);



	playItem->setPosition(Vec2(visibleSize.width / 2, 24));

	auto menu = Menu::create(playItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}
