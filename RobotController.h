#include "cocos2d.h"
#include "Robot.h"
#include "AStar.hpp"
#include "Grid.h"


class RobotController : public cocos2d::Node
{
public:

	AStar::Generator path_generator;
	std::vector<Robot*> robots;

	Grid* grid;

	// Path functions
	void definePathOf(Robot * robot);
	vector<Point> findShortestPath(Point origin, vector<Point> destinations);

	// Collision Avoidance functions
	void preventCollisionOf(Robot* robot);
	bool isCollisionImminent(Point next_position);
	Robot* getRobotAt(Point grid_position);

	CREATE_FUNC(RobotController);
};



