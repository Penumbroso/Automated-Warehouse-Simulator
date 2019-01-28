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

	enum State
	{
		EDITING,
		RUNNING,
		PAUSED
	};

	State state = EDITING;

	std::vector<Robot*> robots;

	Grid* grid;
    
	// TODO: change name to run instead of play.
	void menuPlayCallback(cocos2d::Ref* pSender);
    
	void tick(float dt);

    CREATE_FUNC(Simulator);

protected:
	cocos2d::Node*  _trackNode;

	void createPath(Robot* r);
};

#endif // __HELLOWORLD_SCENE_H__
