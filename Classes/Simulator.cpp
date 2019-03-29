#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "AStar.hpp"
#include <algorithm>

USING_NS_CC;

bool Simulator::init()
{
    if ( !Scene::init() )
        return false;
    
	isRunning = false;
	speed_factor = 1.0f;

	// TODO: make the grid draggable.
	// TODO: make grid zoomable.
	grid = Grid::create();
	stopwatch = Stopwatch::create();
	infobar = Infobar::create();
	toolbar = Toolbar::create();
	actionbar = Actionbar::create();
	robotController = RobotController::create();

	grid->setPosition(30, 0);
	infobar->time = &stopwatch->text;
	robotController->grid = grid;

	this->addChild(grid);
	this->addChild(stopwatch);
	this->addChild(infobar);
	this->addChild(toolbar);
	this->addChild(actionbar);
	this->addChild(robotController);

	this->setCallbacks();

    return true;
}

void Simulator::setCallbacks()
{
	actionbar->runItem->setCallback(CC_CALLBACK_1(Simulator::menuRunCallback, this));
	actionbar->exportItem->setCallback(CC_CALLBACK_1(Simulator::menuExportCallback, this));
	actionbar->resetItem->setCallback(CC_CALLBACK_1(Simulator::menuResetCallback, this));
	actionbar->speedUpItem->setCallback(CC_CALLBACK_1(Simulator::menuSpeedUpCallback, this));
	actionbar->slowDownItem->setCallback(CC_CALLBACK_1(Simulator::menuSlowDownCallback, this));
	actionbar->moveItem->setCallback(CC_CALLBACK_1(Simulator::menuMoveGridCallback, this));

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
		// Remove package from grid if there is a robot on top of it.
		if (robot->isAtPackage())
			grid->setState(Square::EMPTY, robot->package);

		if (!robot->path.empty())
		{
			robot->stopAllActions();
			auto pixel_position = grid->getPositionOf(robot->grid_coord);
			robot->setPosition(pixel_position);

			robotController->preventCollisionOf(robot);

			robot->move(dt);

			// TODO: solve problem of diagonal movement being faster than vertical and horizontal movement
			auto moveTo = MoveTo::create(0.2f * speed_factor, grid->getPositionOf(robot->grid_coord));
			robot->runAction(moveTo);

			if (robot->isAtDeliverty() && robot->isFull())
				Util::addIfUnique<Point>(&packages_delivered, robot->package);

			robot->updateState();
		}

		if (robot->path.empty())
			robotController->definePathOf(robot);

		if (allPackagesWereDelivered() && robot->isParked())
			robot->stopwatch->stop();
	}

	if (allRobotsAreParked() && allPackagesWereDelivered())
		this->stop();
}

void Simulator::start()
{
	this->createRobots();

	for (auto robot : robots)
	{
		robot->stopwatch->setSpeedFactor(speed_factor);
		robot->stopwatch->start();
	}
		
	
	this->robotController->robots = robots;
	this->schedule(CC_SCHEDULE_SELECTOR(Simulator::run), 0.2f * speed_factor);
	this->stopwatch->setSpeedFactor(speed_factor);
	this->stopwatch->start();
	this->isRunning = true;
}

void Simulator::stop()
{
	this->unschedule(CC_SCHEDULE_SELECTOR(Simulator::run));
	this->stopwatch->stop();
	this->isRunning = false;
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
		robot->start = start;
		grid->addChild(robot);

		this->robots.push_back(robot);
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

	this->infobar->time = &this->stopwatch->text;
	for (Robot* robot : this->robots)
		robot->removeFromParentAndCleanup(true);
	
	robots.clear();
	packages_delivered.clear();
	stopwatch->reset();
}

void Simulator::menuRunCallback(cocos2d::Ref * pSender)
{
	this->actionbar->runItem->setNormalImage(Sprite::create("RunBtn_pressed.png"));
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
	Robot * robot = this->robotController->getRobotAt(coord);
	grid->enableDragAndDrop(false);
	switch (this->toolbar->selected)
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
			this->infobar->time = &robot_stopwatch->text;
		}
		break;
	case Toolbar::MOVE:
		//grid->enableDragAndDrop(false);
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

void Simulator::menuSpeedUpCallback(cocos2d::Ref * pSender)
{
	stop();
	speed_factor /= 2;
	start();
}

void Simulator::menuSlowDownCallback(cocos2d::Ref * pSender)
{
	stop();
	speed_factor *= 2;
	start();
}

void Simulator::menuMoveGridCallback(cocos2d::Ref * pSender)
{
	this->grid->enableDragAndDrop(true);
}
