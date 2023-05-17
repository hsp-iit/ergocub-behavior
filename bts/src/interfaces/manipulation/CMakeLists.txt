################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# IMPORT
cmake_minimum_required(VERSION 3.12)

project(manipulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(YARP COMPONENTS os dev idl_tools REQUIRED)

yarp_idl_to_dir(INPUT_FILES CommandInterface.thrift
                OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}
                PLACEMENT SEPARATE
                SOURCES_VAR IDL_SRC
                HEADERS_VAR IDL_H
                )
file(REMOVE CommandInterface_index.txt)
target_include_directories(custom_leaf_nodes PUBLIC include)
