cmake_minimum_required(VERSION 3.13.0)

project(net-server VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# file(GLOB_RECURSE SOURCES
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
# )

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/" SOURCES)

#add_library(net-client STATIC ${SOURCES})

add_executable(net-server ${SOURCES})

target_link_directories(net-server  PRIVATE
    ${CMAKE_SOURCE_DIR}/modules/protobuf-types/lib
)

target_include_directories(net-server  PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
    ${CMAKE_SOURCE_DIR}/modules/protobuf-types/src #protobuf format header
)

target_link_libraries(net-server  PRIVATE 
    net-common
    ws2_32
    mswsock
    protoMessage
)