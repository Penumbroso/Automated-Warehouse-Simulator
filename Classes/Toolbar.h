#include "cocos2d.h"

USING_NS_CC;

class Toolbar : public cocos2d::Layer
{
	public:
		virtual bool init();

		enum Tool {
			PACKAGE,
			BEGIN,
			END,
			ERASE,
			BLOCKADE
		};

		Tool selected = PACKAGE;

		MenuItemImage* runItem;
		MenuItemImage* endItem;
		MenuItemImage* packageItem;
		MenuItemImage* beginItem;
		MenuItemImage* resetItem;
		MenuItemImage* eraseItem;
		MenuItemImage* blockadeItem;

		CREATE_FUNC(Toolbar);
};