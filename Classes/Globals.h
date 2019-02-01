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

extern std::vector<Point> g_start;
extern std::vector<Point> g_end;

// TODO: create a second list of packages that contains only orphan packages (without robots).
// I think this new vector should be initially empty and only cloned at the beggining of the simulation as it is only relevant there.
extern std::vector<Point> g_packages;

extern Tool g_current_tool;

extern bool g_running;