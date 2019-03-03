#include "cocos2d.h"
#include "Robot.h"
#include "AStar.hpp"
#include "Grid.h"


class RobotController : public cocos2d::Node
{
public:
	virtual void onEnter();

	AStar::Generator path_generator;
	vector<Robot*> robots;
	Grid* grid;

	void definePathOf(Robot * robot);
	void preventCollisionOf(Robot* robot);

	CREATE_FUNC(RobotController);

private:
	vector<Point> findShortestPath(Point origin, vector<Point> destinations);
	bool isCollisionImminent(Point next_position);
	Robot* getRobotAt(Point grid_position);
};



