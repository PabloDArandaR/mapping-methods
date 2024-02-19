cmake_minimum_required(VERSION 3.5)
project(mapping_methods)

add_subdirectory(/home/pablo/Software/glfw /home/pablo/Software/glfw/src)

include_directories(include)

find_package(OpenGL REQUIRED)
find_package(Eigen3)

find_package(glfw3 3.3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(HEADERS include/mapping/mapping.hpp include/mapping/no_noise.hpp include/mapping/occupancy_grid.hpp include/mapping/scanner_metadata.hpp
            include/math/grid.hpp include/math/linalg.hpp
            include/types.hpp)

add_executable(debug_case1 src/example1.cpp
                src/mapping/no_noise.cpp src/mapping/occupancy_grid.cpp src/mapping/scanner_metadata.cpp src/mapping/mapping.cpp
                src/math/linalg.cpp src/math/grid.cpp)

target_include_directories(debug_case1 PRIVATE include)

add_executable(debug_case2 src/example2.cpp
                src/mapping/no_noise.cpp src/mapping/occupancy_grid.cpp src/mapping/scanner_metadata.cpp src/mapping/mapping.cpp
                src/math/linalg.cpp src/math/grid.cpp
                ${HEADERS})

add_executable(recorded src/recorded_data.cpp
                src/mapping/no_noise.cpp src/mapping/occupancy_grid.cpp src/mapping/scanner_metadata.cpp src/mapping/mapping.cpp
                src/math/linalg.cpp src/math/grid.cpp
                ${HEADERS})

add_executable(line_GUI src/viewing/line_crossing.cpp 
                src/math/linalg.cpp src/math/grid.cpp
                ${HEADERS})

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/pablo/Software/glfw")

target_link_libraries(debug_case1 Eigen3::Eigen)
target_link_libraries(debug_case2 Eigen3::Eigen)
target_link_libraries(recorded Eigen3::Eigen)
target_link_libraries(line_GUI glfw OpenGL::GL)

message("${CMAKE_MODULE_PATH}")