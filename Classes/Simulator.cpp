#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "AStar.hpp"
#include <algorithm>
#include <limits>

USING_NS_CC;

bool Simulator::init()
{
    if ( !Scene::init() )
        return false;
    
	isRunning = false;

	auto background = LayerColor::create(Color4B::RED);
	this->addChild(background);

	// TODO: make the grid draggable.
	// TODO: make grid zoomable.
	grid = Grid::create();
	for (const auto &p : grid->squares)
	{
		auto square = p.second;
		square->setCallback(CC_CALLBACK_0(Simulator::gridSquareCallback, this, square->grid_coord));
	}

	grid->setPosition(30, 0);
	this->addChild(grid);

	infobar = Infobar::create();
	this->addChild(infobar);

	toolbar = Toolbar::create();
	toolbar->packageItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::PACKAGE));
	toolbar->beginItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::BEGIN));
	toolbar->endItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::END));
	toolbar->eraseItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::ERASE));
	toolbar->blockadeItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::BLOCKADE));
	toolbar->clockItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::CLOCK));
	toolbar->pathItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::PATH));
	this->addChild(toolbar);

	actionbar = Actionbar::create();
	actionbar->runItem->setCallback(CC_CALLBACK_1(Simulator::menuRunCallback, this));
	actionbar->exportItem->setCallback(CC_CALLBACK_1(Simulator::menuExportCallback, this));
	actionbar->resetItem->setCallback(CC_CALLBACK_1(Simulator::menuResetCallback, this));

	this->addChild(actionbar);

	stopwatch = Stopwatch::create();
	this->addChild(stopwatch);

	// Set infobar clock to be the global simulation time
	infobar->time = &stopwatch->text;

	robotController = RobotController::create();
	robotController->grid = grid;
	this->addChild(robotController);

    return true;
}

void Simulator::run(float dt) 
{
	for (auto robot : robots) 
	{
		// Remove package from grid if there is a robot on top of it.
		if (robot->grid_coord == robot->package)
			grid->setState(Square::EMPTY, robot->package);

		if (!robot->path.empty())
		{
			// This is to prevent the problem where the robot goes out of bounds.
			robot->stopAllActions();
			robot->setPosition(grid->getPositionOf(robot->grid_coord));

			robotController->preventCollisionOf(robot);
			robot->move(dt);

			auto moveTo = MoveTo::create(0.19, grid->getPositionOf(robot->grid_coord));
			robot->runAction(moveTo);

			if (robot->grid_coord == robot->end && robot->state == Robot::FULL)
				Util::addIfUnique<Point>(&packages_delivered, robot->package);

			robot->updateState();
		}

		if (robot->path.empty())
			robotController->definePathOf(robot);

		// TODO: substitute the second condition as a function called isRobotParked
		if (allPackagesWereDelivered() && robot->isParked())
			robot->stopwatch->stop();
	}

	if (allRobotsAreParked() && allPackagesWereDelivered())
		stop();
}

void Simulator::start()
{
	this->createRobots();

	// TODO: make this into a function called startIndividualRobotsWatches
	for (auto robot : robots)
		robot->stopwatch->start();
	
	this->robotController->robots = robots;
	this->schedule(CC_SCHEDULE_SELECTOR(Simulator::run), 0.2f);
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
}

/*************/
/* Callbacks */
/*************/

void Simulator::menuToolCallback(Toolbar::Tool tool)
{
	this->toolbar->selected = tool;
}

void Simulator::menuRunCallback(cocos2d::Ref * pSender)
{
	if (isRunning) 
		stop();
	else
		start();
}

void Simulator::menuResetCallback(cocos2d::Ref * pSender)
{
	stop();
	stopwatch->reset();
	reset();
}

void Simulator::gridSquareCallback(Point coord)
{
	Robot * robot = this->robotController->getRobotAt(coord);
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
	// TODO: separete path and clock into different function
	case Toolbar::PATH:
		for (auto coord : robot->complete_path)
		{
			grid->squares[coord]->setColor(Color3B::RED);
		}
		break;
	case Toolbar::CLOCK:
		auto robot_stopwatch = robot->stopwatch;
		this->infobar->time = &robot_stopwatch->text;
	}

}

void Simulator::menuExportCallback(cocos2d::Ref * pSender)
{
	std::ofstream out("output.txt");
	for (auto robot : robots)
	{
		out << robot->stopwatch->toString();
		out << std::endl;
	}

	out.close();
}
