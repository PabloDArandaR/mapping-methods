cmake_minimum_required(VERSION 3.5)
project(mapping_methods)

include_directories(include)

find_package(Eigen3)
find_package(spdlog REQUIRED)
find_package(argparse REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(include)

# ── Core data-structure library ────────────────────────────────────────────

set(HEADERS
    include/Core/Mapping/Map.hpp include/Core/Mapping/NaiveOccupancyGrid.hpp
    include/Core/Mapping/IncrementalNaiveOccupancyGrid.hpp)

set(SOURCE src/Core/Mapping/NaiveOccupancyGrid.cpp
           src/Core/Mapping/IncrementalNaiveOccupancyGrid.cpp)

add_library(mapping ${HEADERS} ${SOURCE})
target_link_libraries(mapping Eigen3::Eigen)

# -- Include ROS

add_subdirectory(include/ROS)

# ── GLSL shader compilation ────────────────────────────────────────────────

find_program(GLSLC glslc HINTS $ENV{VULKAN_SDK}/bin)
if(GLSLC)
  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/occupancy_grid_vert.spv
    COMMAND ${GLSLC} ${CMAKE_SOURCE_DIR}/shaders/occupancy_grid.vert -o
            ${CMAKE_BINARY_DIR}/occupancy_grid_vert.spv
    DEPENDS ${CMAKE_SOURCE_DIR}/shaders/occupancy_grid.vert
    COMMENT "Compiling occupancy_grid.vert")
  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/occupancy_grid_frag.spv
    COMMAND ${GLSLC} ${CMAKE_SOURCE_DIR}/shaders/occupancy_grid.frag -o
            ${CMAKE_BINARY_DIR}/occupancy_grid_frag.spv
    DEPENDS ${CMAKE_SOURCE_DIR}/shaders/occupancy_grid.frag
    COMMENT "Compiling occupancy_grid.frag")
  add_custom_target(
    occupancy_shaders DEPENDS ${CMAKE_BINARY_DIR}/occupancy_grid_vert.spv
                              ${CMAKE_BINARY_DIR}/occupancy_grid_frag.spv)
else()
  message(
    WARNING
      "glslc not found — occupancy grid shaders will not be compiled automatically"
  )
endif()

# ── Utils library (Vulkan viewer) ──────────────────────────────────────────

add_library(
  utils
  include/utils/VulkanOccupancyGridViewer.hpp
  src/utils/VulkanOccupancyGridViewer.cpp
  include/utils/GridGenerator.hpp
  src/utils/GridGenerator.cpp
  include/utils/GridSerializer.hpp
  src/utils/GridSerializer.cpp)
target_compile_definitions(utils PRIVATE SHADER_DIR="${CMAKE_BINARY_DIR}")
target_link_libraries(utils mapping Vulkan::Vulkan glfw)
if(TARGET occupancy_shaders)
  add_dependencies(utils occupancy_shaders)
endif()

# ── Executable tools ───────────────────────────────────────────────────────

add_executable(occupancy_grid_viewer src/tools/occupancy_grid_viewer.cpp)
target_link_libraries(occupancy_grid_viewer utils spdlog::spdlog
                      argparse::argparse)
