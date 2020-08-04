cmake_minimum_required(VERSION 3.16)
project(mnist)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

add_executable(mnist main.cpp include/dataset.hpp include/network.hpp)
target_include_directories(mnist PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(mnist ${OpenCV_LIBS})
