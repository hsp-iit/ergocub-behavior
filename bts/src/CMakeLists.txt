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
        ${CMAKE_SOURCE_DIR}/components/manipulation/include
)

target_sources(custom_leaf_nodes
  PRIVATE
  actions/ask_to_get_closer.cpp
  actions/ask_to_keep_the_box_still.cpp
  actions/ask_to_move_object_closer.cpp
  actions/ask_to_pay_attention.cpp
  actions/perform_grasp.cpp
  actions/robot_give_box.cpp
  actions/robot_handshake.cpp
  actions/robot_says_be_careful.cpp
  actions/robot_wave.cpp
  actions/go_home.cpp

  conditions/are_points_reachable.cpp
  conditions/get_action_recognized.cpp
  conditions/handout_successful.cpp
  conditions/handshake_action_recognized.cpp
  conditions/hello_action_recognized.cpp
  conditions/human_is_not_moving_towards_camera.cpp
  conditions/human_not_getting_closer.cpp
  conditions/human_not_too_far.cpp
  conditions/human_paying_attention.cpp
  conditions/is_box_still.cpp
  conditions/lift_action_recognized.cpp
  conditions/stand_action_recognized.cpp
)

target_include_directories(custom_leaf_nodes
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(custom_leaf_nodes
  PRIVATE
    action_recognition_interface
    object_detection_interface
    manipulation_interface
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
