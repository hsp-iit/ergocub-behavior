################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# IMPORT
cmake_minimum_required(VERSION 3.12)

project(action_recognition LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

 # Create the component (navigation client)
find_package(YARP COMPONENTS os dev REQUIRED)
include_directories(include)
add_executable(action_recognition)  # ADDED
target_sources(action_recognition  # DECOMMENfED
  PRIVATE
    ActionRecognitionInterface.cpp ActionRecognition.cpp)  # DECOMMENTED
target_link_libraries(action_recognition  # DECOMMENTED
  PRIVATE
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_dev)