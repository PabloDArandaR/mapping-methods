cmake_minimum_required(VERSION 3.14)

# Basic examples — link only the core library
foreach(example IN ITEMS example1 example2 recorded_data test_parser)
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} PRIVATE mapping_methods)
endforeach()

# Visualisation example — requires GLFW (optional)
if(TARGET glfw AND TARGET OpenGL::GL)
    add_executable(line_GUI grid_line_crossing.cpp)
    target_link_libraries(line_GUI PRIVATE mapping_methods glfw OpenGL::GL argparse spdlog::spdlog)
endif()
