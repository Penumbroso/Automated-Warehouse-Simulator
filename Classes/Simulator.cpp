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
	toolbar->packageItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::Tool::PACKAGE));
	toolbar->beginItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::Tool::BEGIN));
	toolbar->endItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::Tool::END));
	toolbar->eraseItem->setCallback(CC_CALLBACK_0(Simulator::menuToolCallback, this, Toolbar::Tool::ERASE));
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
				// TODO: it should remove from another list that shows only packages without robots searching or carrying them
				auto it = std::find(packages.begin(), packages.end(), robot->package);
				if (it != packages.end()) packages.erase(it);
			}
			// TODO: make conversion from grid position to screen position easier.
			robot->setPosition(square->getPosition());
			robot->path.pop_back();
			robot->grid_position = next_position;
		}
		else if (!packages.empty())
		{
			auto package = this->getClosestPackageFrom(robot->grid_position);
			auto path = this->createPath(robot->grid_position, package);
			robot->path = path;
			robot->package = package;
		}
			
	}

}

vector<Point> Simulator::createPath(Point origin, Point package)
{
	generator.clearCollisions();

	for (Point pack : packages)
		generator.addCollision(pack);
	
	generator.removeCollision(package);

	// Select which path is the shortest from the package to one of the ends
	int min_length = std::numeric_limits<int>::max();
	vector<Point> shortest_path;
	for (Point end : ends) 
	{
		auto pathToPackage = generator.findPath({ origin.x, origin.y }, { package.x, package.y });
		auto pathToDelivery = generator.findPath({ package.x, package.y }, { end.x, end.y });
		pathToDelivery.pop_back();
		pathToDelivery.insert(pathToDelivery.end(), pathToPackage.begin(), pathToPackage.end());

		if (pathToDelivery.size() < min_length) shortest_path = pathToDelivery;
	}
	
	return shortest_path;
}

void Simulator::createRobots() {
	if (!robots.empty()) return;

	for (Point start : starts) 
	{
		auto robot = Robot::create();
		robot->initWithFile("Robot.png");
		// TODO: create method to convert grid position to screen position
		robot->setPosition(g_square_size / 2 + (start.x) * g_square_size, g_square_size / 2 + (start.y) * g_square_size);
		robot->setColor(Color3B(200, 100, 100));
		robot->setContentSize(Size(g_square_size, g_square_size));
		robot->grid_position = start;
		grid->addChild(robot);

		this->robots.push_back(robot);
	}
}

Point Simulator::getClosestPackageFrom(Point position)
{
	Point closest;
	float shortest_distance = std::numeric_limits<float>::max();
	for (Point package : packages)
	{
		float distance = abs(package.x - position.x) + abs(package.y - position.y);
		if (distance < shortest_distance)
		{
			shortest_distance = distance;
			closest = package;
		}
	}
	return closest;
}

void Simulator::load()
{
	for (Point start : s_start)
	{
		int x = start.x;
		int y = start.y;
		grid->squares.at(Point(x, y))->setColor(Color3B::GRAY);
		grid->squares.at(Point(x, y))->state = Square::State::START;
	}

	for (Point end : s_end)
	{
		int x = end.x;
		int y = end.y;
		grid->squares.at(Point(x, y))->state = Square::State::END;
		grid->squares.at(Point(x, y))->setColor(Color3B::GRAY);
	}

	for (Point package : s_packages)
	{
		int x = package.x;
		int y = package.y;
		grid->squares.at(Point(x, y))->state = Square::State::FILLED;
		grid->squares.at(Point(x, y))->setColor(Color3B::GRAY);
	}

	starts = s_start;
	ends = s_end;
	packages = s_packages;

	s_start = {};
	s_end = {};
	s_packages = {};

	for (Robot* robot : this->robots)
		robot->removeFromParent();
	
	this->robots = {};
}

void Simulator::save()
{
	if (this->saved) return;

	s_start = starts;
	s_end = ends;
	s_packages = packages;

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
	this->saved = false;
	this->running = false;
}

void Simulator::gridSquareCallback(Square* square)
{
	auto plus = Sprite::create("Plus.png");
	plus->setContentSize(Size(g_square_size - 10, g_square_size- 10));

	auto minus = Sprite::create("Minus.png");
	minus->setContentSize(Size(g_square_size - 10, g_square_size - 10));

	switch (this->toolbar->selected)
	{
	case Toolbar::Tool::PACKAGE:
		square->setColor(Color3B::GRAY);
		square->state = Square::FILLED;
		packages.push_back(square->gridLocation);
		break;
	case Toolbar::Tool::BEGIN:
		square->setColor(Color3B::GRAY);
		square->state = Square::START;
		grid->addChild(plus);
		plus->setPosition(square->getPosition());
		starts.push_back(square->gridLocation);
		break;
	case Toolbar::Tool::END:
		square->setColor(Color3B::GRAY);
		square->state = Square::END;
		grid->addChild(minus);
		minus->setPosition(square->getPosition());
		ends.push_back(square->gridLocation);
		break;
	case Toolbar::Tool::ERASE:
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
