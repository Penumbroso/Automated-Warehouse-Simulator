#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "Robot.h"
#include "Globals.h"

USING_NS_CC;

Scene* Simulator::createScene()
{
    return Simulator::create();
}

// on "init" you need to initialize your instance
bool Simulator::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->schedule(CC_SCHEDULE_SELECTOR(Simulator::something), 3);
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Simulator::menuCloseCallback, this));


    float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
    float y = origin.y + closeItem->getContentSize().height/2;
    closeItem->setPosition(Vec2(x,y));

	auto playItem = MenuItemImage::create(
		"PlayNormal.png",
		"PlayNormal.png",
		CC_CALLBACK_1(Simulator::menuPlayCallback, this));

	// Must correct the image size
	playItem->setScale(0.1);

	playItem->setPosition(Vec2(x - 40, y));
    

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, playItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	auto background = LayerColor::create(Color4B::RED);
	this->addChild(background);

	grid = Grid::create();
	this->addChild(grid);
	
    return true;
}


void Simulator::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void Simulator::menuPlayCallback(Ref* pSender)
{
	CCLOG("Play button");

	// Create a robot
	auto robot = Robot::create();
	robot->initWithFile("Robot.png");
	robot->setPosition(g_startPosition);
	//robot->setPositionInGrid(g_start);
	robot->setColor(Color3B::BLUE);
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
