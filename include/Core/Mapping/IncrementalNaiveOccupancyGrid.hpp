#pragma once

#include <Core/Mapping/NaiveOccupancyGrid.hpp>
#include <cstddef>
#include <functional>
#include <optional>
#include <unordered_map>

class IncrementalNaiveOccupancyGrid : public Map<NaiveOccupancyGridCell> {
  private:
    struct TileIndex {
        int m_x;
        int m_y;

        TileIndex(int x, int y) {
            m_x = x;
            m_y = y;
        }

        bool operator==(const TileIndex &other) const { return m_x == other.m_x && m_y == other.m_y; };
    };

    struct TileIndexHash {
        std::size_t operator()(const TileIndex &idx) const {
            auto h1 = std::hash<int>{}(idx.m_x);
            auto h2 = std::hash<int>{}(idx.m_y);
            return h1 ^ (h2 * 2654435761u + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };

  public:
    explicit IncrementalNaiveOccupancyGrid(size_t width, size_t height, float resolution, Eigen::Isometry3d origin = Eigen::Isometry3d::Identity());
    IncrementalNaiveOccupancyGrid(const IncrementalNaiveOccupancyGrid &other);
    IncrementalNaiveOccupancyGrid &operator=(const IncrementalNaiveOccupancyGrid &other);
    ~IncrementalNaiveOccupancyGrid();

    void setValue(size_t x, size_t y, uint8_t value);
    std::optional<uint8_t> value(size_t x, size_t y) const;

    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    float resolution() const { return m_resolution; }
    Eigen::Isometry3d origin() const { return m_origin; }

    /// Get center of a particular grid cell
    Eigen::Isometry3d toWorld(const NaiveOccupancyGridCell &coord) override;
    /// Get cell where the given coordinate is inside
    std::optional<NaiveOccupancyGridCell> fromWorld(const Eigen::Isometry3d &coord) override;

    void forEachTile(const std::function<void(const NaiveOccupancyGrid &)> &callback) const;
    void forEachTileIndexed(const std::function<void(int tileX, int tileY, const NaiveOccupancyGrid &)> &callback) const;
    void setTileValue(int tileX, int tileY, size_t localX, size_t localY, uint8_t value);

  private:
    std::unordered_map<TileIndex, NaiveOccupancyGrid, TileIndexHash> m_tiles;
    /// Width of each tile, in cells
    size_t m_width;
    /// Height of each tile, in cells
    size_t m_height;
    /// Resolution of the map cells, in cells/meter
    float m_resolution;
    /// Transform to the origin of the map
    Eigen::Isometry3d m_origin;
};
