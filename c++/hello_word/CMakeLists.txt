cmake_minimum_required(VERSION 3.5.0)

project(hello_world VERSION 0.1.0 LANGUAGES C CXX)

# Set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# use for without the subdirectory names e.g #include "foo2/foo2.h" -> #include "foo2.h"
# # Include directories 
# target_include_directories(MyExecutable PRIVATE 
#     src
#     src/foo
#     src/foo2
# )

# Create the dist directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
