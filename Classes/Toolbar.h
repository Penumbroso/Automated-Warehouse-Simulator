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
			BLOCKADE,
			CLOCK,
			PATH
		};

		Tool selected = PACKAGE;

		std::vector<MenuItemImage*> buttons;

		// TODO: separate between tool buttons and action buttons

		// Actions
		MenuItemImage* runItem;
		MenuItemImage* exportItem;
		MenuItemImage* resetItem;

		// Tools
		MenuItemImage* endItem;
		MenuItemImage* packageItem;
		MenuItemImage* beginItem;

		MenuItemImage* eraseItem;
		MenuItemImage* blockadeItem;
		MenuItemImage* pathItem;
		MenuItemImage* clockItem;

		CREATE_FUNC(Toolbar);
};