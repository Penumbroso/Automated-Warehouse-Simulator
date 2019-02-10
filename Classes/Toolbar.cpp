#include "Toolbar.h"
#include "Globals.h"


bool Toolbar::init() 
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto rectangle = DrawNode::create();
	rectangle->drawSolidRect(Vec2(0, 0), Vec2(50, visibleSize.height), Color4F(0.3f, 0.3f, 0.3f, 1));
	this->addChild(rectangle);

	// TODO: create a second callback here that changes the icon when pressed.
	runItem = MenuItemImage::create(
		"PlayNormal.png",
		"PlayPressed.png");

	// TODO: create a package icon
	packageItem = MenuItemImage::create(
		"PackageButton.png",
		"PackageButton.png");

	beginItem = MenuItemImage::create(
		"Plus.png",
		"Plus.png");

	endItem = MenuItemImage::create(
		"Minus.png",
		"Minus.png");

	// TODO: create a erase icon
	eraseItem = MenuItemImage::create(
		"EraseButton.png",
		"EraseButton.png");

	// TODO: create a reset icon
	resetItem = MenuItemImage::create(
		"EraseButton.png",
		"EraseButton.png");

	// TODO: add a purely collidable object so that the user can make walls

	runItem->setPosition(Vec2(25, visibleSize.height - 25 * 2));
	packageItem->setPosition(Vec2(25, visibleSize.height - 25 * 4));
	beginItem->setPosition(Vec2(25, visibleSize.height - 25 * 6));
	endItem->setPosition(Vec2(25, visibleSize.height - 25 * 8));
	eraseItem->setPosition(Vec2(25, visibleSize.height - 25 * 10));
	resetItem->setPosition(Vec2(25, visibleSize.height - 25 * 12));
	  
	auto menu = Menu::create(runItem, packageItem, beginItem, endItem, eraseItem, resetItem,  NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}