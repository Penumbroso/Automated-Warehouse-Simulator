#ifndef __SIMULATOR_SCENE_H__
#define __SIMULATOR_SCENE_H__

#include "cocos2d.h"

class Simulator : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	enum State
	{
		EDITING,
		PLAYING,
		PAUSED
	};

	State state = EDITING;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuPlayCallback(cocos2d::Ref* pSender);
    
	void something(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(Simulator);
};

#endif // __HELLOWORLD_SCENE_H__
