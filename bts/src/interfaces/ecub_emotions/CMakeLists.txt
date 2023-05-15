################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# IMPORT

find_package(YARP COMPONENTS os dev idl_tools REQUIRED)

yarp_idl_to_dir(INPUT_FILES idl.thrift
                OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}
                PLACEMENT SEPARATE
                SOURCES_VAR IDL_SRC
                HEADER_VAR IDL_H
                )

target_include_directories(custom_leaf_nodes PUBLIC include)
