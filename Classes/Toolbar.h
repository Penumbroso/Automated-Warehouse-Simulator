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
			PATH,
			MOVE
		};

		Tool selected = PACKAGE;

		std::vector<MenuItemImage*> buttons;

		MenuItemImage* endItem;
		MenuItemImage* packageItem;
		MenuItemImage* beginItem;
		MenuItemImage* eraseItem;
		MenuItemImage* blockadeItem;
		MenuItemImage* pathItem;
		MenuItemImage* clockItem;

		void setTool(Tool tool, MenuItemImage* btn);

		CREATE_FUNC(Toolbar);
	private:
		DrawNode* selected_bg;

		void setCallbacks();
		void createButtons();
};