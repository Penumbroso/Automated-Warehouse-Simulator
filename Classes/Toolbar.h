#include "cocos2d.h"

USING_NS_CC;

class Toolbar : public cocos2d::Layer
{
	public:
		virtual bool init();

		void menuPackageCallback(cocos2d::Ref* pSender);
		void menuBeginningCallback(cocos2d::Ref* pSender);
		void menuEndCallback(cocos2d::Ref* pSender);
		void menuEraseCallback(cocos2d::Ref* pSender);
		void menuRunCallback(cocos2d::Ref* pSender);

		CREATE_FUNC(Toolbar);
};