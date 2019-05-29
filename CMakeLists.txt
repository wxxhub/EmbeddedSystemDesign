cmake_minimum_required(VERSION 3.5)
project(embedded_design)

find_package(OpenCV REQUIRED)

include_directories(
    vision
    port
)

aux_source_directory(vision/src DIR_LIB_SRCS)
aux_source_directory(port/src DIR_LIB_SRCS)

add_library(${PROJECT_NAME}
    ${DIR_LIB_SRCS}
)

add_executable(demo demo/demo.cpp)
target_link_libraries(demo
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
)

add_executable(port_demo demo/port_demo.cpp)
target_link_libraries(port_demo
    ${PROJECT_NAME}
    boost_system
    boost_thread
)