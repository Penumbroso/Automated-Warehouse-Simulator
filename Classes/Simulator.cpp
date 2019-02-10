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
	// TODO: change state of simulator when all packages have been delivered.
	// It ends when the list of packages to be delivered is empty. ( the clone one )

	for (Robot* robot : this->robots) {
		if (!robot->path.empty())
		{	
			// TODO: check for collision. If so, recreate path with obstacle.
			auto next_position = robot->path.back();
			auto square = this->grid->squares.at(next_position);
			if (next_position == robot->package) 
			{
				grid->setState(Square::EMPTY, square->gridLocation);

				// Remove package from collidables
				auto it = std::find(collidables.begin(), collidables.end(), robot->package);
				if (it != collidables.end()) collidables.erase(it);
			}

			robot->setPosition(square->getPosition());
			robot->path.pop_back();
			robot->grid_position = next_position;
		}
		else if (!packages.empty())
		{
			// TODO: substitute this function for the getClosestFrom, the problem is: I believe collision is not allowing a correct assesment of which is the closest.
			auto package = this->getClosestFrom(robot->grid_position, packages);
			auto end = this->getClosestFrom(package, ends);
			auto path = this->createPath(robot->grid_position, package, end);

			robot->path = path;
			robot->package = package;

			// Remove package from available packages
			auto it = std::find(packages.begin(), packages.end(), robot->package);
			if (it != packages.end()) packages.erase(it);
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
	
	return pathToDelivery;
}

void Simulator::createRobots() {
	if (!robots.empty()) return;

	for (Point start : starts) 
	{
		auto robot = Robot::create();
		robot->initWithFile("Robot.png");
		robot->setPosition(grid->getPositionOf(start));
		robot->setColor(Color3B(200, 100, 100));
		robot->setContentSize(Size(grid->square_size, grid->square_size));
		robot->grid_position = start;
		grid->addChild(robot);

		this->robots.push_back(robot);
	}
}

// TODO: rename this or change it so it returns the path instead of the point, this would optmize.
Point Simulator::getClosestFrom(Point origin, vector<Point> destinations) {
	Point closest;
	int min_length = std::numeric_limits<int>::max();

	for (Point destination : destinations)
	{
		generator.clearCollisions();
		generator.addCollisions(collidables);
		generator.removeCollision(destination);

		auto path = generator.findPath({ origin.x, origin.y }, { destination.x, destination.y });
		if (path.size() < min_length) {
			closest = destination;
			min_length = path.size();
		}
	}

	return closest;
}

void Simulator::load()
{
	// TODO: perhaps intead of calling this function, I should create an update function in the grid that constantly enquires these vectors, and when one of them is changed it would automaticly change the grid visual
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
	switch (this->toolbar->selected)
	{
	case Toolbar::PACKAGE:
		// TODO: change filled to package
		grid->setState(Square::PACKAGE, square->gridLocation);

		// TODO: its allowing to add duplicates, it shouldnt
		packages.push_back(square->gridLocation);
		collidables.push_back(square->gridLocation);
		break;

	case Toolbar::BEGIN:
		grid->setState(Square::BEGIN, square->gridLocation);

		// TODO: its allowing to add duplicates, it shouldnt
		starts.push_back(square->gridLocation);
		break;

	case Toolbar::END:
		grid->setState(Square::END, square->gridLocation);

		// TODO: its allowing to add duplicates, it shouldnt
		ends.push_back(square->gridLocation);
		break;

	case Toolbar::ERASE:
		vector<Point>* vector;

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

		auto it = std::find(vector->begin(), vector->end(), square->gridLocation);
		if (it != vector->end()) vector->erase(it);

		grid->setState(Square::EMPTY, square->gridLocation);
	}
}
