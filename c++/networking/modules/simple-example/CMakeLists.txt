cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES
    "*.h"
    "*.cpp"
)

#add_library(net-client STATIC ${SOURCES})

add_executable(example ${SOURCES})

target_include_directories(example   PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
)

target_link_libraries(example  PRIVATE 
    net-common
    ws2_32
    mswsock
)