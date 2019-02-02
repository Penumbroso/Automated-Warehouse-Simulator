#include "Toolbar.h"
#include "Globals.h"


bool Toolbar::init() 
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto menuContainer = Sprite::create("MenuContainer.png");
	menuContainer->setPosition(Vec2(visibleSize.width / 2, 25));
	this->addChild(menuContainer);

	playItem = MenuItemImage::create(
		"PlayNormal.png",
		"PlayPressed.png",
		CC_CALLBACK_1(Toolbar::menuRunCallback, this));

	playItem->setPosition(Vec2(visibleSize.width / 2 - 100, 24));

	auto packageItem = MenuItemImage::create(
		"PackageButton.png",
		"PackageButton.png",
		CC_CALLBACK_1(Toolbar::menuPackageCallback, this));

	packageItem->setPosition(Vec2(visibleSize.width / 2 - 50, 24));

	auto beginningItem = MenuItemImage::create(
		"BeginningButton.png",
		"BeginninbButton.png",
		CC_CALLBACK_1(Toolbar::menuBeginningCallback, this));

	beginningItem->setPosition(Vec2(visibleSize.width / 2 + 0, 24));

	auto endItem = MenuItemImage::create(
		"EndButton.png",
		"EndButton.png",
		CC_CALLBACK_1(Toolbar::menuEndCallback, this));

	endItem->setPosition(Vec2(visibleSize.width / 2 + 50, 24));

	auto eraseItem = MenuItemImage::create(
		"EraseButton.png",
		"EraseButton.png",
		CC_CALLBACK_1(Toolbar::menuEraseCallback, this));

	eraseItem->setPosition(Vec2(visibleSize.width / 2 + 100, 24));

	// TODO: create reset button.
	// Have to save all the globals before the simulation begin and then load the save.
	  
	auto menu = Menu::create(playItem, packageItem, beginningItem, endItem, eraseItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void Toolbar::menuPackageCallback(cocos2d::Ref * pSender)
{
	g_current_tool = PACKAGE;
}

void Toolbar::menuBeginningCallback(cocos2d::Ref * pSender)
{
	g_current_tool = BEGIN;
}

void Toolbar::menuEndCallback(cocos2d::Ref * pSender)
{
	g_current_tool = END;
}

void Toolbar::menuEraseCallback(cocos2d::Ref * pSender)
{
	g_current_tool = ERASE;
}

void Toolbar::menuRunCallback(cocos2d::Ref * pSender)
{
	g_running = !g_running;
	// TODO: change from pause to play and vice versa.
	playItem->setNormalImage(Sprite::create("EndButton.png"));
}
