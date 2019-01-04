#include "Grid.h"
#include "Gap.h"

USING_NS_CC;

bool Grid::init()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	const auto numberOfLines = visibleSize.height / 50;
	const auto numberOfColumns = visibleSize.width / 50;

	// This need to be fixed to the correct size
	Gap* gaps[100][100];

	for (int j = 0; j < numberOfColumns; j++) {
		for (int i = 0; i < numberOfLines; i++) {

			// Add single square that will later form a grid
			auto square = Gap::create();
			square->initWithFile("square.png");

			gaps[j][i] = square;
			square->gridLocation = Point(j, i);

			// Set the position of this square from the left corner to the right corner
			square->setPosition(Vec2(j * 50 + 25,  i * 50 + 25));

			// Add the square sprite as a child of this layer
			this->addChild(square, 0);
		}
	}

	// Draw vertical lines
	for (int i = 0; i < numberOfColumns; i++) {
		auto draw = DrawNode::create();
		draw->drawLine(Point(i * 50, 0), Point(i * 50, visibleSize.height), Color4F::GRAY);
		addChild(draw);
	}

	// Draw horizontal lines
	for (int i = 0; i < numberOfLines; i++) {
		auto draw = DrawNode::create();
		draw->drawLine(Point(0, i * 50), Point(visibleSize.width, i * 50), Color4F::GRAY);
		addChild(draw);
	}

	return true;

}