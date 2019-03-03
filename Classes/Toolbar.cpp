#include "Toolbar.h"

bool Toolbar::init() 
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto top_bar = DrawNode::create();

	top_bar->drawSolidRect(
		Vec2(0, visibleSize.height),
		Vec2(visibleSize.width, visibleSize.height - 30),
		Color4F(0.3f, 0.3f, 0.3f, 1));

	this->addChild(top_bar);

	auto left_bar = DrawNode::create();

	left_bar->drawSolidRect(
		Vec2(0, 0), 
		Vec2(30, visibleSize.height), 
		Color4F(0.3f, 0.3f, 0.3f, 1));

	this->addChild(left_bar);

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

	blockadeItem = MenuItemImage::create(
		"BlockadeBtn.png",
		"BlockadeBtn_pressed.png");

	runItem->setPosition(Vec2(15, visibleSize.height - 15 * 2));
	packageItem->setPosition(Vec2(15, visibleSize.height - 15 * 4));
	beginItem->setPosition(Vec2(15, visibleSize.height - 15 * 6));
	endItem->setPosition(Vec2(15, visibleSize.height - 15 * 8));
	eraseItem->setPosition(Vec2(15, visibleSize.height - 15 * 10));
	resetItem->setPosition(Vec2(15, visibleSize.height - 15 * 12));
	blockadeItem->setPosition(Vec2(15, visibleSize.height - 15 * 14));
	  
	auto menu = Menu::create(runItem, packageItem, beginItem, endItem, eraseItem, resetItem, blockadeItem,  NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}