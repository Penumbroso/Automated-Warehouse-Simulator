#include "globals.h"

const int g_square_size = 30;

bool g_running = false;

Tool g_current_tool = PACKAGE;

std::vector<Point> g_packages;
std::vector<Point> g_start;
std::vector<Point> g_end;