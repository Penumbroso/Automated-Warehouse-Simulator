#include "Simulator.h"
#include "SimpleAudioEngine.h"
#include "Globals.h"
#include "AStar.hpp"
#include <algorithm>
#include "Toolbar.h"
#include <vector>

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

	auto toolbar = Toolbar::create(CC_CALLBACK_1(Simulator::menuPlayCallback, this));
	this->addChild(toolbar);

	this->schedule(CC_SCHEDULE_SELECTOR(Simulator::tick), 0.2f);
    return true;
}

void Simulator::menuPlayCallback(Ref* pSender)
{
	// TODO: create one robot for each start point.
	// Create a 'for' for each begin in the global vector.
	auto robot = Robot::create();
	robot->initWithFile("Robot.png");
	robot->setPosition(g_square_size / 2 + (g_start.x ) * g_square_size, g_square_size / 2 + (g_start.y ) * g_square_size);
	robot->setColor(Color3B(200, 100, 100));
	robot->setContentSize(Size(g_square_size, g_square_size));
	robot->grid_position = g_start;
	grid->addChild(robot);

	this->createPath(robot);

	this->robots.push_back(robot);
	this->state = RUNNING;
}

void Simulator::tick(float dt) {
	if (this->state == EDITING);

	// TODO: change state of simulator when all packages have been delivered.
	// It ends when the list of packages to be delivered is empty. ( the clone one )
	if (this->state == RUNNING) {
		for (Robot* robot : this->robots) {
			if (!robot->path.empty())
			{	// TODO: create smooth movement instead of current grid based movement.
				// Move pixel by pixel until it gets to the correct position ( got to check every move ).
				// Then pop the path and do it again.
				auto pos = robot->path.back();
				auto position = Point(g_square_size / 2 + pos.x * g_square_size, g_square_size / 2 + pos.y * g_square_size);
				robot->setPosition(position);
				robot->path.pop_back();
				robot->grid_position = Point(pos.x, pos.y);
			}
			else
			{
				if (!g_packages.empty()) {
					auto pos = g_packages.back();
					int x = pos.x;
					int y = pos.y;
					// TODO: erase package from the UI and vector when the robot is ON the package placement instead of after delivering.
					// Most likely has to be done on the if instead of else (current)
					this->grid->squares[x][y]->setColor(Color3B::WHITE);
					g_packages.pop_back();
					if (!g_packages.empty()) {
						this->createPath(robot);
					}
				}

			}
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


	Point destination = g_packages.back();
	AStar::Vec2i dest;
	dest.x = destination.x;
	dest.y = destination.y;
	generator.removeCollision(dest);

	// TODO: remove the duplicate destination point that is a result of the merge between both vectors.
	// Perhaps removing the first or last element of one of the vectors solves this easily.
	std::vector<AStar::Vec2i> pathToPackage = generator.findPath({ (int)robot->grid_position.x, (int)robot->grid_position.y }, { (int)destination.x, (int)destination.y });
	std::vector<AStar::Vec2i> pathToDelivery = generator.findPath({ (int)destination.x, (int)destination.y }, { (int)g_end.x, (int)g_end.y });

	pathToDelivery.insert(pathToDelivery.end(), pathToPackage.begin(), pathToPackage.end());
	robot->path = pathToDelivery;
}