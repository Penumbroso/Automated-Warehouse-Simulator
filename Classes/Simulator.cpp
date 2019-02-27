#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "AStar.hpp"
#include <algorithm>
#include <vector>
#include <limits>

USING_NS_CC;

Scene* Simulator::createScene()
{
    return Simulator::create();
}

bool Simulator::init()
{
    if ( !Scene::init() )
        return false;
    
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

	toolbar = Toolbar::create();
	toolbar->runItem->setCallback(CC_CALLBACK_1(Simulator::menuRunCallback, this));
	toolbar->packageItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::PACKAGE));
	toolbar->beginItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::BEGIN));
	toolbar->endItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::END));
	toolbar->eraseItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::ERASE));
	toolbar->resetItem->setCallback(CC_CALLBACK_1(Simulator::menuResetCallback, this));

	this->addChild(toolbar);

	stopwatch = Stopwatch::create();
	this->addChild(stopwatch);

	toolbar->addChild(stopwatch->label);

	path_generator.setWorldSize({ grid->number_of_columns, grid->number_of_lines });
	path_generator.setHeuristic(AStar::Heuristic::manhattan);
	path_generator.setDiagonalMovement(false);

    return true;
}

void Simulator::run(float dt) 
{
	for (auto robot : this->robots) 
	{
		if (!robot->path.empty())
		{
			this->preventCollisionOf(robot);
			robot->move(dt);
		}

		// Remove package from grid if there is a robot on top of it.
		if (robot->grid_coord == robot->package) 
		{
			grid->setState(Square::EMPTY, robot->package);
		}
			
		// Update screen position of a robot
		auto current_grid_position = robot->grid_coord;
		auto current_screen_position = grid->getPositionOf(current_grid_position);
		robot->setPosition(current_screen_position);

		if (robot->path.empty())
			this->definePathOf(robot);

		if (grid->packages.size() == this->packages_delivered)
			this->unscheduleAllSelectors();
	}
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
		grid->addChild(robot);

		this->robots.push_back(robot);
	}
}

void Simulator::definePathOf(Robot * robot)
{
	if (robot->state == Robot::EMPTY && !grid->available_packages.empty()) 
	{
		robot->path = this->findShortestPath(robot->grid_coord, grid->available_packages);
		robot->destination = robot->path[0];
		robot->package = robot->destination;
		Util::removeIfContains(&grid->available_packages, robot->destination);
	}
	else
	{
		robot->path = this->findShortestPath(robot->grid_coord, grid->ends);
		robot->destination = robot->path[0];
	}
}

void Simulator::preventCollisionOf(Robot * robot)
{
	auto next_position = robot->path.back();

	if (isCollisionImminent(next_position))
	{
		auto collision_robot = this->getRobotAt(next_position);
		auto path = collision_robot->path;
		if (!Util::contains<Point>(&path, robot->grid_coord))
		{
			robot->path.push_back(robot->grid_coord);
		}
		else
		{
			grid->static_collidables.push_back(next_position);
			robot->path = this->findShortestPath(robot->grid_coord, { robot->destination });
			grid->static_collidables.pop_back();

		}
	}
}

vector<Point> Simulator::findShortestPath(Point origin, vector<Point> destinations) {
	vector<Point> shortest_path;
	int min_size = std::numeric_limits<int>::max();

	for (Point destination : destinations)
	{
		path_generator.clearCollisions();
		path_generator.addCollisions(grid->static_collidables);
		path_generator.removeCollision(destination);

		auto path = path_generator.findPath({ origin.x, origin.y }, { destination.x, destination.y });
		if (path.size() < min_size) {
			shortest_path = path;
			min_size = path.size();
		}
	}

	return shortest_path;
}

bool Simulator::isCollisionImminent(Point next_position)
{
	if (this->getRobotAt(next_position))
		return true;
	else
		return false;
}

Robot * Simulator::getRobotAt(Point grid_position)
{
	for (auto robot : robots)
	{
		if (robot->grid_coord == grid_position)
			return robot;
	}
	return nullptr;
}

void Simulator::load()
{
	// TODO: (refactoring) create an update function in the grid that constantly enquires these vectors, 
	// and when one of them is changed it would automaticly change the grid visual
	// Maybe calling and update function only when one of those vectors are changed
	for (Point package : grid->packages)
		grid->setState(Square::PACKAGE, package);
	
	grid->available_packages = grid->packages;

	for (Robot* robot : this->robots)
		robot->removeFromParent();
	
	this->robots = {};
}

void Simulator::menuToolCallback(Toolbar::Tool tool)
{
	this->toolbar->selected = tool;
}

void Simulator::menuRunCallback(cocos2d::Ref * pSender)
{
	this->createRobots();

	if (!this->isRunning) 
	{
		this->schedule(CC_SCHEDULE_SELECTOR(Simulator::run), 0.2f);
		this->stopwatch->start();
	}
	else
	{
		this->unscheduleAllSelectors();
		this->stopwatch->stop();
	}

	this->isRunning = !this->isRunning;
}

void Simulator::menuResetCallback(cocos2d::Ref * pSender)
{
	this->unscheduleAllSelectors();
	this->load();
	this->stopwatch->reset();
	this->isRunning = false;
}

void Simulator::gridSquareCallback(Point coord)
{
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
	}
}
