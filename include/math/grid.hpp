#include <list>
#include <utility>

#include "types.hpp"

namespace mm {
namespace grid {

std::tuple<double, double> line_parameters(Point, Point);
std::list<Cell> crossing_line_cells(Point, Point);

} // namespace grid
} // namespace mm
