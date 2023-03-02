#include <ecub_head.h>

#include <yarp/os/Property.h>

using namespace yarp::os;


eCubHead::eCubHead(const std::string& robot_name, const std::string& port_prefix)
{
    /* Check YARP network. */
    if (!network_.checkNetwork())
        throw(std::runtime_error(log_prefix_ + "::ctor. Error: YARP is not available."));

    Property properties;
    properties.put("device", "remote_controlboard");
    properties.put("local", "/" + port_prefix + "/head/");
    properties.put("remote", "/" + robot_name + "/head");

    /* Try to open the driver. */
    if (!driver_.open(properties))
        throw(std::runtime_error(log_prefix_ + "::ctor. Error: cannot open the head driver."));

    if (!(driver_.view(position_interface_) && (position_interface_ != nullptr)))
    {
        throw(std::runtime_error(log_prefix_ + "::ctor. Error: cannot open the head position view."));
    }
}


void eCubHead::close()
{
    stop();

    if (driver_.isValid())
        driver_.close();
}


bool eCubHead::stop()
{
    return position_interface_->stop();
}


bool eCubHead::set_neck_pitch(const double& value)
{
    return position_interface_->positionMove(0, value);
}


bool eCubHead::set_neck_roll(const double& value)
{
    return position_interface_->positionMove(1, value);
}


bool eCubHead::set_neck_yaw(const double& value)
{
    return position_interface_->positionMove(2, value);
}


bool eCubHead::set_camera_tilt(const double& value)
{
    return position_interface_->positionMove(3, value);
}


bool eCubHead::set_neck_pitch_velocity(const double& value)
{
    return position_interface_->setRefSpeed(0, value);
}


bool eCubHead::set_neck_roll_velocity(const double& value)
{
    return position_interface_->setRefSpeed(1, value);
}


bool eCubHead::set_neck_yaw_velocity(const double& value)
{
    return position_interface_->setRefSpeed(2, value);
}


bool eCubHead::set_camera_tilt_velocity(const double& value)
{
    return position_interface_->setRefSpeed(3, value);
}
