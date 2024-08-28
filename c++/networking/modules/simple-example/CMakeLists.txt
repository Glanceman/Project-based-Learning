cmake_minimum_required(VERSION 3.5.0)

project(simple-example VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# file(GLOB_RECURSE SOURCES
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
# )

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/" SOURCES)

#add_library(net-client STATIC ${SOURCES})


add_executable(simple-example ${SOURCES})

target_link_directories(simple-example PRIVATE
    ${CMAKE_SOURCE_DIR}/modules/protobuf-types/lib
)

target_include_directories(simple-example PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
    ${CMAKE_SOURCE_DIR}/modules/protobuf-types/gen #protobuf format header
)

target_link_libraries(simple-example  PRIVATE 
    net-common
    ws2_32
    mswsock
    protoMessage
)