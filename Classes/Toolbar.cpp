#include "Toolbar.h"

bool Toolbar::init() 
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto left_bar = DrawNode::create();

	left_bar->drawSolidRect(
		Vec2(0, 0), 
		Vec2(30, visibleSize.height), 
		Color4F(0.3f, 0.3f, 0.3f, 1));

	this->addChild(left_bar);

	// TODO: create a second callback here that changes the icon after pressed the run button to the pause button.

	// TODO: create a button to show the path a robot has taken

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

	// TODO: create a path icon
	pathItem = MenuItemImage::create(
		"BlockadeBtn.png",
		"BlockadeBtn_pressed.png");

	// TODO: create a clock icon
	clockItem = MenuItemImage::create(
		"Timer.png",
		"Timer_pressed.png");

	exportItem = MenuItemImage::create(
		"Timer.png",
		"Timer_pressed.png");


	buttons.push_back(runItem);
	buttons.push_back(packageItem);
	buttons.push_back(beginItem);
	buttons.push_back(endItem);
	buttons.push_back(eraseItem);
	buttons.push_back(resetItem);
	buttons.push_back(blockadeItem);
	//buttons.push_back(pathItem);
	buttons.push_back(clockItem);
	buttons.push_back(exportItem);

	int offset = 30;
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setPosition(Vec2(15, visibleSize.height - 15 * i * 2 - offset));
	}
	  
	auto menu = Menu::create(runItem, packageItem, beginItem, endItem, eraseItem, resetItem, blockadeItem, clockItem, exportItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}