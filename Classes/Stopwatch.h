#include "cocos2d.h"
#include <string>

class Stopwatch : public cocos2d::Node
{
	
public:
	virtual bool init();
	void start();
	void stop();

	int getMinutes();
	int getSeconds();
	int getMilliseconds();
	void reset();

	std::string text;
	std::string toString();
	void setSpeedMultiplier(float multiplier);

	CREATE_FUNC(Stopwatch);

private:
	int minutes;
	int seconds;
	int milliseconds;

	float speed_multiplier;


	void count(float dt);
};