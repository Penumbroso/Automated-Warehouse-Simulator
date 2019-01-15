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

	virtual void onClickTrackNode(bool bClicked, const cocos2d::Vec2& touchPos);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
	void tick(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(Simulator);

protected:
	cocos2d::Node*  _trackNode;
};

#endif // __HELLOWORLD_SCENE_H__
