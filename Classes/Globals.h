#include "cocos2d.h"
#include <vector>

USING_NS_CC;

enum Tool {
	PACKAGE,
	BEGIN,
	END,
	ERASE
};

extern const int g_square_size;
extern Point g_start;
extern Point g_end;
extern std::vector<Point> g_packages;

extern Tool g_current_tool;