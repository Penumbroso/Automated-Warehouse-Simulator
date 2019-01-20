#include "Grid.h"

USING_NS_CC;

bool Grid::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	const auto numberOfLines = visibleSize.height / 50;
	const auto numberOfColumns = visibleSize.width / 50;


	for (int j = 0; j < numberOfColumns; j++) {
		for (int i = 0; i < numberOfLines; i++) {
			auto square = Square::create();
			square->initWithFile("square.png");

			this->squares[j][i] = square;
			square->gridLocation = Point(j, i);

			square->setPosition(Vec2(j * 50 + 25,  i * 50 + 25));

			this->addChild(square, 0);
		}
	}

	Color4F lightGray = Color4F(0.9f, 0.9f, 0.9f, 1);

	for (int i = 0; i < numberOfColumns; i++) {
		auto drawVerticalLine = DrawNode::create();
		drawVerticalLine->drawLine(Point(i * 50, 0), Point(i * 50, visibleSize.height), lightGray);
		addChild(drawVerticalLine);
	}

	for (int i = 0; i < numberOfLines; i++) {
		auto drawHorizontalLine = DrawNode::create();
		drawHorizontalLine->drawLine(Point(0, i * 50), Point(visibleSize.width, i * 50), lightGray);
		addChild(drawHorizontalLine);
	}

	return true;

}