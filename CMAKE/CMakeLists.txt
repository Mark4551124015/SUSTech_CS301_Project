cmake_minimum_required(VERSION 3.20)

project("Project" C CXX ASM)

include(cmake/st-project_my.cmake)

add_executable(${PROJECT_NAME})
add_st_target_properties(${PROJECT_NAME})


set(CMAKE_EXE_LINKER_FLAGS
"-u _printf_float, -u _scanf_float"
)
