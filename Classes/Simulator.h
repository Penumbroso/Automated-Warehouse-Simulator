#ifndef __SIMULATOR_SCENE_H__
#define __SIMULATOR_SCENE_H__

#include "cocos2d.h"
#include "Grid.h"
#include "Robot.h"
#include "Toolbar.h"
#include <vector>
#include "AStar.hpp"
#include "Stopwatch.h"

using std::vector;

class Simulator : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(Simulator);

private:
	std::vector<Robot*> robots;
	Grid* grid;
	Toolbar* toolbar;
	Stopwatch* stopwatch;
	bool isRunning;
	vector<Point> packages_delivered;
	AStar::Generator path_generator;

	void run(float dt);
	void start();
	void stop();
	void load();
	bool isCollisionImminent(Point next_position);
	void createRobots();
	Robot* getRobotAt(Point grid_position);
	void definePathOf(Robot * robot);
	void preventCollisionOf(Robot* robot);
	vector<Point> findShortestPath(Point origin, vector<Point> destinations);

	// Callbacks
	void menuToolCallback(Toolbar::Tool tool);
	void menuRunCallback(cocos2d::Ref * pSender);
	void menuResetCallback(cocos2d::Ref * pSender);
	void gridSquareCallback(Point coord);
};

#endif // __HELLOWORLD_SCENE_H__
