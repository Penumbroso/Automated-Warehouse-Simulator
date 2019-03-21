#include "cocos2d.h"

USING_NS_CC;

class Actionbar : public cocos2d::Layer
{
public:
	virtual bool init();

	std::vector<MenuItemImage*> buttons;

	// TODO: add button to accelerato or desaccelerate simulation
	MenuItemImage* runItem;
	MenuItemImage* exportItem;
	MenuItemImage* resetItem;

	CREATE_FUNC(Actionbar);
};