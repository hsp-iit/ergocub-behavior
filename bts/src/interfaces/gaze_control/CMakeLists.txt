################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# IMPORT


 # Create the component (navigation client)
find_package(YARP COMPONENTS os dev REQUIRED)

yarp_idl_to_dir(INPUT_FILES gaze-controller.thrift
                OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}
                PLACEMENT SEPARATE
                )

file(REMOVE gaze-controller_index.txt)
target_include_directories(custom_leaf_nodes PUBLIC include)
