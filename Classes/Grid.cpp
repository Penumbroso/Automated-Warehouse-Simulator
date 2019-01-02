#include "Grid.h"

USING_NS_CC;

bool Grid::init()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto numberOfLines = visibleSize.height / 50;
	auto numberOfColumns = visibleSize.width / 50;

	for (int i = 0; i < numberOfColumns; i++) {
		
		auto draw = DrawNode::create();
		draw->drawLine(Point(i * 50, 0), Point(i * 50, visibleSize.height), Color4F::RED);
		addChild(draw);
	}

	for (int i = 0; i < numberOfLines; i++) {

		auto draw = DrawNode::create();
		draw->drawLine(Point(0, i * 50), Point(visibleSize.width, i * 50), Color4F::RED);
		addChild(draw);
	}

	//// Add single square that will later form a grid
	//auto square = Sprite::create("square.png");

	//// Set the position of this square from the left corner to the right corner
	//int half_of_square_width = 25;
	//square->setPosition(Vec2(half_of_square_width + i * 49, visibleSize.height - half_of_square_width + origin.y - j * 49));

	//// Add the square sprite as a child of this layer
	//this->addChild(square, 0);
	

	return true;

}