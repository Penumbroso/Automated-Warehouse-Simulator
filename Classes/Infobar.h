#include "cocos2d.h"

USING_NS_CC;

class Infobar : public cocos2d::Layer
{
public:
	virtual bool init();

	std::string* time;

	void updateSpeed(float speed);
	void updateZoom(float zoom);

	CREATE_FUNC(Infobar);

private:
	cocos2d::Label* speed_factor_label;
	cocos2d::Label* zoom_label;

	cocos2d::Label* time_label;

	void updateClock(float dt);
};