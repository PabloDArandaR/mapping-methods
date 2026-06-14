find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

list(APPEND HEADERS include/ROS/Mapping/MapPublisher.hpp
     include/ROS/Mapping/NaiveOccupancyGridPublisher.hpp
     include/ROS/Mapping/Publish.hpp include/ROS/Mapping/ROS2Publisher.hpp)

target_link_libraries(mapping rclcpp)
target_link_libraries(mapping std_msgs)
target_link_libraries(mapping geometry_msgs)
