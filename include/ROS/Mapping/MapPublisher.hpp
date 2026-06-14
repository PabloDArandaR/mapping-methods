#include <ROS/Mapping/ROS2Publisher.hpp>
#include <concepts>

template <typename MapType> class MapPublisher : public ROS2Publisher {
    bool publish() override;
};
