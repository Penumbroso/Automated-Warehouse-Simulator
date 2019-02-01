#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "Globals.h"
#include "AStar.hpp"
#include <algorithm>
#include "Toolbar.h"
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

	auto toolbar = Toolbar::create();
	this->addChild(toolbar);

	this->schedule(CC_SCHEDULE_SELECTOR(Simulator::tick), 0.15f);
    return true;
}

void Simulator::tick(float dt) {
	if (!g_running) CCLOG("Paused");

	// TODO: change state of simulator when all packages have been delivered.
	// It ends when the list of packages to be delivered is empty. ( the clone one )
	if (g_running) 
	{
		if (this->robots.empty())
			this->createRobots();
		for (Robot* robot : this->robots) {
			if (!robot->path.empty())
			{	// TODO: create smooth movement instead of current grid based movement.
				// Move pixel by pixel until it gets to the correct position ( got to check every move ).
				// Then pop the path and do it again.
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
			else if (!g_packages.empty()) this->createPath(robot);
		}
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
