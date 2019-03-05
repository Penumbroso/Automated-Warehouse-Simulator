#ifndef __SIMULATOR_SCENE_H__
#define __SIMULATOR_SCENE_H__

#include "cocos2d.h"
#include "Grid.h"
#include "Robot.h"
#include "Toolbar.h"
#include <vector>
#include "Stopwatch.h"
#include "RobotController.h"

using std::vector;

class Simulator : public cocos2d::Scene
{
public:
    virtual bool init();

    CREATE_FUNC(Simulator);

private:
	vector<Robot*> robots;
	Grid* grid;
	Toolbar* toolbar;
	Stopwatch* stopwatch;
	RobotController * robotController;

	bool isRunning;
	vector<Point> packages_delivered;

	void run(float dt);
	void start();
	void stop();
	void reset();
	void createRobots();
	bool allPackagesWereDelivered();
	bool allRobotsAreParked();

	// Callbacks functions
	void menuToolCallback(Toolbar::Tool tool);
	void menuRunCallback(cocos2d::Ref * pSender);
	void menuResetCallback(cocos2d::Ref * pSender);
	void gridSquareCallback(Point coord);
};

#endif // __HELLOWORLD_SCENE_H__
