cmake_minimum_required(VERSION 3.20.0)

SET(CURRENT_PROJECT_NAME TinyRenderer_Test)

project(${CURRENT_PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 23)

message("test/CURRENT_PROJECT_NAME ${CURRENT_PROJECT_NAME} ")
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.h")
# Remove main.cpp from sources
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/" TEST_SOURCES)

add_executable(${CURRENT_PROJECT_NAME} ${TEST_SOURCES} ${SOURCES})

# gtest 头文件
# gmock 头文件
target_include_directories(
    ${CURRENT_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/libs/googletest-1.15.2/googletest/include
    ${CMAKE_SOURCE_DIR}/libs/googletest-1.15.2/googlemock/include
)

target_link_directories(${CURRENT_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/googletest-1.15.2/build/lib/)


set(GTEST_LIBS 
    gtest 
    gmock 
    gtest_main 
    gmock_main 
    pthread
)
target_link_libraries(${CURRENT_PROJECT_NAME} ${GTEST_LIBS})