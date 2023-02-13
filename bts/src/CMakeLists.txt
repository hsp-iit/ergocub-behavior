################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the custom_leaf_nodes library
add_library(custom_leaf_nodes STATIC)

include_directories(
        actions
        conditions
        ${CMAKE_SOURCE_DIR}/components/action_recognition/include
        ${CMAKE_SOURCE_DIR}/components/object_detection/include
)

target_sources(custom_leaf_nodes
  PRIVATE
  actions/ask_to_get_closer.cpp
  actions/ask_to_pay_attention.cpp
  actions/ask_to_keep_the_box_still.cpp
  actions/perform_grasp.cpp

  actions/ask_to_move_object_closer.cpp
  conditions/handout_successful.cpp
  conditions/correct_action_recognized.cpp
  conditions/human_not_too_far.cpp
  conditions/human_not_getting_closer.cpp
  conditions/human_paying_attention.cpp
  conditions/are_points_reachable.cpp

  conditions/is_box_still.cpp
)

target_include_directories(custom_leaf_nodes
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(custom_leaf_nodes
  PRIVATE
    action_recognition_interface
    BT::behaviortree_cpp_v3
    YARP::YARP_os
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
