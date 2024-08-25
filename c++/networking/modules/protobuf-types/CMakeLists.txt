cmake_minimum_required(VERSION 3.13.0)

project(protoMessage VERSION 0.1.0 LANGUAGES C CXX)


set(absl_DIR ${CMAKE_SOURCE_DIR}/libs/protobuf-27.3-g++/lib/cmake/absl)
set(utf8_range_DIR ${CMAKE_SOURCE_DIR}/libs/protobuf-27.3-g++/lib/cmake/utf8_range)
set(Protobuf_DIR ${CMAKE_SOURCE_DIR}/libs/protobuf-27.3-g++/lib/cmake/protobuf)

find_package(Protobuf REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# file(GLOB_RECURSE SOURCES
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
# )

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/" SOURCES)
#aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/gen/" FORMAT_SOURCES)

#add_library(net-client STATIC ${SOURCES})
#message(${CMAKE_SOURCE_DIR}/libs/protobuf-27.3-g++/lib)
# link_directories(
#     ${CMAKE_SOURCE_DIR}/libs/protobuf-27.3-g++/lib/
# )

#add_executable(protoMessage ${SOURCES} )
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_library(protoMessage STATIC ${SOURCES})

target_include_directories(protoMessage PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/gen    
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/protobuf-27.3-g++/include
)

#get_target_property(PROTOBUF_LIB_PATH protobuf::libprotobuf IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE)
#message(STATUS "protobuf::libprotobuf is located at: ${PROTOBUF_LIB_PATH}")


target_link_libraries(protoMessage protobuf::libprotobuf)

