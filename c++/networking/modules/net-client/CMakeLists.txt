cmake_minimum_required(VERSION 3.13.0)

project(net-client VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# file(GLOB_RECURSE SOURCES
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
# )


file(GLOB IMGUI_SRC
    ${CMAKE_SOURCE_DIR}/libs/imgui/*.h
    ${CMAKE_SOURCE_DIR}/libs/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.* #imgui backend
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.*
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3_loader.*
)

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/" SOURCES)

#add_library(net-client STATIC ${SOURCES})

add_executable(net-client ${SOURCES} ${IMGUI_SRC})

target_link_directories(net-client  PRIVATE
    ${CMAKE_SOURCE_DIR}/modules/protobuf-types/lib
    ${CMAKE_SOURCE_DIR}/libs/glfw-3.3.8.bin.WIN64/lib-mingw-w64
)


target_include_directories(net-client  PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
    ${CMAKE_SOURCE_DIR}/modules/protobuf-types/gen #protobuf format header
    ${CMAKE_SOURCE_DIR}/libs/imgui/
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends
    ${CMAKE_SOURCE_DIR}/libs/glfw-3.3.8.bin.WIN64/include/
)

target_link_libraries(net-client  PRIVATE 
    net-common
    ws2_32
    mswsock
    protoMessage
    opengl32
    glfw3.a
)