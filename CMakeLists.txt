cmake_minimum_required(VERSION 3.12)

project(handover-demo-y1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Add the custom cmake modules
set(CMAKE_CONFIG_PATH ${CMAKE_MODULE_PATH}  "${CMAKE_CURRENT_LIST_DIR}/bts/cmake-modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CONFIG_PATH}")

message(STATUS "CMAKE_CONFIG_PATH ${CMAKE_MODULE_PATH}  ${CMAKE_CURRENT_LIST_DIR}/bts/cmake-modules")
message(STATUS ""${CMAKE_CONFIG_PATH}"")

# BehaviorTree.CPP links Boost::coroutine, but does not look for it
find_package(ZMQ)
find_package(Boost COMPONENTS coroutine REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)

find_package(YARP COMPONENTS os idl_tools dev REQUIRED)

if( ZMQ_FOUND )
    message(STATUS "ZeroMQ found.")
    add_definitions( -DZMQ_FOUND )
    list(APPEND BUILD_TOOL_INCLUDE_DIRS ${ZMQ_INCLUDE_DIRS})
else()
    message(WARNING "ZeroMQ NOT found. The execution of the BT will not be visible with Groot.")
endif()

add_subdirectory(components)
add_subdirectory(bts/src)