################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(YARP COMPONENTS os dev sig REQUIRED)
include_directories(include)

add_library(ergocub_perception_interface)
target_sources(ergocub_perception_interface
        PRIVATE
        eCubPerceptionInterface.cpp)
target_link_libraries(ergocub_perception_interface
        PRIVATE
        YARP::YARP_os
        YARP::YARP_init
        YARP::YARP_dev
        YARP::YARP_sig)