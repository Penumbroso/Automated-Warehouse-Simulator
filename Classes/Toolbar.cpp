#include "Toolbar.h"
#include "Globals.h"


bool Toolbar::init() 
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto rectangle = DrawNode::create();

	rectangle->drawSolidRect(
		Vec2(0, 0), 
		Vec2(30, visibleSize.height), 
		Color4F(0.3f, 0.3f, 0.3f, 1));

	this->addChild(rectangle);

	// TODO: create a second callback here that changes the icon after pressed the run button to the pause button.

	runItem = MenuItemImage::create(
		"RunBtn.png",
		"RunBtn_pressed.png");

	packageItem = MenuItemImage::create(
		"PackageBtn.png",
		"PackageBtn_pressed.png");

	beginItem = MenuItemImage::create(
		"PlusBtn.png",
		"PlusBtn_pressed.png");

	endItem = MenuItemImage::create(
		"MinusBtn.png",
		"MinusBtn_pressed.png");

	eraseItem = MenuItemImage::create(
		"EraseBtn.png",
		"EraseBtn_pressed.png");

	resetItem = MenuItemImage::create(
		"resetBtn.png",
		"resetBtn_pressed.png");

	// TODO: add a purely collidable object so that the user can make walls

	runItem->setPosition(Vec2(15, visibleSize.height - 15 * 2));
	packageItem->setPosition(Vec2(15, visibleSize.height - 15 * 4));
	beginItem->setPosition(Vec2(15, visibleSize.height - 15 * 6));
	endItem->setPosition(Vec2(15, visibleSize.height - 15 * 8));
	eraseItem->setPosition(Vec2(15, visibleSize.height - 15 * 10));
	resetItem->setPosition(Vec2(15, visibleSize.height - 15 * 12));
	  
	auto menu = Menu::create(runItem, packageItem, beginItem, endItem, eraseItem, resetItem,  NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}