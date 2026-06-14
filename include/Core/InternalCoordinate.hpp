#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Geometry>

class InternalCoordinate {
    Eigen::Isometry3d toWorld();
    InternalCoordinate fromWorld(Eigen::Isometry3d wrl);
};
