#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "Globals.h"
#include "AStar.hpp"
#include <algorithm>

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
	//this->addChild(pTextField, 1);

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

	this->schedule(CC_SCHEDULE_SELECTOR(Simulator::tick), 1.0f);
    return true;
}

void Simulator::menuPlayCallback(Ref* pSender)
{
	auto robot = Robot::create();
	robot->initWithFile("Robot.png");
	robot->setPosition(25 + (g_start.x ) * 50, 25 + (g_start.y ) * 50);
	robot->setColor(Color3B(200, 100, 100));
	this->addChild(robot);

	// number of squares
	auto visibleSize = Director::getInstance()->getVisibleSize();

	int numberOfLines = visibleSize.height / 50;
	int numberOfColumns = visibleSize.width / 50;

	AStar::Generator generator;
	generator.setWorldSize({ numberOfColumns, numberOfLines });
	generator.setHeuristic(AStar::Heuristic::euclidean);
	generator.setDiagonalMovement(false);

	auto path = generator.findPath({ (int)g_start.x, (int)g_start.y }, { (int)g_end.x, (int)g_end.y });
	std::vector<Point> pathing;

	for (auto& coordinate : path) {
		auto point = Point(coordinate.x, coordinate.y);
		pathing.push_back(point);
	}

	robot->path = pathing;


	this->robots.push_back(robot);
	this->state = RUNNING;
}

void Simulator::tick(float dt) {
	if (this->state == EDITING)
		CCLOG("Editing");

	if (this->state == RUNNING) {
		for (Robot* robot : this->robots) {
			if (!robot->path.empty())
			{	
				Point position = robot->path.back();
				position = Point(25 + position.x * 50, 25 + position.y * 50);
				robot->setPosition(position);
				robot->path.pop_back();
			}
		}
	}

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