#pragma once

#include "mapping/mapping.hpp"

namespace mm {

class bayesFilterMethod : public mapping {
  public:
    void update(const Eigen::Isometry3d& pose, const sensor::Measurement& measurement) override;

  private:
};

} // namespace mm
