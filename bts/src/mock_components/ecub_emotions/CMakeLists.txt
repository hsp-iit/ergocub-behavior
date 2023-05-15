add_executable(mock_emotions)

target_include_directories(mock_emotions
    PRIVATE
    ../../interfaces/ecub_emotions/include
)

target_sources(mock_emotions
    PRIVATE
    MockEmotions.cpp
    ../../interfaces/ecub_emotions/src/ergoCubEmotions_IDL.cpp
)

target_link_libraries(mock_emotions
    PRIVATE
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_dev
    )
