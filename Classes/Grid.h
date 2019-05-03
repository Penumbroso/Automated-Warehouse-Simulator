#ifndef __GRID_H__
#define __GRID_H__

#include "cocos2d.h"
#include "Square.h"
#include "Util.h"

USING_NS_CC;

class Grid : public cocos2d::Layer
{
public:
	virtual bool init();

	std::map<Point, Square*> squares;
	std::map<Point, Sprite*> symbols;

	Menu * menu;

	const int square_size = 30;

	float number_of_columns;
	float number_of_lines;

	Point getPositionOf(Point point);
	Point getGridPositionOf(Point screen_position);

	void setState(Square::State state, Point point);

	vector<Point> starts;
	vector<Point> delivery_points;
	vector<Point> packages;
	vector<Point> blockades;

	vector<Point> available_packages;
	vector<Point> static_collidables;

	void toggleDragAndDrop();
	
	CREATE_FUNC(Grid);

protected:
	void drawLines();
	void addSymbol(const std::string &filename, Point point);
	void removeSymbol(Point point);

private:
	void createSquares();
	bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event);


	Point initial_touch_location;
};

#endif