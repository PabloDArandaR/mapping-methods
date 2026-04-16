#include <cmath>
#include <iostream>
#include <list>

#include "ds/occupancy_grid.hpp"
#include "mapping/no_noise.hpp"
#include "math/grid.hpp"
#include "sensor/lidar_measurement.hpp"
#include "types.hpp"

namespace mm {

noNoiseMethod::noNoiseMethod(){};

noNoiseMethod::noNoiseMethod(occupancyGrid* map) { this->map = map; };

void noNoiseMethod::update(const Eigen::Isometry3d& pose, const sensor::Measurement& measurement) {
    const auto& lidar = static_cast<const sensor::LidarMeasurement&>(measurement);

    const Eigen::Matrix3d rotation = pose.rotation();
    const Eigen::Vector3d translation = pose.translation();

    double angle{lidar.metadata.angle_min};

    for (double val : lidar.ranges) {
        // Compute ray endpoints in sensor frame, then transform to map frame
        Eigen::Vector3d r_initial_sensor(lidar.metadata.range_min * std::cos(angle),
                                        lidar.metadata.range_min * std::sin(angle), 0.0);
        Eigen::Vector3d r_final_sensor(val * std::cos(angle), val * std::sin(angle), 0.0);

        Eigen::Vector3d r_initial = rotation * r_initial_sensor + translation;
        Eigen::Vector3d r_final   = rotation * r_final_sensor   + translation;

        Point initial_grid_point(r_initial[0] / this->map->get_resolution(),
                                 r_initial[1] / this->map->get_resolution());
        Point final_grid_point(r_final[0] / this->map->get_resolution(),
                               r_final[1] / this->map->get_resolution());

        std::list<Cell> crossing_cells(grid::crossing_line_cells(initial_grid_point, final_grid_point));
        this->fill_cells(crossing_cells, lidar.metadata.range_max, val);

        angle += lidar.metadata.angle_increment;
    }
}

void noNoiseMethod::fill_cells(std::list<Cell>& cell_list, double range_max, double range_val) {
    while (cell_list.size() != 1) {
        Cell checking_cell = cell_list.front();
        if (this->map->is_valid_cell(checking_cell)) {
            int new_probability = 0;
            try {
                this->map->set_probability(checking_cell.y, checking_cell.x, new_probability);
            } catch (int) {
            }
        }
        cell_list.pop_front();
    }

    Cell last_cell = cell_list.front();
    if (this->map->is_valid_cell(last_cell)) {
        int new_probability = (range_val == range_max) ? 0 : 100;
        this->map->set_probability(last_cell.y, last_cell.x, new_probability);
    }
}

} // namespace mm
