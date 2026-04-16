#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "ds/occupancy_grid.hpp"
#include "mapping/no_noise.hpp"
#include "sensor/lidar_measurement.hpp"

using namespace mm;

int main() {
    noNoiseMethod mapping_method;

    // Lidar metadata (fixed for this dataset)
    sensor::LidarMetadata meta;
    meta.angle_min       = -1.0;
    meta.angle_max       =  1.0;
    meta.angle_increment = 2.0 / 150.0;
    meta.range_min       = 0.3;
    meta.range_max       = 15.0;

    // Set occupancy grid
    occupancyGrid gridmap(300, 300, 50, 0.1);
    mapping_method.set_map(&gridmap);

    // Parse recorded CSV data
    std::string data_line;
    std::ifstream data_file("../recording_4.csv");
    while (std::getline(data_file, data_line)) {
        std::istringstream iss(data_line);
        char comma;
        double posX, posY, posZ, q0, qX, qY, qZ;
        iss >> posX >> comma >> std::ws >> posY >> comma >> std::ws >> posZ >> comma >> std::ws;
        iss >> q0 >> comma >> std::ws >> qX >> comma >> std::ws >> qY >> comma >> std::ws >> qZ;

        sensor::LidarMeasurement m;
        m.metadata = meta;
        double value{0};
        int i{0};
        while (iss >> std::ws >> comma >> value) {
            m.ranges.push_back(value);
            ++i;
        }

        Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
        pose.translation() << posX, posY, posZ;
        Eigen::Quaterniond q(q0, qX, qY, qZ);
        pose.rotate(q.normalized());

        mapping_method.update(pose, m);
        mapping_method.get_map()->store_map("map.csv");
    }
}
