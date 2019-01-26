#include "cocos2d.h"

USING_NS_CC;

class Toolbar : public cocos2d::Layer
{
	public:
		static Toolbar* create(ccMenuCallback callbackFunc);
		virtual bool init(ccMenuCallback callbackFunc);

		void menuPackageCallback(cocos2d::Ref* pSender);
		void menuBeginningCallback(cocos2d::Ref* pSender);
		void menuEndCallback(cocos2d::Ref* pSender);
		void menuEraseCallback(cocos2d::Ref* pSender);
};