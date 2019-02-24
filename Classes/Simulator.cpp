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
		
	this->addChild(grid);

	toolbar = Toolbar::create();
	toolbar->runItem->setCallback(CC_CALLBACK_1(Simulator::menuRunCallback, this));
	toolbar->packageItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::PACKAGE));
	toolbar->beginItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::BEGIN));
	toolbar->endItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::END));
	toolbar->eraseItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::ERASE));
	toolbar->resetItem->setCallback(CC_CALLBACK_1(Simulator::menuResetCallback, this));

	this->addChild(toolbar);

	generator.setWorldSize({ grid->number_of_columns, grid->number_of_lines });
	generator.setHeuristic(AStar::Heuristic::manhattan);
	generator.setDiagonalMovement(false);

	auto _listener = EventListenerCustom::create("prevent_collision", CC_CALLBACK_1(Simulator::preventCollision, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

    return true;
}

void Simulator::run(float dt) 
{
	for (auto robot : this->robots) 
	{

		// Remove package from grid if there is a robot on top of it.
		if (robot->grid_position == robot->package)
			grid->setState(Square::EMPTY, robot->package);

		// Update screen position of a robot
		auto current_grid_position = robot->grid_position;
		auto current_screen_position = grid->getPositionOf(current_grid_position);
		robot->setPosition(current_screen_position);

		if (robot->path.empty())
			this->definePathOf(robot);

		if (grid->packages.size() == this->delivered)
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
		robot->grid_position = start;
		grid->addChild(robot);

		this->robots.push_back(robot);
	}
}

void Simulator::definePathOf(Robot * robot)
{
	if (robot->state == Robot::EMPTY && !grid->available_packages.empty()) 
	{
		robot->path = this->findShortestPath(robot->grid_position, grid->available_packages);
		robot->destination = robot->path[0];
		robot->package = robot->destination;
		Util::removeIfContains(&grid->available_packages, robot->destination);
	}
	else
	{
		robot->path = this->findShortestPath(robot->grid_position, grid->ends);
		robot->destination = robot->path[0];
	}
}

void Simulator::preventCollision(EventCustom * event)
{
	Robot* robot = static_cast<Robot*>(event->getUserData());
	auto next_position = robot->path.back();

	if (isCollisionImminent(next_position))
	{
		auto collision_robot = this->getRobotAt(next_position);
		auto path = collision_robot->path;
		if (!Util::contains<Point>(&path, robot->grid_position))
		{
			robot->path.push_back(robot->grid_position);
		}
		else
		{
			grid->static_collidables.push_back(next_position);
			robot->path = this->findShortestPath(robot->grid_position, { robot->destination });
			grid->static_collidables.pop_back();

		}
	}
}

vector<Point> Simulator::findShortestPath(Point origin, vector<Point> destinations) {
	vector<Point> shortest_path;
	int min_size = std::numeric_limits<int>::max();

	for (Point destination : destinations)
	{
		generator.clearCollisions();
		generator.addCollisions(grid->static_collidables);
		generator.removeCollision(destination);

		auto path = generator.findPath({ origin.x, origin.y }, { destination.x, destination.y });
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
		if (robot->grid_position == grid_position)
			return robot;
	}
	return nullptr;
}

void Simulator::load()
{
	// TODO: (refactoring) create an update function in the grid that constantly enquires these vectors, 
	// and when one of them is changed it would automaticly change the grid visual
	// Maybe calling and update function only when one of those vectors are changed
	for (Point package : saved_packages)
		grid->setState(Square::PACKAGE, package);
	
	grid->available_packages = saved_packages;

	for (Robot* robot : this->robots)
		robot->removeFromParent();
	
	this->robots = {};

	this->saved = false;
}

void Simulator::save()
{
	if (this->saved) return;

	saved_packages = grid->available_packages;

	this->saved = true;
}

void Simulator::menuToolCallback(Toolbar::Tool tool)
{
	this->toolbar->selected = tool;
}

void Simulator::menuRunCallback(cocos2d::Ref * pSender)
{
	this->save();
	this->createRobots();

	if (!this->running)
	{
		this->schedule(CC_SCHEDULE_SELECTOR(Simulator::run), 0.1f);
		for (auto robot : robots) robot->run();
	}
	else
	{
		this->unscheduleAllSelectors();
		for (auto robot : robots) robot->unscheduleAllSelectors();
	}
		

	this->running = !this->running;
}

void Simulator::menuResetCallback(cocos2d::Ref * pSender)
{
	this->unscheduleAllSelectors();
	this->load();
	this->running = false;
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
