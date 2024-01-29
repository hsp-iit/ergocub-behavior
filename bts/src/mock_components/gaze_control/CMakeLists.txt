add_executable(mock_gaze_control)

target_include_directories(mock_gaze_control
    PRIVATE
    ../../interfaces/gaze_control/include
)

target_sources(mock_gaze_control
    PRIVATE
    MockGazeController.cpp
    ../../interfaces/gaze_control/src/RPCServerInterface.cpp
)

target_link_libraries(mock_gaze_control
    PRIVATE
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_dev
    )
