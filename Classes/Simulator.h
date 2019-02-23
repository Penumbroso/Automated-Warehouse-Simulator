#ifndef __SIMULATOR_SCENE_H__
#define __SIMULATOR_SCENE_H__

#include "cocos2d.h"
#include "Grid.h"
#include "Robot.h"
#include "Toolbar.h"
#include <vector>
#include "AStar.hpp"

using std::vector;

class Simulator : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	std::vector<Robot*> robots;

	Grid* grid;
	Toolbar* toolbar;
    
	void run(float dt);
	void updateUI(float dt);

    CREATE_FUNC(Simulator);

protected:
	void load();
	void save();
	void menuToolCallback(Toolbar::Tool tool);
	void menuRunCallback(cocos2d::Ref * pSender);
	void menuResetCallback(cocos2d::Ref * pSender);
	void gridSquareCallback(Point coord);
	void createRobots();
	void definePathOf(Robot * robot);
	void preventCollisionOf(Robot * robot);
	vector<Point> findShortestPath(Point origin, vector<Point> destinations);

	vector<Point> saved_starts;
	vector<Point> saved_ends;
	vector<Point> saved_packages;
	vector<Point> saved_collidables;

private:
	bool running = false;
	bool saved = false;
	AStar::Generator generator;

	// TODO: move this to its own file as it can be used by other if added a template.
	bool isCollisionImminent(Point next_position);
	Robot* getRobotAt(Point grid_position);
};

#endif // __HELLOWORLD_SCENE_H__
