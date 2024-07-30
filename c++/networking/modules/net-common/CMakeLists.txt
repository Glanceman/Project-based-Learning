cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# file(GLOB_RECURSE SOURCES
#     "/*.h"
#     "/*.cpp"
# )


add_library(net-common net_common.h net_message.h olc_net.h)
target_include_directories(net-common INTERFACE 
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
)

# No need to link libraries for header-only library
# target_link_libraries(net-common PRIVATE 
#     ws2_32
#     mswsock
# )

set_target_properties(net-common PROPERTIES LINKER_LANGUAGE CXX)