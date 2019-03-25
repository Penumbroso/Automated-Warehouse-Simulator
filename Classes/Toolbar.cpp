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

	selected_bg = DrawNode::create();
	selected_bg->drawSolidRect(
		Vec2(0, 0),
		Vec2(30, 30),
		Color4F(0.25f, 0.25f, 0.25f, 1));
	selected_bg->setContentSize(Size(30, 30));
	selected_bg->setVisible(false);
	selected_bg->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(selected_bg);

	// TODO: create a second callback here that changes the icon after pressed the run button to the pause button.

	// TODO: create a button to show the path a robot has taken

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

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);

	buttons.push_back(packageItem);
	buttons.push_back(beginItem);
	buttons.push_back(endItem);
	buttons.push_back(eraseItem);
	buttons.push_back(blockadeItem);
	buttons.push_back(clockItem);
	buttons.push_back(pathItem);

	int offset = 200;
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setPosition(Vec2(15, visibleSize.height - 15 * i * 2 - offset));
		menu->addChild(buttons[i]);
	}
	  
	this->addChild(menu, 1);

	return true;
}

void Toolbar::selectTool(Tool tool)
{
	selected_bg->setVisible(true);
	this->selected = tool;
	switch (tool)
	{
	case Toolbar::PACKAGE:
		selected_bg->setPosition(packageItem->getPosition());
		break;
	case Toolbar::BEGIN:
		selected_bg->setPosition(beginItem->getPosition());
		break;
	case Toolbar::END:
		selected_bg->setPosition(endItem->getPosition());
		break;
	case Toolbar::ERASE:
		selected_bg->setPosition(eraseItem->getPosition());
		break;
	case Toolbar::BLOCKADE:
		selected_bg->setPosition(blockadeItem->getPosition());
		break;
	case Toolbar::CLOCK:
		selected_bg->setPosition(clockItem->getPosition());
		break;
	case Toolbar::PATH:
		selected_bg->setPosition(pathItem->getPosition());
		break;
	default:
		break;
	}
}
