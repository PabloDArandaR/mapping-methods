#pragma once

#include <chrono>
#include <string>

namespace mm {
namespace sensor {

/// Base class for all sensor measurements.
/// All sensor-specific measurement types must derive from this class.
class Measurement {
  public:
    virtual ~Measurement() = default;

    /// Timestamp of the measurement.
    std::chrono::time_point<std::chrono::steady_clock> timestamp{};

    /// Name of the sensor frame this measurement originates from (e.g. "lidar_link").
    std::string frame_id{};
};

} // namespace sensor
} // namespace mm
