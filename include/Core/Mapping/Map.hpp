#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Geometry>

#include <optional>

template <typename TCoord> class Map {
    using InternalCoordinate = TCoord;

  public:
    virtual Eigen::Isometry3d toWorld(const TCoord &coord) = 0;
    virtual std::optional<TCoord> fromWorld(const Eigen::Isometry3d &coord) = 0;

  protected:
    Eigen::Isometry3d m_origin;
};
