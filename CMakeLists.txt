cmake_minimum_required(VERSION 3.5)
project(vision)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 3.3.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Threads)

add_executable(vision main.cpp vision/settings/VisionProperties.cpp vision/settings/VisionProperties.hpp vision/pipeline/GripPipeline.cpp vision/pipeline/GripPipeline.hpp vision/math/PositionMath.cpp vision/math/PositionMath.hpp)
target_link_libraries(vision ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
