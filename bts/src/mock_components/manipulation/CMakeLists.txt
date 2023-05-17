add_executable(mock_manipulation)

target_include_directories(mock_manipulation
    PRIVATE
    ../../interfaces/manipulation/include
)

target_sources(mock_manipulation
    PRIVATE
    MockManipulation.cpp
    ../../interfaces/manipulation/src/CommandInterface.cpp
)

target_link_libraries(mock_manipulation
    PRIVATE
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_dev
    )
