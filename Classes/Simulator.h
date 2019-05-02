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
#include "Stopwatch.h"

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
	std::map<Robot*, String> robot_times;
	std::map<PhysicsBody*, Robot*> robots_bodies;

	float speed_factor;

	void start();
	void stop();
	void proceed();
	void reset();
	void createRobots();
	bool allPackagesWereDelivered();
	bool allRobotsAreParked();
	void setCallbacks();
	void createCustomEvents();

	// Callbacks functions
	void menuRunCallback(cocos2d::Ref * pSender);
	void menuResetCallback(cocos2d::Ref * pSender);
	void gridSquareCallback(Point coord);
	void menuExportCallback(cocos2d::Ref * pSender);
	void menuChangeSpeedCallback(float multiplier);
	void menuMoveGridCallback(cocos2d::Ref* pSender);
	void menuZoomCallback(float multiplier);
	bool onContactBegin(PhysicsContact& contact);

	// Event functions
	void robotIsAtDelivery(EventCustom* event);
	void robotIsAtPackage(EventCustom* event);
	void robotIsParked(EventCustom* event);
};

#endif // __HELLOWORLD_SCENE_H__
