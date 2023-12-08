cmake_minimum_required(VERSION 3.20)

project("Project" C CXX ASM)

include(cmake/st-project.cmake)
include(cmake/lvgl.cmake)

add_executable(${PROJECT_NAME})
add_st_target_properties(${PROJECT_NAME})
add_lvgl_target_properties(${PROJECT_NAME})