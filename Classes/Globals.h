#include "cocos2d.h"
#include <vector>

USING_NS_CC;

enum Tool {
	PACKAGE,
	BEGIN,
	END
};
extern Point g_start;
extern Point g_end;
extern std::vector<Point> g_packages;

extern Tool g_currentTool;