#include "cocos2d.h"

USING_NS_CC;

class Infobar : public cocos2d::Layer
{
public:
	virtual bool init();

	cocos2d::Label* label;
	std::string* time;

	CREATE_FUNC(Infobar);

private:
	void updateClock(float dt);
};