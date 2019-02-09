#include "cocos2d.h"
#include <vector>

USING_NS_CC;

extern const int g_square_size;

// TODO: move globals to the simulator
extern std::vector<Point> s_start;
extern std::vector<Point> s_end;
extern std::vector<Point> s_packages;
extern std::vector<Point> s_collidables;


// TODO: make global colors