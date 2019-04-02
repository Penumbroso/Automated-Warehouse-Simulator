#include "Grid.h"

USING_NS_CC;

bool Grid::init()
{
	if (!Layer::init())
		return false;
	
	menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	number_of_lines = visibleSize.height / square_size + 1;
	number_of_columns = visibleSize.width / square_size + 1;

	this->createSquares();
	this->drawLines();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Grid::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Grid::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

Point Grid::getPositionOf(Point point)
{
	return squares.at(point)->getPosition();
}

void Grid::setState(Square::State state, Point point)
{
	auto square = squares.at(point);
	square->setColor(Color3B::GRAY);
	square->state = state;

	switch (state)
	{
	case Square::BEGIN:
		this->addSymbol("Plus.png", point);
		Util::addIfUnique<Point>(&starts, point);
		break;

	case Square::END:
		this->addSymbol("Minus.png", point);
		Util::addIfUnique<Point>(&delivery_points, point);
		break;

	case Square::PACKAGE:
		Util::addIfUnique<Point>(&available_packages, point);
		Util::addIfUnique<Point>(&packages, point);
		Util::addIfUnique<Point>(&static_collidables, point);
		break;

	case Square::BLOCKADE:
		Util::addIfUnique<Point>(&static_collidables, point);
		Util::addIfUnique<Point>(&blockades, point);
		break;

	case Square::EMPTY:
		this->removeSymbol(point);
		square->setColor(Color3B::WHITE);
		Util::removeIfContains(&starts, point);
		Util::removeIfContains(&delivery_points, point);
		Util::removeIfContains(&available_packages, point);
		Util::removeIfContains(&static_collidables, point);
		break;
	}

}

void Grid::toggleDragAndDrop()
{
	menu->setEnabled(!menu->isEnabled());
}

void Grid::drawLines()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Color4F lightGray = Color4F(0.95f, 0.95f, 0.95f, 1);

	for (int i = 0; i < number_of_columns; i++) {
		auto drawVerticalLine = DrawNode::create();
		drawVerticalLine->drawLine(Point(i * square_size, 0), Point(i * square_size, visibleSize.height), lightGray);
		addChild(drawVerticalLine);
	}

	for (int i = 0; i < number_of_lines; i++) {
		auto drawHorizontalLine = DrawNode::create();
		drawHorizontalLine->drawLine(Point(0, i * square_size), Point(visibleSize.width, i * square_size), lightGray);
		addChild(drawHorizontalLine);
	}
}

void Grid::addSymbol(const std::string &filename, Point point)
{
	Sprite* symbol = Sprite::create(filename);
	symbol->setContentSize(Size(square_size - 10, square_size - 10));
	symbol->setPosition(squares.at(point)->getPosition());
	this->symbols[point] = symbol;
	this->addChild(symbol);
}

void Grid::removeSymbol(Point point)
{
	if (this->symbols.count(point) > 0) {
		auto symbol = this->symbols.at(point);
		if(symbol)
			symbol->removeFromParentAndCleanup(true);

		this->symbols[point] = NULL;
	}
}

void Grid::createSquares()
{
	for (int j = 0; j < number_of_columns; j++) {
		for (int i = 0; i < number_of_lines; i++) {
			auto square = Square::create();
			square->setNormalImage(Sprite::create("square.png"));
			square->setContentSize(Size(square_size, square_size));

			squares[Point(j, i)] = square;
			square->grid_coord = Point(j, i);

			square->setPosition(Vec2(j * square_size + square_size / 2, i * square_size + square_size / 2));

			menu->addChild(square);
		}
	}
}

bool Grid::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	return true;
}


void Grid::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
	this->setPosition(this->getPosition() + touch->getDelta());
}

