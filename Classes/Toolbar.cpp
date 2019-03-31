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
		Color4F(0.20f, 0.20f, 0.20f, 1));
	selected_bg->setContentSize(Size(30, 30));
	selected_bg->setVisible(false);
	selected_bg->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(selected_bg);

	createButtons();

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);

	buttons.push_back(packageItem);
	buttons.push_back(beginItem);
	buttons.push_back(endItem);
	buttons.push_back(eraseItem);
	buttons.push_back(blockadeItem);
	buttons.push_back(clockItem);
	buttons.push_back(pathItem);

	int offset = 300;
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setPosition(Vec2(15, visibleSize.height - 15 * i * 2 - offset));
		menu->addChild(buttons[i]);
	}
	  
	this->addChild(menu, 1);

	this->setCallbacks();

	return true;
}

void Toolbar::setTool(Tool tool, MenuItemImage * btn)
{
	selected_bg->setVisible(true);
	this->selected = tool;
	selected_bg->setPosition(btn->getPosition());
}

void Toolbar::setCallbacks()
{
	this->packageItem->setCallback(CC_CALLBACK_0(Toolbar::setTool, this, Toolbar::PACKAGE, packageItem));
	this->beginItem->setCallback(CC_CALLBACK_0(Toolbar::setTool, this, Toolbar::BEGIN, beginItem));
	this->endItem->setCallback(CC_CALLBACK_0(Toolbar::setTool, this, Toolbar::END, endItem));
	this->eraseItem->setCallback(CC_CALLBACK_0(Toolbar::setTool, this, Toolbar::ERASE, eraseItem));
	this->blockadeItem->setCallback(CC_CALLBACK_0(Toolbar::setTool, this, Toolbar::BLOCKADE, blockadeItem));
	this->clockItem->setCallback(CC_CALLBACK_0(Toolbar::setTool, this, Toolbar::CLOCK, clockItem));
	this->pathItem->setCallback(CC_CALLBACK_0(Toolbar::setTool, this, Toolbar::PATH, pathItem));
}

void Toolbar::createButtons()
{
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
}
