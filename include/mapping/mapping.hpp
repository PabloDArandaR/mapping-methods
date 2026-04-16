#pragma once

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

#include "ds/occupancy_grid.hpp"
#include "sensor/measurement.hpp"

namespace mm {

class mapping {
  public:
    mapping();

    /// Update the map given the robot's current pose in the world frame and a sensor measurement.
    /// @param pose   Robot pose as a rigid transform (rotation + translation).
    /// @param measurement  Sensor measurement; concrete algorithms cast to their expected subtype.
    virtual void update(const Eigen::Isometry3d& pose, const sensor::Measurement& measurement);

    occupancyGrid* get_map();
    void set_map(occupancyGrid* new_map);

  protected:
    occupancyGrid* map;
};

} // namespace mm
