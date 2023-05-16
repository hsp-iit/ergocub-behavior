################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the custom_leaf_nodes library
add_library(custom_leaf_nodes STATIC)

add_subdirectory(interfaces)
add_subdirectory(mock_components)

find_package(eCubPerceptionInterface REQUIRED)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        actions
        conditions
)

file(GLOB BT_NODES "actions/*.cpp" "conditions/*.cpp")
file(GLOB COMPONENTS "interfaces/*/src/*.cpp")

target_sources(custom_leaf_nodes
  PRIVATE
  ${BT_NODES}
  ${COMPONENTS}
  common.cpp
)

target_link_libraries(custom_leaf_nodes
  PRIVATE
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
