#ifndef __SIMULATOR_SCENE_H__
#define __SIMULATOR_SCENE_H__

#include "cocos2d.h"
#include "Grid.h"
#include "Robot.h"
#include "Toolbar.h"
#include <vector>
#include "RobotController.h"
#include "Infobar.h"
#include <fstream>
#include "Actionbar.h"

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
	Infobar* infobar;
	Actionbar* actionbar;
	Stopwatch* stopwatch;
	RobotController * robotController;

	bool isRunning;
	vector<Point> packages_delivered;
	std::map<Point, Stopwatch*> stopwatches;

	float speed_factor;

	void run(float dt);
	void start();
	void stop();
	void reset();
	void createRobots();
	bool allPackagesWereDelivered();
	bool allRobotsAreParked();
	void setCallbacks();

	// Callbacks functions
	void menuRunCallback(cocos2d::Ref * pSender);
	void menuResetCallback(cocos2d::Ref * pSender);
	void gridSquareCallback(Point coord);
	void menuExportCallback(cocos2d::Ref * pSender);
	void menuSpeedUpCallback(cocos2d::Ref* pSender);
	void menuSlowDownCallback(cocos2d::Ref* pSender);
};

#endif // __HELLOWORLD_SCENE_H__
