#pragma once

#include <Core/Mapping/Map.hpp>
#include <cassert>
#include <cstdint>

#include <eigen3/Eigen/Eigen>

#include <types.hpp>

struct NaiveOccupancyGridCell {
    size_t m_x;
    size_t m_y;

    NaiveOccupancyGridCell(size_t x, size_t y) : m_x(x), m_y(y) {}
};

class NaiveOccupancyGrid : Map<NaiveOccupancyGridCell> {
  public:
    explicit NaiveOccupancyGrid(size_t width, size_t height, float resolution, Eigen::Isometry3d origin = Eigen::Isometry3d::Identity());
    NaiveOccupancyGrid(const NaiveOccupancyGrid &other);
    NaiveOccupancyGrid &operator=(const NaiveOccupancyGrid &other);
    ~NaiveOccupancyGrid();

    void setValue(size_t x, size_t y, uint8_t value);
    uint8_t value(size_t x, size_t y) const;

    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    float resolution() const { return m_resolution; }
    Eigen::Isometry3d origin() const { return m_origin; }

    /// Get center of a particular grid cell
    Eigen::Isometry3d toWorld(const NaiveOccupancyGridCell &coord) override;
    /// Get cell where the given coordinate is inside
    std::optional<NaiveOccupancyGridCell> fromWorld(const Eigen::Isometry3d &coord) override;

  private:
    size_t m_width;
    size_t m_height;
    float m_resolution;
    Eigen::Isometry3d m_origin;
    Eigen::MatrixXi m_data;
};
