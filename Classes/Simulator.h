#ifndef __SIMULATOR_SCENE_H__
#define __SIMULATOR_SCENE_H__

#include "cocos2d.h"
#include "Grid.h"

class Simulator : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onEnter() override;

	enum State
	{
		EDITING,
		PLAYING,
		PAUSED
	};

	State state = EDITING;

	Grid* grid;
    
    // a selector callback
	void menuPlayCallback(cocos2d::Ref* pSender);
    
	void tick(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(Simulator);
};

#endif // __HELLOWORLD_SCENE_H__
