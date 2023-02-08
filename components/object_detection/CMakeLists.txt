################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# IMPORT
cmake_minimum_required(VERSION 3.12)

project(object_detection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

 # Create the component (navigation client)
find_package(YARP COMPONENTS os dev REQUIRED)
include_directories(include)
add_executable(object_detection)  # ADDED
target_sources(object_detection  # DECOMMENTED
  PRIVATE
    ObjectDetectionInterface.cpp ObjectDetection.cpp)  # DECOMMENTED
target_link_libraries(object_detection  # DECOMMENTED
  PRIVATE
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_dev)