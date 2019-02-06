#ifndef __SIMULATOR_SCENE_H__
#define __SIMULATOR_SCENE_H__

#include "cocos2d.h"
#include "Grid.h"
#include "Robot.h"
#include "Toolbar.h"
#include "Globals.h"
#include <vector>

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

    CREATE_FUNC(Simulator);

protected:
	void load();
	void save();
	void menuToolCallback(Toolbar::Tool tool);
	void menuRunCallback(cocos2d::Ref * pSender);
	void menuResetCallback(cocos2d::Ref * pSender);
	void gridSquareCallback(Square* square);
	vector<Point> createPath(Point origin, Point package);
	void createRobots();
	Point getClosestPackageFrom(Point position);


	// TODO: create a second list of packages that contains only orphan packages (without robots).
	// I think this new vector should be initially empty and only cloned at the beggining of the simulation as it is only relevant there.
	vector<Point> starts;
	vector<Point> ends;
	vector<Point> packages;

	vector<Point> available_packages;

private:
	bool running = false;
	bool saved = false;
	AStar::Generator generator;
};

#endif // __HELLOWORLD_SCENE_H__
