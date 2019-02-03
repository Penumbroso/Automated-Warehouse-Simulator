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
	this->addChild(grid);

	toolbar = Toolbar::create();
	toolbar->setCallback(toolbar->runItem, CC_CALLBACK_1(Simulator::menuRunCallback, this));
	toolbar->setCallback(toolbar->packageItem, CC_CALLBACK_0(Simulator::menuToolCallback, this, Tool::PACKAGE));
	toolbar->setCallback(toolbar->beginItem, CC_CALLBACK_0(Simulator::menuToolCallback, this, Tool::BEGIN));
	toolbar->setCallback(toolbar->endItem, CC_CALLBACK_0(Simulator::menuToolCallback, this, Tool::END));
	toolbar->setCallback(toolbar->eraseItem, CC_CALLBACK_0(Simulator::menuToolCallback, this, Tool::ERASE));
	toolbar->setCallback(toolbar->resetItem, CC_CALLBACK_1(Simulator::menuResetCallback, this));
	this->addChild(toolbar);

    return true;
}

void Simulator::tick(float dt) 
{

	// TODO: change state of simulator when all packages have been delivered.
	// It ends when the list of packages to be delivered is empty. ( the clone one )

	for (Robot* robot : this->robots) {
		if (!robot->path.empty())
		{	// TODO: create smooth movement instead of current grid based movement.
			// Move pixel by pixel until it gets to the correct position ( got to check every move ).
			// Then pop the path and do it again.

			// TODO: check for collision. If so, recreate path with obstacle.
			auto pos = robot->path.back();
			int x = pos.x;
			int y = pos.y;
			if (x == robot->package.x && y == robot->package.y) 
			{
				this->grid->squares[x][y]->setColor(Color3B::WHITE);
				auto it = std::find(g_packages.begin(), g_packages.end(), robot->package);
				if (it != g_packages.end()) g_packages.erase(it);
			}
			auto position = Point(g_square_size / 2 + x * g_square_size, g_square_size / 2 + y * g_square_size);
			robot->setPosition(position);
			robot->path.pop_back();
			robot->grid_position = Point(x, y);
		}
		else if (!g_packages.empty()) 
			this->createPath(robot);
	}

}

void Simulator::createPath(Robot* robot)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	int numberOfLines = visibleSize.height / g_square_size;
	int numberOfColumns = visibleSize.width / g_square_size;

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

	Point destination;
	float shortest_distance = std::numeric_limits<float>::max();
	for (Point package : g_packages) 
	{
		float distance = abs(package.x - robot->grid_position.x) + abs(package.y - robot->grid_position.y);
		if (distance < shortest_distance)
		{
			shortest_distance = distance;
			destination = package;
		}
	}

	AStar::Vec2i dest;
	dest.x = destination.x;
	dest.y = destination.y;
	generator.removeCollision(dest);

	int min_length = std::numeric_limits<int>::max();
	std::vector<AStar::Vec2i> min_path;
	for (Point end : g_end) 
	{
		auto pathToPackage = generator.findPath({ (int)robot->grid_position.x, (int)robot->grid_position.y }, { (int)destination.x, (int)destination.y });
		auto pathToDelivery = generator.findPath({ (int)destination.x, (int)destination.y }, { (int)end.x, (int)end.y });
		pathToDelivery.pop_back();
		pathToDelivery.insert(pathToDelivery.end(), pathToPackage.begin(), pathToPackage.end());

		if (pathToDelivery.size() < min_length) min_path = pathToDelivery;
	}

	robot->package = destination;
	robot->path = min_path;
}

void Simulator::createRobots() {
	if (!robots.empty()) return;

	for (Point start : g_start) 
	{
		auto robot = Robot::create();
		robot->initWithFile("Robot.png");
		robot->setPosition(g_square_size / 2 + (start.x) * g_square_size, g_square_size / 2 + (start.y) * g_square_size);
		robot->setColor(Color3B(200, 100, 100));
		robot->setContentSize(Size(g_square_size, g_square_size));
		robot->grid_position = start;
		grid->addChild(robot);

		this->createPath(robot);

		this->robots.push_back(robot);
	}
}

void Simulator::load()
{
	for (Point start : s_start)
	{
		int x = start.x;
		int y = start.y;
		grid->squares[x][y]->state = Square::State::START;
		grid->squares[x][y]->setColor(Color3B::BLUE);
	}

	for (Point end : s_end)
	{
		int x = end.x;
		int y = end.y;
		grid->squares[x][y]->state = Square::State::END;
		grid->squares[x][y]->setColor(Color3B::RED);
	}

	for (Point package : s_packages)
	{
		int x = package.x;
		int y = package.y;
		grid->squares[x][y]->state = Square::State::FILLED;
		grid->squares[x][y]->setColor(Color3B::GRAY);
	}

	g_start = s_start;
	g_end = s_end;
	g_packages = s_packages;

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

	s_start = g_start;
	s_end = g_end;
	s_packages = g_packages;

	this->saved = true;
}

void Simulator::menuToolCallback(Tool tool)
{
	g_current_tool = tool;
}

void Simulator::menuRunCallback(cocos2d::Ref * pSender)
{
	this->save();
	this->createRobots();

	if (!this->running)
		this->schedule(CC_SCHEDULE_SELECTOR(Simulator::tick), 0.15f);
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
