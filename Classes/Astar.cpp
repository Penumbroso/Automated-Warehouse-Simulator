#include "Astar.h"
#include <vector>

void Astar::Astar()
{
	CCLOG("A* Algorithm");
	std::vector<cocos2d::Point> closed;
	std::vector<cocos2d::Point> open;


}

struct Node
{
	int y;
	int x;
	int parentX;
	int parentY;
	float gCost;
	float hCost;
	float fCost;
};

