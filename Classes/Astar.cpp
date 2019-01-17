#include "Astar.h"
#include <vector>
#include <limits>

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

std::vector<Node> getValidAdjacents(Node current, std::vector<Node> obstacles) {
	std::vector<Node> validAdjacents;
	std::vector<Node> possibleAdjacents;

	Node up;
	up.x = current.x;
	up.y = current.y + 1;
	up.parentX = current.x;
	up.parentY = current.y;
	possibleAdjacents.push_back(up);

	Node down;
	down.x = current.x;
	down.y = current.y - 1;
	down.parentX = current.x;
	down.parentY = current.y;
	possibleAdjacents.push_back(down);

	Node left;
	left.x = current.x - 1;
	left.y = current.y;
	left.parentX = current.x;
	left.parentY = current.y;
	possibleAdjacents.push_back(left);

	Node right;
	right.x = current.x + 1;
	right.y = current.y;
	right.parentX = current.x;
	right.parentY = current.y;
	possibleAdjacents.push_back(right);

	for (Node adjacent : possibleAdjacents)
	{
		boolean valid = true;
		for (Node obstacle : obstacles)
		{
			if (obstacle.x == adjacent.x && obstacle.y == adjacent.y) {
				valid = false;
				break;
			}

		}
		if (valid) validAdjacents.push_back(adjacent);
	}

	return validAdjacents;
}

void Astar::Astar(cocos2d::Point start, cocos2d::Point destination, std::vector<cocos2d::Point> obstacles)
{
	CCLOG("A* Algorithm");

	// I need to know the total size of the grid? Probrably for bounderies

	std::vector<Node> closed;
	std::vector<Node> open;

	Node startNode;
	startNode.x = start.x;
	startNode.y = start.y;

	Node destinationNode;
	destinationNode.x = destination.x;
	destinationNode.y = destination.y;

	std::vector<Node> obstaclesNodes;
	
	for (cocos2d::Point obstacle : obstacles) 
	{
		Node obstacleNode;
		obstacleNode.x = obstacle.x;
		obstacleNode.y = obstacle.y;
		obstaclesNodes.push_back(obstacleNode);
	}

	// First step: add start node to the closed list, define him as current Node
	closed.push_back(startNode);
	Node currentParent = startNode;
	// Second step: add all adjecent nodes of the lowest value from the closed list to the open list
	// Say to all the adjacent nodes who their parent is
	// Using the manhatan euristic, calulete the value of H which correspond to the distance of the current node from the open list
	// to the destination, ignore obstacles, 10 points for each step

	std::vector<Node> validAdjacents = getValidAdjacents(currentParent, obstaclesNodes);
	//for (Node adjacent : validAdjacents) {
	//	open.push_back(adjacent);
	//	adjacent.parent = currentParent;
	//	adjacent.hCost = calculateManhattanDistance(adjacent, destinationNode);
	//}
	//
	//int lowestH = std::numeric_limits<int>::max();
	//Node lowestHNode = NULL;
	//for (Node nodeInOpenList : open) {
	//	if (nodeInOpenList.hCost < lowestH)
	//		lowestHNode = nodeInOpenList;
	//}
	
	// Calculate the G value, which is the cost from the current Node to the adjecent node ( probraby gonna ignore this step)
	// Calculate F score, which is F = H + G
	// Check all the values on the open list for which one has the lowest F score (Back to the loop)

}



int calculateManhattanDistance(Node origin, Node destination) {
	// TODO
	return 0;
}