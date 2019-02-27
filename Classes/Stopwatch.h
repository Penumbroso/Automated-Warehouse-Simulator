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

	cocos2d::Label* label;
	std::string toString();

	CREATE_FUNC(Stopwatch);

private:
	int minutes;
	int seconds;
	int milliseconds;

	void count(float dt);
};