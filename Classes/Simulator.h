#ifndef __SIMULATOR_SCENE_H__
#define __SIMULATOR_SCENE_H__

#include "cocos2d.h"
#include "Grid.h"
#include "Robot.h"
#include <vector>

class Simulator : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	std::vector<Robot*> robots;

	Grid* grid;
    
	void tick(float dt);

    CREATE_FUNC(Simulator);

protected:
	void load();
	void save();
	void createPath(Robot* r);
	void createRobots();
};

#endif // __HELLOWORLD_SCENE_H__
