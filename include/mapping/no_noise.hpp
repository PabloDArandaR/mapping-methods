#pragma once

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
#include <list>
#include <vector>

#include "mapping/mapping.hpp"
#include "sensor/lidar_measurement.hpp"

namespace mm {

class noNoiseMethod : public mapping {
  public:
    noNoiseMethod();
    noNoiseMethod(occupancyGrid* map);

    /// Expects measurement to be a LidarMeasurement.
    void update(const Eigen::Isometry3d& pose, const sensor::Measurement& measurement) override;

    void fill_cells(std::list<Cell>& cell_list, double range_max, double range_val);
};

} // namespace mm
