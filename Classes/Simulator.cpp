#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "AStar.hpp"
#include <algorithm>

USING_NS_CC;

bool Simulator::init()
{    
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	isRunning = false;
	speed_factor = 1.0f;

	grid = Grid::create();
	stopwatch = Stopwatch::create();
	infobar = Infobar::create();
	toolbar = Toolbar::create();
	actionbar = Actionbar::create();
	robotController = RobotController::create();

	grid->setPosition(30, 0);
	infobar->time = &stopwatch->text;
	robotController->grid = grid;

	addChild(grid);
	addChild(stopwatch);
	addChild(infobar);
	addChild(toolbar);
	addChild(actionbar);
	addChild(robotController);

	setCallbacks();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Simulator::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto robotAtDeliveryListener = EventListenerCustom::create("robot_at_delivery", CC_CALLBACK_1(Simulator::robotIsAtDelivery, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(robotAtDeliveryListener, this);

	auto robotAtPackageListener = EventListenerCustom::create("robot_at_package", CC_CALLBACK_1(Simulator::robotIsAtPackage, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(robotAtPackageListener, this);

    return true;
}

void Simulator::setCallbacks()
{
	actionbar->runItem->setCallback(CC_CALLBACK_1(Simulator::menuRunCallback, this));
	actionbar->exportItem->setCallback(CC_CALLBACK_1(Simulator::menuExportCallback, this));
	actionbar->resetItem->setCallback(CC_CALLBACK_1(Simulator::menuResetCallback, this));
	actionbar->speedUpItem->setCallback(CC_CALLBACK_0(Simulator::menuChangeSpeedCallback, this, 1/2.0));
	actionbar->slowDownItem->setCallback(CC_CALLBACK_0(Simulator::menuChangeSpeedCallback, this, 2.0));
	actionbar->moveItem->setCallback(CC_CALLBACK_1(Simulator::menuMoveGridCallback, this));
	actionbar->zoomInItem->setCallback(CC_CALLBACK_0(Simulator::menuZoomCallback, this, 1.10));
	actionbar->zoomOutItem->setCallback(CC_CALLBACK_0(Simulator::menuZoomCallback, this, 1/1.10));

	for (const auto &p : grid->squares)
	{
		auto square = p.second;
		square->setCallback(CC_CALLBACK_0(Simulator::gridSquareCallback, this, square->grid_coord));
	}
}

void Simulator::run(float dt) 
{
	for (auto robot : robots) 
	{
		if (robot->getNumberOfRunningActions() == 0 && !robot->screen_path.empty())
			robot->move(dt);
		
		if (robot->grid_path.empty())
			robotController->definePathOf(robot);

		if (allPackagesWereDelivered() && robot->isParked())
			robot->stopwatch->stop();
	}

	if (allRobotsAreParked() && allPackagesWereDelivered())
		stop();
}

void Simulator::start()
{
	createRobots();

	for (auto robot : robots)
	{
		robot->stopwatch->setSpeedFactor(speed_factor);
		robot->stopwatch->start();
	}
		
	robotController->robots = robots;
	schedule(CC_SCHEDULE_SELECTOR(Simulator::run), 0.001f * speed_factor);
	stopwatch->setSpeedFactor(speed_factor);
	stopwatch->start();
	isRunning = true;
}

void Simulator::stop()
{
	unschedule(CC_SCHEDULE_SELECTOR(Simulator::run));
	stopwatch->stop();
	for (auto robot : robots)
		robot->stopwatch->start();

	isRunning = false;
}

void Simulator::proceed()
{
	stopwatch->start();
	for (auto robot : robots)
		robot->stopwatch->start();


	schedule(CC_SCHEDULE_SELECTOR(Simulator::run), 0.001f * speed_factor);
	isRunning = true;
}

void Simulator::createRobots() {
	if (!robots.empty()) return;

	for (Point start : grid->starts) 
	{
		auto robot = Robot::create();
		robot->initWithFile("Robot.png");
		robot->setPosition(grid->getPositionOf(start));
		robot->setColor(Color3B(150, 150, 150));
		robot->setContentSize(Size(grid->square_size, grid->square_size));
		robot->grid_coord = start;
		robot->grid_start = start;

		auto physicsBody = PhysicsBody::createBox(Size(30.0f, 30.0f), PhysicsMaterial(0.1f, 1.0f, 0.0f));
		physicsBody->setGravityEnable(false);
		physicsBody->setDynamic(true);
		physicsBody->setContactTestBitmask(true);
		robot->addComponent(physicsBody);

		grid->addChild(robot);

		robots_bodies[physicsBody] = robot;
		robots.push_back(robot);
	}
}

bool Simulator::allPackagesWereDelivered()
{
	return grid->packages.size() == packages_delivered.size();
}

bool Simulator::allRobotsAreParked()
{
	for (auto robot : robots) 
	{
		if (!robot->isParked())
			return false;
	}
	return true;
}

void Simulator::reset()
{
	for (Point package : grid->packages)
		grid->setState(Square::PACKAGE, package);
	
	grid->available_packages = grid->packages;

	infobar->time = &stopwatch->text;
	for (auto robot : robots)
		robot->removeFromParentAndCleanup(true);
	
	robots.clear();
	packages_delivered.clear();
	stopwatch->reset();
}

void Simulator::menuRunCallback(cocos2d::Ref * pSender)
{
	actionbar->runItem->setNormalImage(Sprite::create("RunBtn_pressed.png"));
	if (isRunning) 
		stop();
	else
		start();
}

void Simulator::menuResetCallback(cocos2d::Ref * pSender)
{
	stop();
	reset();
}

void Simulator::gridSquareCallback(Point coord)
{
	auto robot = robotController->getRobotAt(coord);
	switch (toolbar->selected)
	{
	case Toolbar::PACKAGE:
		grid->setState(Square::PACKAGE, coord);
		break;

	case Toolbar::BEGIN:
		grid->setState(Square::BEGIN, coord);
		break;

	case Toolbar::END:
		grid->setState(Square::END, coord);
		break;

	case Toolbar::ERASE:
		grid->setState(Square::EMPTY, coord);
		break;

	case Toolbar::BLOCKADE:
		grid->setState(Square::BLOCKADE, coord);
		break;
	case Toolbar::PATH:
		if (robot) 
		{
			for (auto coord : robot->complete_path)
				grid->squares[coord]->setColor(Color3B::RED);
		}
		break;
	case Toolbar::CLOCK:
		if (robot)
		{
			auto robot_stopwatch = robot->stopwatch;
			infobar->time = &robot_stopwatch->text;
		}
		break;
	}

}

void Simulator::menuExportCallback(cocos2d::Ref * pSender)
{
	std::ofstream out("times.txt");
	for (auto robot : robots)
	{
		out << robot->stopwatch->toString();
		out << std::endl;
	}

	out.close();
}

void Simulator::menuChangeSpeedCallback(float multiplier)
{
	stop();
	speed_factor *= multiplier;
	start();
}

void Simulator::menuMoveGridCallback(cocos2d::Ref * pSender)
{
	grid->toggleDragAndDrop();
}

void Simulator::menuZoomCallback(float multiplier)
{
	float scale = grid->getScale();
	grid->setScale(scale * multiplier);
}

bool Simulator::onContactBegin(PhysicsContact & contact)
{
	CCLOG("Collision");
	this->stop();
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();

	auto r1 = robots_bodies[bodyA];
	auto r2 = robots_bodies[bodyB];

	r1->pause();
	r2->pause();
	//r1->stopAllActions();
	//r2->stopAllActions();

	robotController->repath(r1, r2);

	this->resume();

	return true;
}

void Simulator::robotIsAtDelivery(EventCustom* event)
{
	Robot* robot = static_cast<Robot*>(event->getUserData());
	Util::addIfUnique<Point>(&packages_delivered, robot->grid_package);
}

void Simulator::robotIsAtPackage(EventCustom* event)
{
	// TODO: Remove package from static_collidables and recalculate every robot path
	Robot* robot = static_cast<Robot*>(event->getUserData());
	grid->setState(Square::EMPTY, robot->grid_package);
}
