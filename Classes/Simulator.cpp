#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "Robot.h"
#include "Globals.h"
#include "Astar.h"

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

	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Simulator::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Simulator::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	auto pTextField = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
		"fonts/arial.ttf",
		24);
	this->addChild(pTextField, 1);

	pTextField->setPosition(300, 300);

	_trackNode = pTextField;



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

    return true;
}

void Simulator::onEnter()
{
	Scene::onEnter();
	//this->schedule(CC_SCHEDULE_SELECTOR(Simulator::tick), 1.0f);
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

	Astar::Astar(g_start, g_end, g_packages);


	// Say to the robot to go get something
	// Say to the robot to deliver the package
}

void Simulator::tick(float dt) {
	if (this->state == EDITING)
		CCLOG("Editing");

}

void Simulator::onClickTrackNode(bool bClicked, const Vec2& touchPos)
{
	auto pTextField = (TextFieldTTF*)_trackNode;
	if (bClicked)
	{
		// TextFieldTTFTest be clicked
		CCLOG("TextFieldTTFDefaultTest:TextFieldTTF attachWithIME");
		pTextField->attachWithIME();
	}
	else
	{
		// TextFieldTTFTest not be clicked
		CCLOG("TextFieldTTFDefaultTest:TextFieldTTF detachWithIME");
		pTextField->detachWithIME();
	}
}

bool Simulator::onTouchBegan(Touch  *touch, Event  *event)
{
	CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
	//_beginPos = touch->getLocation();
	return true;
}

void Simulator::onTouchEnded(Touch  *touch, Event  *event)
{

	auto endPos = touch->getLocation();


	// decide the trackNode is clicked.
	Rect rect;
	rect.size = _trackNode->getContentSize();
	auto clicked = isScreenPointInRect(endPos, Camera::getVisitingCamera(), _trackNode->getWorldToNodeTransform(), rect, nullptr);
	this->onClickTrackNode(clicked, endPos);
	CCLOG("----------------------------------");
}