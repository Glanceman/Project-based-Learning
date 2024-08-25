cmake_minimum_required(VERSION 3.13.0)


project(net-common VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


message("${CMAKE_PROJECT_NAME} CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

add_library(net-common INTERFACE)

target_include_directories(net-common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
)


target_compile_features(net-common INTERFACE cxx_std_20)
