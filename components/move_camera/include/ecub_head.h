#include <yarp/dev/IPositionControl.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Network.h>

#ifndef ECUB_HEAD_H
#define ECUB_HEAD_H

class eCubHead
{
public:
    eCubHead(const std::string& robot_name, const std::string& port_prefix);

    void close();

    bool stop();

    bool set_neck_pitch(const double& value);

    bool set_neck_roll(const double& value);

    bool set_neck_yaw(const double& value);

    bool set_camera_tilt(const double& value);

    bool set_neck_pitch_velocity(const double& value);

    bool set_neck_roll_velocity(const double& value);

    bool set_neck_yaw_velocity(const double& value);

    bool set_camera_tilt_velocity(const double& value);

private:
    yarp::dev::PolyDriver driver_;

    yarp::dev::IPositionControl* position_interface_;

    yarp::os::Network network_;

    const std::string log_prefix_ = "eCubHead";
};

#endif /* ECUB_HEAD_H */
