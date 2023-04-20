################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the custom_leaf_nodes library
add_library(custom_leaf_nodes STATIC)
find_package(iDynTree REQUIRED)
include_directories(
        actions
        conditions
        ${CMAKE_SOURCE_DIR}/components/action_recognition/include
        ${CMAKE_SOURCE_DIR}/components/object_detection/include
        ${CMAKE_SOURCE_DIR}/components/ergocub_perception/include
        ${CMAKE_SOURCE_DIR}/components/manipulation/include
        ${CMAKE_SOURCE_DIR}/components/move_camera/include
)

target_sources(custom_leaf_nodes
  PRIVATE
  actions/robot_change_face.cpp
  actions/robot_look_at_poi.cpp
  actions/do_response_action.cpp
  actions/go_home.cpp
  actions/go_ready.cpp
  actions/go_grasp.cpp
  actions/go_release.cpp
  conditions/box_detected.cpp
  conditions/face_detected.cpp
  conditions/object_is_close.cpp
  conditions/focus_detected.cpp
  conditions/action_detected.cpp
  common.cpp
)

target_include_directories(custom_leaf_nodes
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(custom_leaf_nodes
  PRIVATE
    action_recognition_interface
    object_detection_interface
    ergocub_perception_interface
    manipulation_interface
    move_camera
    BT::behaviortree_cpp_v3
    YARP::YARP_os
    iDynTree::idyntree-high-level
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
    YARP::YARP_dev
    )
