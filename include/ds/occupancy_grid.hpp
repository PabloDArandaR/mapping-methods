#include <cassert>
#include <cstdint>
#include <tuple>
#include <vector>

#include "types.hpp"

#ifndef OCCUPANCY_GRID_GUARD
#define OCCUPANCY_GRID_GUARD

namespace mm {

class occupancyGrid {
  public:
    occupancyGrid();
    occupancyGrid(int width, int height, int initial_value, double resolution);
    occupancyGrid(std::vector<int8_t>* new_map, int width, int height, int initial_value, double resolution);

    std::vector<int8_t>* get_map();
    double get_resolution();
    int get_width();
    int get_height();
    int get_probability(int& x, int& y);

    void set_probability(int& row, int& column, int& new_probability);
    void set_map_by_reference(std::vector<int8_t>*);

    inline std::tuple<int, int> calculate_cell_x_y(double& x, double& y);

    bool is_valid_cell(Cell check_cell);

    void store_map(std::string filename);
    int return_index(int row, int column);

  private:
    int width, height;  // cells
    double resolution;  // m/cells
    std::vector<int8_t>* map;
};

} // namespace mm

#endif
