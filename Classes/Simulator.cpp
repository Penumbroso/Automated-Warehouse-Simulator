#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "Globals.h"
#include "AStar.hpp"
#include <algorithm>
#include "Toolbar.h"

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

	//// Register Touch Event
	//auto listener = EventListenerTouchOneByOne::create();
	//listener->onTouchBegan = CC_CALLBACK_2(Simulator::onTouchBegan, this);
	//listener->onTouchEnded = CC_CALLBACK_2(Simulator::onTouchEnded, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//auto pTextField = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
	//	"fonts/arial.ttf",
	//	24);
	////this->addChild(pTextField, 1);

	//pTextField->setPosition(300, 300);

	//_trackNode = pTextField;

    auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = LayerColor::create(Color4B::RED);
	this->addChild(background);

	grid = Grid::create();
	this->addChild(grid);

	auto toolbar = Toolbar::create(CC_CALLBACK_1(Simulator::menuPlayCallback, this));
	this->addChild(toolbar);

	this->schedule(CC_SCHEDULE_SELECTOR(Simulator::tick), 0.5f);
    return true;
}

void Simulator::menuPlayCallback(Ref* pSender)
{
	auto robot = Robot::create();
	robot->initWithFile("Robot.png");
	robot->setPosition(25 + (g_start.x ) * 50, 25 + (g_start.y ) * 50);
	robot->setColor(Color3B(200, 100, 100));
	grid->addChild(robot);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	int numberOfLines = visibleSize.height / 50;
	int numberOfColumns = visibleSize.width / 50;
	
	AStar::Generator generator;
	generator.setWorldSize({ numberOfColumns, numberOfLines });
	generator.setHeuristic(AStar::Heuristic::manhattan);
	generator.setDiagonalMovement(false);

	for (Point point : g_packages)
	{
		AStar::Vec2i vec2i;
		vec2i.x = point.x;
		vec2i.y = point.y;
		generator.addCollision(vec2i);
	}

	auto path = generator.findPath({ (int)g_start.x, (int)g_start.y }, { (int)g_end.x, (int)g_end.y });

	robot->path = path;

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
				auto pos = robot->path.back();
				auto position = Point(25 + pos.x * 50, 25 + pos.y * 50);
				robot->setPosition(position);
				robot->path.pop_back();
			}
		}
	}

}

//void Simulator::onClickTrackNode(bool bClicked, const Vec2& touchPos)
//{
//	auto pTextField = (TextFieldTTF*)_trackNode;
//	if (bClicked)
//	{
//		// TextFieldTTFTest be clicked
//		CCLOG("TextFieldTTFDefaultTest:TextFieldTTF attachWithIME");
//		pTextField->attachWithIME();
//	}
//	else
//	{
//		// TextFieldTTFTest not be clicked
//		CCLOG("TextFieldTTFDefaultTest:TextFieldTTF detachWithIME");
//		pTextField->detachWithIME();
//	}
//}
//
//bool Simulator::onTouchBegan(Touch  *touch, Event  *event)
//{
//	CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
//	//_beginPos = touch->getLocation();
//	return true;
//}
//
//void Simulator::onTouchEnded(Touch  *touch, Event  *event)
//{
//
//	//auto endPos = touch->getLocation();
//
//
//	//// decide the trackNode is clicked.
//	//Rect rect;
//	//rect.size = _trackNode->getContentSize();
//	//auto clicked = isScreenPointInRect(endPos, Camera::getVisitingCamera(), _trackNode->getWorldToNodeTransform(), rect, nullptr);
//	//this->onClickTrackNode(clicked, endPos);
//	//CCLOG("----------------------------------");
//}