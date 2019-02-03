#include "Toolbar.h"
#include "Globals.h"


bool Toolbar::init() 
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto menuContainer = Sprite::create("MenuContainer.png");
	menuContainer->setPosition(Vec2(visibleSize.width / 2, 25));
	this->addChild(menuContainer);

	runItem = MenuItemImage::create(
		"PlayNormal.png",
		"PlayPressed.png");

	packageItem = MenuItemImage::create(
		"PackageButton.png",
		"PackageButton.png");

	beginItem = MenuItemImage::create(
		"BeginningButton.png",
		"BeginninbButton.png");

	endItem = MenuItemImage::create(
		"EndButton.png",
		"EndButton.png");

	eraseItem = MenuItemImage::create(
		"EraseButton.png",
		"EraseButton.png");

	resetItem = MenuItemImage::create(
		"EraseButton.png",
		"EraseButton.png");

	runItem->setPosition(Vec2(visibleSize.width / 2 - 100, 24));
	packageItem->setPosition(Vec2(visibleSize.width / 2 - 50, 24));
	beginItem->setPosition(Vec2(visibleSize.width / 2 + 0, 24));
	endItem->setPosition(Vec2(visibleSize.width / 2 + 50, 24));
	eraseItem->setPosition(Vec2(visibleSize.width / 2 + 100, 24));
	resetItem->setPosition(Vec2(visibleSize.width / 2 + 150, 24));
	  
	auto menu = Menu::create(runItem, packageItem, beginItem, endItem, eraseItem, resetItem,  NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void Toolbar::setCallback(MenuItemImage* btn, ccMenuCallback callback)
{
	btn->setCallback(callback);
}