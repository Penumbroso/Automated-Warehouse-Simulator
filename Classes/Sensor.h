#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "cocos2d.h"

USING_NS_CC;

class Sensor : public cocos2d::Node
{
public:

	virtual bool init();

	void signal();
	void setOwner(cocos2d::Node* owner);

	CREATE_FUNC(Sensor);

private:
	cocos2d::Node * owner;
};

#endif