#include <memory>
#include <rclcpp/rclcpp.h>

template <typename InternalRepresentation, typename ROSType> class ROS2Publisher {
  public:
    virtual bool publish() = 0;

  private:
    std::shared_ptr<InternalRepresentation> m_data;
};
