#include "RobotController.h"


void RobotController::onEnter()
{
	Node::onEnter();

	path_generator.setWorldSize({ grid->number_of_columns, grid->number_of_lines });
	path_generator.setHeuristic(AStar::Heuristic::manhattan);
	path_generator.setDiagonalMovement(true);

	auto robotCompletedMovementListener = EventListenerCustom::create("robot_completed_movement", CC_CALLBACK_1(RobotController::robotCompletedMovement, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(robotCompletedMovementListener, this);
}

void RobotController::definePathOf(Robot * robot)
{
	vector<Point> destinations;

	if (robot->state == Robot::EMPTY && !grid->available_packages.empty())
	{
		destinations = grid->available_packages;
		robot->grid_path = this->findShortestPath(robot->grid_position, destinations);
		robot->screen_path = this->convertGridPathToScreenPath(robot->grid_path);

		robot->screen_package = robot->screen_path[0];

		Util::removeIfContains<Point>(&grid->available_packages, robot->grid_destination);
	}

	else if(robot->state == Robot::FULL)
	{
		destinations = grid->delivery_points;
		robot->grid_path = this->findShortestPath(robot->grid_position, destinations);
		robot->screen_path = this->convertGridPathToScreenPath(robot->grid_path);

		robot->screen_delivery_point = robot->screen_path[0];
	}

	else if (robot->state == Robot::EMPTY && grid->available_packages.empty()) 
	{
		destinations = { robot->grid_start };
		robot->grid_path = this->findShortestPath(robot->grid_position, destinations);
		robot->screen_path = this->convertGridPathToScreenPath(robot->grid_path);
	}

	robot->grid_destination = robot->grid_path[0];
	robot->screen_destination = robot->screen_path[0];
}

// TODO: should check to see the situation when it comes to diagonal movement since the robot occupies multiple squares on the grid.
void RobotController::preventCollisionOf(Robot * robot)
{
	auto next_position = robot->grid_path.back();

	if (isCollisionImminent(next_position))
	{
		auto collision_robot = getRobotAt(next_position);
		auto path = collision_robot->grid_path;
		
		if (robot->isInThe(path) || collision_robot->grid_path.empty())
		{
			grid->static_collidables.push_back(next_position);
			robot->grid_path = findShortestPath(robot->grid_position, { robot->grid_destination });
			grid->static_collidables.pop_back();
		}
		else
		{
			robot->grid_path.push_back(robot->grid_position);
		}
	}
}

void RobotController::repath(Robot * r1, Robot * r2)
{
	auto next_position = r1->grid_path.back();
	auto path = r2->grid_path;

	if (r1->isInThe(path) || r2->grid_path.empty())
	{
		for (auto p : r2->grid_path)
		{
			grid->static_collidables.push_back(next_position);
		}
		
		r1->grid_path = findShortestPath(r1->grid_position, { r1->grid_destination });
		r1->screen_path = this->convertGridPathToScreenPath(r1->grid_path);

		for (auto p : r2->grid_path)
		{
			grid->static_collidables.pop_back();
		}
		
	}
}

vector<Point> RobotController::findShortestPath(Point origin, vector<Point> destinations) {
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

bool RobotController::isCollisionImminent(Point next_position)
{
	if (this->getRobotAt(next_position))
		return true;
	else
		return false;
}

vector<Point> RobotController::convertGridPathToScreenPath(vector<Point> path)
{
	vector<Point> screen_path;
	for (auto point : path) 
	{
		screen_path.push_back(grid->getPositionOf(point));
	}
	return screen_path;
}

Robot * RobotController::getRobotAt(Point grid_position)
{
	for (auto robot : robots)
	{
		if (robot->grid_position == grid_position)
			return robot;
	}
	return nullptr;
}

void RobotController::robotCompletedMovement(EventCustom* event)
{
	Robot* robot = static_cast<Robot*>(event->getUserData());
	this->definePathOf(robot);
	robot->move();
}
