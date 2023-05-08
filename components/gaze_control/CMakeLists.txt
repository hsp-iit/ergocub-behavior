################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# IMPORT
cmake_minimum_required(VERSION 3.12)

project(gaze_control LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

 # Create the component (navigation client)
find_package(YARP COMPONENTS os dev REQUIRED)
include_directories(include)

add_library(gaze_control_interface)  # ADDED
target_sources(gaze_control_interface  # DECOMMENfED
        PRIVATE
        RPCServerInterface.cpp)  # DECOMMENTED
target_link_libraries(gaze_control_interface  # DECOMMENTED
        PRIVATE
        YARP::YARP_os
        YARP::YARP_init
        YARP::YARP_dev)