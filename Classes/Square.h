#include "cocos2d.h"

class Square : public cocos2d::MenuItemImage
{
public:
	void onEnter() override;

	// TODO: make state a typedef of tool or vice versa
	enum State 
	{
		EMPTY,
		FILLED,
		START,
		END
	};

	State state = EMPTY;

	cocos2d::Point gridLocation;

	CREATE_FUNC(Square);

};