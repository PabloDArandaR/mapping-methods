#pragma once

#include "sensor/measurement.hpp"
#include <vector>

namespace mm {
namespace sensor {

/// Static configuration of a lidar sensor (fixed per sensor setup).
struct LidarMetadata {
    double angle_min{0.0};        ///< Minimum scan angle [rad]
    double angle_max{0.0};        ///< Maximum scan angle [rad]
    double angle_increment{0.0};  ///< Angular step between beams [rad]
    double range_min{0.0};        ///< Minimum valid range [m]
    double range_max{0.0};        ///< Maximum valid range [m]
};

/// A single lidar scan measurement.
class LidarMeasurement : public Measurement {
  public:
    LidarMetadata metadata;      ///< Sensor configuration for this scan
    std::vector<double> ranges;  ///< Range readings [m], one per beam
};

} // namespace sensor
} // namespace mm
