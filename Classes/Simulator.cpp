#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "Robot.h"
#include "Globals.h"

USING_NS_CC;

Scene* Simulator::createScene()
{
    return Simulator::create();
}

bool Simulator::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = LayerColor::create(Color4B::RED);
	this->addChild(background);

	grid = Grid::create();
	this->addChild(grid);
	
	// Menu
	auto menuContainer = Sprite::create("MenuContainer.png");
	menuContainer->setPosition(Vec2(visibleSize.width / 2, 25));
	this->addChild(menuContainer);

	// Button
	auto playItem = MenuItemImage::create(
											"PlayNormal.png",
											"PlayPressed.png",
											CC_CALLBACK_1(Simulator::menuPlayCallback, this));

	playItem->setPosition(Vec2(visibleSize.width / 2, 24));
    
    auto menu = Menu::create(playItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	// Something in this line of code is making autocomplete not working
	//this->schedule(CC_SCHEDULE_SELECTOR(Simulator::something), 3);

    return true;
}

void Simulator::menuPlayCallback(Ref* pSender)
{
	CCLOG("Play button");

	// Create a robot
	auto robot = Robot::create();
	robot->initWithFile("Robot.png");
	robot->setPosition(g_startPosition);
	//robot->setPositionInGrid(g_start);
	robot->setColor(Color3B(200, 100, 100));
	this->addChild(robot);

	auto position = grid->getPositionOf(g_start);
	CCLOG("%f %f", position.x, position.y);

	// Say to the robot to go get something
	// Say to the robot to deliver the package
}

void Simulator::something(float dt) {
	if (this->state == EDITING)
		CCLOG("Editing");

}
