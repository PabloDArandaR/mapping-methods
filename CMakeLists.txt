cmake_minimum_required(VERSION 3.5)
project(mapping_methods)

add_subdirectory(/home/pablo/Software/glfw /home/pablo/Software/glfw/src)

include_directories(include)

find_package(Eigen3)
find_package(spdlog REQUIRED)
find_package(argparse REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(HEADERS include/abstract/scanner_metadata.hpp
    include/ds/occupancy_grid.hpp
    include/graphics/glfw.hpp
    include/mapping/mapping.hpp include/mapping/no_noise.hpp include/mapping/bayes_filter.hpp
    include/math/grid.hpp include/math/linalg.hpp
    include/types.hpp
    include/toolset/parser/parser.hpp include/toolset/parser/freiburg.hpp
    )

add_executable(debug_case1 src/tools/example1.cpp
    src/ds/occupancy_grid.cpp
    src/mapping/mapping.cpp src/mapping/no_noise.cpp
    src/abstract/scanner_metadata.cpp
    src/math/grid.cpp src/math/linalg.cpp
    )


add_executable(debug_case2 src/tools/example2.cpp
    src/ds/occupancy_grid.cpp
    src/mapping/mapping.cpp src/mapping/no_noise.cpp
    src/abstract/scanner_metadata.cpp
    src/math/grid.cpp src/math/linalg.cpp
    )
                
add_executable(recorded src/tools/recorded_data.cpp
    src/mapping/no_noise.cpp src/ds/occupancy_grid.cpp src/abstract/scanner_metadata.cpp src/mapping/mapping.cpp
    src/math/linalg.cpp src/math/grid.cpp
    ${HEADERS})
                
add_executable(line_GUI src/tools/grid_line_crossing.cpp 
    src/math/linalg.cpp src/math/grid.cpp
    src/graphics/glfw.cpp
    ${HEADERS})

add_executable(freiburg_eval src/tools/evaluate_freiburg.cpp 
    src/math/linalg.cpp src/math/grid.cpp src/toolset/parser/freiburg.cpp
    ${HEADERS})
                
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/pablo/Software/glfw")

target_include_directories(debug_case1 PRIVATE include)

target_link_libraries(debug_case1 Eigen3::Eigen)
target_link_libraries(debug_case2 Eigen3::Eigen)
target_link_libraries(recorded Eigen3::Eigen)
target_link_libraries(line_GUI glfw OpenGL::GL argparse::argparse spdlog::spdlog)
target_link_libraries(freiburg_eval spdlog::spdlog)

message("${CMAKE_MODULE_PATH}")