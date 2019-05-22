#include "cocos2d.h"

class Square : public cocos2d::MenuItemImage
{
public:
	void onEnter() override;

	enum State 
	{
		EMPTY,
		PACKAGE,
		BEGIN,
		END, 
		BLOCKADE
	};

	State state;

	cocos2d::Point grid_position;

	CREATE_FUNC(Square);

};