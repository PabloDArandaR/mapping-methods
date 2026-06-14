#include <Core/Mapping/IncrementalNaiveOccupancyGrid.hpp>
#include <types.hpp>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Geometry>

IncrementalNaiveOccupancyGrid::IncrementalNaiveOccupancyGrid(size_t width, size_t height, float resolution, Eigen::Isometry3d origin)
    : m_width(width), m_height(height), m_resolution(resolution), m_origin(origin) {

    m_tiles.emplace(TileIndex(0, 0), NaiveOccupancyGrid(m_width, m_height, m_resolution, m_origin));
    m_tiles.emplace(TileIndex(-1, 0),
                    NaiveOccupancyGrid(m_width, m_height, m_resolution, m_origin * Eigen::Translation3d(-m_width / m_resolution, 0.0f, 0.0f)));
    m_tiles.emplace(TileIndex(0, -1),
                    NaiveOccupancyGrid(m_width, m_height, m_resolution, m_origin * Eigen::Translation3d(0.0f, -m_height / m_resolution, 0.0f)));
    m_tiles.emplace(TileIndex(-1, -1), NaiveOccupancyGrid(m_width, m_height, m_resolution,
                                                          m_origin * Eigen::Translation3d(-m_width / m_resolution, -m_height / m_resolution, 0.0f)));
};

IncrementalNaiveOccupancyGrid::IncrementalNaiveOccupancyGrid(const IncrementalNaiveOccupancyGrid &other) = default;
IncrementalNaiveOccupancyGrid &IncrementalNaiveOccupancyGrid::operator=(const IncrementalNaiveOccupancyGrid &other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_resolution = other.m_resolution;
    m_origin = other.m_origin;
    m_tiles = other.m_tiles;
    return *this;
};

IncrementalNaiveOccupancyGrid::~IncrementalNaiveOccupancyGrid() = default;

void IncrementalNaiveOccupancyGrid::setValue(size_t x, size_t y, uint8_t value) {
    assert(value < 100);
    size_t xIndex = x / m_width;
    size_t yIndex = y / m_height;

    if (m_tiles.find(TileIndex(xIndex, yIndex)) != m_tiles.end())
        m_tiles.at(TileIndex(xIndex, yIndex)).setValue(x - m_width * xIndex, y - m_height * yIndex, value);
};

std::optional<uint8_t> IncrementalNaiveOccupancyGrid::value(size_t x, size_t y) const {
    size_t xIndex = x / m_width;
    size_t yIndex = y / m_height;
    auto it = m_tiles.find(TileIndex(xIndex, yIndex));
    if (it != m_tiles.end()) {
        auto extraX = x % m_width;
        auto extraY = y % m_height;
        return it->second.value(extraX, extraY);
    }
    return std::nullopt;
}

void IncrementalNaiveOccupancyGrid::forEachTile(const std::function<void(const NaiveOccupancyGrid &)> &callback) const {
    for (const auto &[idx, tile] : m_tiles)
        callback(tile);
}

void IncrementalNaiveOccupancyGrid::forEachTileIndexed(
    const std::function<void(int, int, const NaiveOccupancyGrid &)> &callback) const {
    for (const auto &[idx, tile] : m_tiles)
        callback(idx.m_x, idx.m_y, tile);
}

void IncrementalNaiveOccupancyGrid::setTileValue(int tileX, int tileY, size_t localX, size_t localY, uint8_t value) {
    auto it = m_tiles.find(TileIndex(tileX, tileY));
    if (it != m_tiles.end())
        it->second.setValue(localX, localY, value);
}

Eigen::Isometry3d IncrementalNaiveOccupancyGrid::toWorld(const NaiveOccupancyGridCell &coord) {
    return m_origin * Eigen::Translation3d(coord.m_x / m_resolution, coord.m_y / m_resolution, 0.0f);
};

std::optional<NaiveOccupancyGridCell> IncrementalNaiveOccupancyGrid::fromWorld(const Eigen::Isometry3d &coord) {

    auto diff = (coord.matrix() - m_origin.matrix()).block<3, 1>(0, 3);
    size_t xIndex = diff(0) / m_width;
    size_t yIndex = diff(1) / m_height;
    auto it = m_tiles.find(TileIndex(xIndex, yIndex));
    if (it != m_tiles.end()) {
        return it->second.fromWorld(coord);
    }

    return std::nullopt;
};
