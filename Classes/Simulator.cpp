#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "AStar.hpp"
#include <algorithm>
#include <vector>
#include <limits>
#include "Util.h"

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
		square->setCallback(CC_CALLBACK_0(Simulator::gridSquareCallback, this, square));
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

    return true;
}

void Simulator::run(float dt) 
{
	for (auto robot : this->robots) {
		if (!robot->path.empty())
		{	
			auto next_position = robot->path.back();
			auto next_square = this->grid->squares.at(next_position);

			// Move if the path is clear of robots
			if (!isCollisionImminent(next_position))
			{
				robot->setPosition(next_square->getPosition());
				robot->path.pop_back();
				robot->grid_position = next_position;
			}
			//else
			//{
			//	// Case 1: if the current position is NOT on the path of the collidable robot just wait
				// colliding_robot = getCollidingRobot(next_position);
				// if(colliding_robot.path.contains(robot.grid_position))
				//		DO NOTHING
				// Case 2: if the current position IS on the path of the collidable robot move away(Redo pathing ? )
				// else
				//		MOVEAWAY
			//	// 
			//}
			
			// Remove package when it gets picked up
			if (robot->grid_position == robot->package)
			{
				grid->setState(Square::EMPTY, robot->package);
				Util::removeIfContains(&collidables, robot->package);
			}
		}
		else if (!packages.empty())
		{
			robot->package = this->getClosestFrom(robot->grid_position, packages);
			robot->end = this->getClosestFrom(robot->package, ends);
			robot->path = this->createPath(robot->grid_position, robot->package, robot->end);

			Util::removeIfContains(&packages, robot->package);
		}
		else
		{
			// TODO: remove robots as the reach destination? probrably not
			/*this->unscheduleAllSelectors();
			this->running = false;*/
		}
			
	}

}

vector<Point> Simulator::createPath(Point origin, Point package, Point end)
{
	generator.clearCollisions();
	generator.addCollisions(collidables);
	generator.removeCollision(package);

	auto pathToPackage = generator.findPath({ origin.x, origin.y }, { package.x, package.y });
	auto pathToDelivery = generator.findPath({ package.x, package.y }, { end.x, end.y });
	pathToDelivery.pop_back(); // Removes repeated package point from the path
	pathToDelivery.insert(pathToDelivery.end(), pathToPackage.begin(), pathToPackage.end());
	pathToDelivery.pop_back(); // Remove the first coordinate since we are already in it
	
	return pathToDelivery;
}

void Simulator::createRobots() {
	if (!robots.empty()) return;

	for (Point start : starts) 
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

// TODO: (Optimization) Maybe change this so it return a path instead of a point for sake of optimization
Point Simulator::getClosestFrom(Point origin, vector<Point> destinations) {
	Point closest;
	int min_size = std::numeric_limits<int>::max();

	for (Point destination : destinations)
	{
		generator.clearCollisions();
		generator.addCollisions(collidables);
		generator.removeCollision(destination);

		auto path = generator.findPath({ origin.x, origin.y }, { destination.x, destination.y });
		if (path.size() < min_size) {
			closest = destination;
			min_size = path.size();
		}
	}

	return closest;
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
	// TODO: (refactoring) create an update function in the grid that constantly enquires these vectors, and when one of them is changed it would automaticly change the grid visual
	// Maybe calling and update function only when one of those vectors are changed
	for (Point start : saved_starts)
		grid->setState(Square::BEGIN, start);

	for (Point end : saved_ends)
		grid->setState(Square::END, end);

	for (Point package : saved_packages)
		grid->setState(Square::PACKAGE, package);
	
	starts = saved_starts;
	ends = saved_ends;
	packages = saved_packages;
	collidables = saved_collidables;

	for (Robot* robot : this->robots)
		robot->removeFromParent();
	
	this->robots = {};

	this->saved = false;
}

void Simulator::save()
{
	if (this->saved) return;

	saved_starts = starts;
	saved_ends = ends;
	saved_packages = packages;
	saved_collidables = collidables;

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
		this->schedule(CC_SCHEDULE_SELECTOR(Simulator::run), 0.15f);
	else
		this->unscheduleAllSelectors();

	this->running = !this->running;
}

void Simulator::menuResetCallback(cocos2d::Ref * pSender)
{
	this->unscheduleAllSelectors();
	this->load();
	this->running = false;
}

void Simulator::gridSquareCallback(Square* square)
{
	auto grid_position = square->gridLocation;

	switch (this->toolbar->selected)
	{
	case Toolbar::PACKAGE:
		grid->setState(Square::PACKAGE, square->gridLocation);
		Util::addIfUnique<Point>(&packages, grid_position);
		Util::addIfUnique<Point>(&collidables, grid_position);
		break;

	case Toolbar::BEGIN:
		grid->setState(Square::BEGIN, square->gridLocation);
		Util::addIfUnique<Point>(&starts, grid_position);
		break;

	case Toolbar::END:
		grid->setState(Square::END, square->gridLocation);
		Util::addIfUnique<Point>(&ends, grid_position);
		break;

	case Toolbar::ERASE:
		vector<Point>* vector = NULL;

		switch (square->state)
		{
		case Square::PACKAGE:
			vector = &packages;
			break;
		case Square::BEGIN:
			vector = &starts;
			break;
		case Square::END:
			vector = &ends;
			break;
		}

		// TODO: those two function are always together, maybe move setState of grid inside the remove element?
		grid->setState(Square::EMPTY, square->gridLocation);
		Util::removeIfContains(vector, grid_position);
		

	}
}
