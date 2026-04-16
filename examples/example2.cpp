#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "ds/occupancy_grid.hpp"
#include "mapping/no_noise.hpp"
#include "sensor/lidar_measurement.hpp"

using namespace mm;

int main() {
    noNoiseMethod mapping_method;

    // Set occupancy grid
    occupancyGrid gridmap(3, 3, 50, 1.0);
    mapping_method.set_map(&gridmap);

    // Build a lidar measurement
    sensor::LidarMeasurement m;
    m.metadata.angle_min       = 1.1;
    m.metadata.angle_max       = 0.0;
    m.metadata.angle_increment = 1.1;
    m.metadata.range_min       = 0.1;
    m.metadata.range_max       = 15.0;
    m.ranges                   = {2.2};

    // Robot pose
    Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
    pose.translation() << 0.1, 0.1, 0.0;

    mapping_method.update(pose, m);
}
