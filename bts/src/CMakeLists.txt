################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the custom_leaf_nodes library
add_library(custom_leaf_nodes STATIC)
find_package(eCubPerceptionInterface REQUIRED)
include_directories(
        actions
        conditions
        ${CMAKE_SOURCE_DIR}/components/manipulation/include
        ${CMAKE_SOURCE_DIR}/components/gaze_control/include
        ${CMAKE_SOURCE_DIR}/components/ecub_emotions/include
)

file(GLOB BT_NODES    
    "actions/*.cpp"
    "conditions/*.cpp"
)

target_sources(custom_leaf_nodes
  PRIVATE
  ${BT_NODES}
  common.cpp
)

target_include_directories(custom_leaf_nodes
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(custom_leaf_nodes
  PRIVATE
    manipulation_interface
    gaze_control_interface
    ecub_emotions_interface
    eCubPerceptionInterface::eCubPerceptionInterface
    BT::behaviortree_cpp_v3
    YARP::YARP_os
    )


add_executable(run_bt)

target_sources(run_bt
    PRIVATE
    run_bt.cpp
)

target_link_libraries(run_bt
    PRIVATE
    custom_leaf_nodes
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_dev
    )
