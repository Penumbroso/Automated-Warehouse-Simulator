#include "cocos2d.h"
#include "Robot.h"
#include "AStar.hpp"
#include "Grid.h"


class RobotController : public cocos2d::Node
{
public:
	virtual void onEnter();

	vector<Robot*> robots;
	Grid* grid;

	void definePathOf(Robot * robot);
	void preventCollisionOf(Robot* robot);
	void repath(Robot* r1, Robot* r2);
	Robot* getRobotAt(Point grid_position);

	void robotCompletedMovement(EventCustom* event);

	CREATE_FUNC(RobotController);

private:
	vector<Point> findShortestPath(Point origin, vector<Point> destinations);
	bool isCollisionImminent(Point next_position);
	AStar::Generator path_generator;
	vector<Point> convertGridPathToScreenPath(vector<Point> path);
	void setupGenerator();
};



