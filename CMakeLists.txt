cmake_minimum_required(VERSION 3.27)
project(ccwc)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)

find_package(cxxopts CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE cxxopts::cxxopts)