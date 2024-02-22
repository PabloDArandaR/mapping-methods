#include "mapping/mapping.hpp"

mapping::mapping(){};

void mapping::update([[maybe_unused]] Eigen::Vector3d relative_position,
                                  [[maybe_unused]] Eigen::Vector4d relative_rotation,
                                  [[maybe_unused]] std::vector<float> laser_measurements){};

occupancyGrid* mapping::get_map() { return this->map; }

void mapping::set_map(occupancyGrid* new_map) { this->map = new_map; };