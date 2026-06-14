#include <Core/Mapping/NaiveOccupancyGrid.hpp>
#include <Eigen/Geometry>
#include <types.hpp>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigen>

NaiveOccupancyGrid::NaiveOccupancyGrid(size_t width, size_t height, float resolution, Eigen::Isometry3d origin)
    : m_width(width), m_height(height), m_resolution(resolution), m_origin(origin), m_data(Eigen::MatrixXi::Zero(width, height)) {};

NaiveOccupancyGrid::NaiveOccupancyGrid(const NaiveOccupancyGrid &other)
    : m_height(other.m_height), m_width(other.m_width), m_resolution(other.m_resolution), m_origin(other.m_origin),
      m_data(other.m_data) {

      };
NaiveOccupancyGrid &NaiveOccupancyGrid::operator=(const NaiveOccupancyGrid &other) {
    m_width = other.m_width;
    m_data = other.m_data;
    m_height = other.m_height;
    m_resolution = other.m_resolution;
    m_origin = other.m_origin;
    return *this;
};

NaiveOccupancyGrid::~NaiveOccupancyGrid() = default;

void NaiveOccupancyGrid::setValue(size_t x, size_t y, uint8_t value) {
    assert(value < 100);
    assert(x < m_width);
    assert(y < m_height);
    m_data(x, y) = value;
};
uint8_t NaiveOccupancyGrid::value(size_t x, size_t y) const { return m_data(x, y); }

Eigen::Isometry3d NaiveOccupancyGrid::toWorld(const NaiveOccupancyGridCell &coord) {
    return m_origin * Eigen::Translation3d(coord.m_x / m_resolution, coord.m_y / m_resolution, 0.0f);
};

std::optional<NaiveOccupancyGridCell> NaiveOccupancyGrid::fromWorld(const Eigen::Isometry3d &coord) {
    auto diff = (coord.matrix() - m_origin.matrix()).block<3, 1>(0, 3);

    return NaiveOccupancyGridCell(diff(0) * m_resolution, diff(1) * m_resolution);
};
