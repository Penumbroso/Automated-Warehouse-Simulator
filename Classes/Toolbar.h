#include "cocos2d.h"

USING_NS_CC;

class Toolbar : public cocos2d::Layer
{
	public:
		virtual bool init();

		void menuPlayCallback(Ref * pSender);

		CREATE_FUNC(Toolbar);
};