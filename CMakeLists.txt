cmake_minimum_required(VERSION 3.16.3)

project(WaterCylinder CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Headers)

find_package(Qt5 REQUIRED COMPONENTS Core Gui)

find_package(OpenMP)
# if(OpenMP_CXX_FOUND)
#     target_link_libraries(MyTarget PUBLIC OpenMP::OpenMP_CXX)
# endif()

enable_testing()
add_subdirectory(googletest)
add_subdirectory(Sources)
add_subdirectory(Test)