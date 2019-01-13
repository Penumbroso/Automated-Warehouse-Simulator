#include "Astar.h"


int Astar::helper()
{
	return 10;
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

