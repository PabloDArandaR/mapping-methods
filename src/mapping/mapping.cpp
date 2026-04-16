#include "mapping/mapping.hpp"

namespace mm {

mapping::mapping(){};

void mapping::update([[maybe_unused]] const Eigen::Isometry3d& pose,
                     [[maybe_unused]] const sensor::Measurement& measurement){};

occupancyGrid* mapping::get_map() { return this->map; }

void mapping::set_map(occupancyGrid* new_map) { this->map = new_map; };

} // namespace mm
