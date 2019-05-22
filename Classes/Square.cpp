#include "Square.h"

using namespace cocos2d;

void Square::onEnter()
{
	MenuItemImage::onEnter();
	state = EMPTY;
}

