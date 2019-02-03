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
			ERASE
		};

		Tool current = PACKAGE;

		void setCallback(MenuItemImage* btn, ccMenuCallback callback);

		MenuItemImage* runItem;
		MenuItemImage* endItem;
		MenuItemImage* packageItem;
		MenuItemImage* beginItem;
		MenuItemImage* resetItem;
		MenuItemImage* eraseItem;

		CREATE_FUNC(Toolbar);
};