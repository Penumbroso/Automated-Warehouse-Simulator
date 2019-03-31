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
	MenuItemImage* speedUpItem;
	MenuItemImage* slowDownItem;
	MenuItemImage* moveItem;
	MenuItemImage* zoomInItem;
	MenuItemImage* zoomOutItem;

	CREATE_FUNC(Actionbar);
private:
	void createButtons();
};