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
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();



	auto background = LayerColor::create(Color4B::RED);
	this->addChild(background);

	// TODO: make the grid draggable.
	// TODO: make grid zoomable.
	grid = Grid::create();
	for (const auto &p : grid->squares)
	{
		auto square = p.second;
		// TODO: put the CC_CALLBACK inside the grid and instead pass the only the function
		// It might be better not to do this.
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
				square->setColor(Color3B::WHITE);

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
	pathToDelivery.pop_back();
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
		robot->setContentSize(Size(g_square_size, g_square_size));
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
	for (Point start : s_start)
		grid->setState(Square::START, start);

	for (Point end : s_end)
		grid->setState(Square::END, end);

	for (Point package : s_packages)
		grid->setState(Square::FILLED, package);
	
	starts = s_start;
	ends = s_end;
	packages = s_packages;
	collidables = s_collidables;

	for (Robot* robot : this->robots)
		robot->removeFromParent();
	
	this->robots = {};

	this->saved = false;
}

void Simulator::save()
{
	if (this->saved) return;

	s_start = starts;
	s_end = ends;
	s_packages = packages;
	s_collidables = collidables;

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
	// TODO: change the location where plus and minus are added, it should make part of the logic of the simulator, but rather of the GUI.
	auto plus = Sprite::create("Plus.png");
	plus->setContentSize(Size(g_square_size - 10, g_square_size- 10));

	auto minus = Sprite::create("Minus.png");
	minus->setContentSize(Size(g_square_size - 10, g_square_size - 10));

	switch (this->toolbar->selected)
	{
	case Toolbar::PACKAGE:
		// TODO: change filled to package
		grid->setState(Square::FILLED, square->gridLocation);
		packages.push_back(square->gridLocation);

		// TODO: its allowing to add duplicates, it shouldnt
		collidables.push_back(square->gridLocation);
		break;

	case Toolbar::BEGIN:
		// TODO: change start to begin
		grid->setState(Square::START, square->gridLocation);
		// TODO: move plus to the grid function setState
		grid->addChild(plus);
		plus->setPosition(square->getPosition());
		starts.push_back(square->gridLocation);
		break;

	case Toolbar::END:
		grid->setState(Square::END, square->gridLocation);
		grid->addChild(minus);
		// TODO: move the minus sprite to the grid function setState
		minus->setPosition(square->getPosition());
		ends.push_back(square->gridLocation);
		break;

	case Toolbar::ERASE:
		vector<Point>* vector;

		switch (square->state)
		{
		case Square::FILLED:
			vector = &packages;
			break;
		case Square::START:
			vector = &starts;
			break;
		case Square::END:
			vector = &ends;
			break;
		}

		auto it = std::find(vector->begin(), vector->end(), square->gridLocation);
		if (it != vector->end()) vector->erase(it);
		square->state = Square::EMPTY;
		square->setColor(Color3B::WHITE);
	}
}
