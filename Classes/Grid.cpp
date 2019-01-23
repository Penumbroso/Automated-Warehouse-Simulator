#include "Grid.h"
#include "Globals.h"

USING_NS_CC;

bool Grid::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	const auto numberOfLines = visibleSize.height / g_square_size;
	const auto numberOfColumns = visibleSize.width / g_square_size;


	for (int j = 0; j < numberOfColumns; j++) {
		for (int i = 0; i < numberOfLines; i++) {
			auto square = Square::create();
			square->initWithFile("square.png");

			this->squares[j][i] = square;
			square->gridLocation = Point(j, i);

			square->setPosition(Vec2(j * g_square_size + g_square_size / 2,  i * g_square_size + g_square_size / 2));

			this->addChild(square, 0);
		}
	}

	Color4F lightGray = Color4F(0.9f, 0.9f, 0.9f, 1);

	for (int i = 0; i < numberOfColumns; i++) {
		auto drawVerticalLine = DrawNode::create();
		drawVerticalLine->drawLine(Point(i * g_square_size, 0), Point(i * g_square_size, visibleSize.height), lightGray);
		addChild(drawVerticalLine);
	}

	for (int i = 0; i < numberOfLines; i++) {
		auto drawHorizontalLine = DrawNode::create();
		drawHorizontalLine->drawLine(Point(0, i * g_square_size), Point(visibleSize.width, i * g_square_size), lightGray);
		addChild(drawHorizontalLine);
	}

	return true;

}