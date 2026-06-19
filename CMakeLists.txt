cmake_minimum_required(VERSION 3.20)
project(Lab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare -Wshadow")

find_package(Eigen3 REQUIRED)

set(SRC_tc main.cpp)
add_executable(RUN ${SRC_tc})

target_link_libraries(RUN Eigen3::Eigen)
