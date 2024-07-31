cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE SOURCES
    "*.h"
    "*.cpp"
)

#add_library(net-client STATIC ${SOURCES})

add_executable(net-client ${SOURCES})

target_include_directories(net-client  PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
)

target_link_libraries(net-client  PRIVATE 
    net-common
    ws2_32
    mswsock
)