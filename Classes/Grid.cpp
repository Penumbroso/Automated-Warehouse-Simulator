#include "Grid.h"
#include "Globals.h"

USING_NS_CC;

bool Grid::init()
{
	if (!Layer::init())
		return false;
	
	menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	number_of_lines = visibleSize.height / g_square_size + 1;
	number_of_columns = visibleSize.width / g_square_size + 1;

	for (int j = 0; j < number_of_columns; j++) {
		for (int i = 0; i < number_of_lines; i++) {
			auto square = Square::create();
			square->setNormalImage(Sprite::create("square.png"));
			square->setContentSize(Size(g_square_size, g_square_size));

			squares[Point(j, i)] = square;
			square->gridLocation = Point(j, i);

			square->setPosition(Vec2(j * g_square_size + g_square_size / 2,  i * g_square_size + g_square_size / 2));

			menu->addChild(square);
		}
	}

	this->drawLines();

	return true;
}

Point Grid::getPositionOf(Point point)
{
	return squares.at(point)->getPosition();
}

void Grid::setState(Square::State state, Point point)
{
	// TODO: add the icon sprite like plus and minus sign
	squares.at(point)->setColor(Color3B::GRAY);
	squares.at(point)->state = state;
}

void Grid::drawLines()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Color4F lightGray = Color4F(0.9f, 0.9f, 0.9f, 1);

	for (int i = 0; i < number_of_columns; i++) {
		auto drawVerticalLine = DrawNode::create();
		drawVerticalLine->drawLine(Point(i * g_square_size, 0), Point(i * g_square_size, visibleSize.height), lightGray);
		addChild(drawVerticalLine);
	}

	for (int i = 0; i < number_of_lines; i++) {
		auto drawHorizontalLine = DrawNode::create();
		drawHorizontalLine->drawLine(Point(0, i * g_square_size), Point(visibleSize.width, i * g_square_size), lightGray);
		addChild(drawHorizontalLine);
	}
}
