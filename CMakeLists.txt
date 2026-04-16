cmake_minimum_required(VERSION 3.14)
project(mapping_methods VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# --- argparse (header-only, used by visualisation example only) ---
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/morrisfranken/argparse.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(argparse)

# --- System packages ---
find_package(Eigen3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(OpenGL)
find_package(glfw3 3.3)

set(OpenGL_GL_PREFERENCE GLVND)

# --- Library sources ---
set(LIB_SOURCES
    src/ds/occupancy_grid.cpp
    src/mapping/mapping.cpp
    src/mapping/no_noise.cpp
    src/math/grid.cpp
    src/math/linalg.cpp
    src/toolset/parser/freiburg.cpp
)

if(glfw3_FOUND AND OpenGL_FOUND)
    list(APPEND LIB_SOURCES src/graphics/glfw.cpp)
endif()

# --- Library target ---
add_library(mapping_methods STATIC ${LIB_SOURCES})

target_include_directories(mapping_methods
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(mapping_methods
    PUBLIC
        Eigen3::Eigen
        spdlog::spdlog
)

if(glfw3_FOUND AND OpenGL_FOUND)
    target_link_libraries(mapping_methods PUBLIC glfw OpenGL::GL)
endif()

# --- Examples ---
add_subdirectory(examples)
