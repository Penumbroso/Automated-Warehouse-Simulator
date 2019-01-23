#include "cocos2d.h"

class Square : public cocos2d::Sprite
{
public:
	void onEnter() override;

	enum State 
	{
		EMPTY,
		FILLED,
		START,
		END
	};

	State state = EMPTY;

	cocos2d::Point gridLocation;

	cocos2d::Rect getRect();
	bool containsTouchLocation(cocos2d::Touch* touch);
	bool onTouchBegan(cocos2d::Touch* t, cocos2d::Event* e);
	bool onTouchEnded(cocos2d::Touch* t, cocos2d::Event* e);

	CREATE_FUNC(Square);
	cocos2d::EventListenerTouchOneByOne* getTouchListener() const { return _touchListener; }

private:
	cocos2d::EventListenerTouchOneByOne*    _touchListener;
};