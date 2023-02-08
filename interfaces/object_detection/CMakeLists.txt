################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

cmake_minimum_required(VERSION 3.12)
project(action_recognition_interface LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
find_package(YARP COMPONENTS os idl_tools REQUIRED)

# Create the object_detection_interface C++ Library
add_library(object_detection_interface STATIC)
find_package(YARP COMPONENTS os idl_tools REQUIRED)
yarp_add_idl(IDL_GEN_FILES object_detection.thrift)
target_sources(object_detection_interface
  PRIVATE
 ${IDL_GEN_FILES}
  ObjectDetectionInterface.cpp
  ObjectDetectionInterface.h
)
target_include_directories(object_detection_interface
#   INTERFACE
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(object_detection_interface
  PRIVATE
    YARP::YARP_os)