################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# IMPORT
cmake_minimum_required(VERSION 3.12)

# project(ergocub_perception_interface LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

 # Create the component (navigation client)
# find_package(YARP COMPONENTS os sig dev REQUIRED)
# include_directories(include)

# add_executable(ergocub_perception)  # ADDED
# target_sources(ergocub_perception  # DECOMMENfED
#   PRIVATE
#     eCubPerceptionInterface.cpp)  # DECOMMENTED
# target_link_libraries(ergocub_perception  # DECOMMENTED
#   PRIVATE
#     YARP::YARP_os
#     YARP::YARP_init
#     YARP::YARP_dev
#     YARP::YARP_sig)


find_package(YARP COMPONENTS os dev sig REQUIRED)
include_directories(include)

add_library(ergocub_perception_interface)  # ADDED
target_sources(ergocub_perception_interface  # DECOMMENfED
        PRIVATE
        eCubPerceptionInterface.cpp)  # DECOMMENTED
target_link_libraries(ergocub_perception_interface  # DECOMMENTED
        PRIVATE
        YARP::YARP_os
        YARP::YARP_init
        YARP::YARP_dev
        YARP::YARP_sig)