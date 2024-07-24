cmake_minimum_required(VERSION 3.5.0)

project(asioApp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)




file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})


# Include Asio
target_include_directories(${CMAKE_PROJECT_NAME}  PRIVATE 
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
)

# # Define ASIO_STANDALONE if you're not using Boost
# target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE 
#     ASIO_STANDALONE
#     _WIN32_WINNT=0x0601  # Target Windows 7 or later
# )

# Link necessary libraries for Windows
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    ws2_32
    mswsock
)

# Create the dist directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

