################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the custom_leaf_nodes library
add_library(custom_leaf_nodes STATIC)

include_directories(actions conditions)
target_sources(custom_leaf_nodes
  PRIVATE
  actions/ask_to_get_closer.cpp
  conditions/handout_successful.cpp
  conditions/correct_action_recognized.cpp
)

target_include_directories(custom_leaf_nodes
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(custom_leaf_nodes
  PUBLIC
    BT::behaviortree_cpp_v3
    )



add_executable(run_bt)

target_sources(run_bt
    PRIVATE
    run_bt.cpp
)

target_link_libraries(run_bt  # DECOMMENTED
    PRIVATE
    custom_leaf_nodes
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_dev)
