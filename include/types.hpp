#include <eigen3/Eigen/Dense>

#ifndef TYPES_GUARD
#define TYPES_GUARD

namespace mm {

struct Point {
    int x, y;

    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
};

struct Cell {
    int x, y;

    Cell() : x(0), y(0) {}
    Cell(int x, int y) : x(x), y(y) {}
};

} // namespace mm

#endif
