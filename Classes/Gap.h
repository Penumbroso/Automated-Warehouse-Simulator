#include "cocos2d.h"

class Gap : public cocos2d::Sprite
{
public:
	//static Gap* createWithSpriteFrameName(const std::string& spriteFrameName);

	void onEnter() override;
	//void onExit() override;

	enum State 
	{
		EMPTY = 0,
		FILLED = 1,
		START = 2,
		END = 3
	};

	State state = EMPTY;

	cocos2d::Point  gridLocation;

	cocos2d::Rect getRect();
	bool containsTouchLocation(cocos2d::Touch* touch);
	bool onTouchBegan(cocos2d::Touch* t, cocos2d::Event* e);
	bool onTouchEnded(cocos2d::Touch* t, cocos2d::Event* e);

	CREATE_FUNC(Gap);
	cocos2d::EventListenerTouchOneByOne* getTouchListener() const { return _touchListener; }

private:
	cocos2d::EventListenerTouchOneByOne*    _touchListener;
};