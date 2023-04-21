#pragma once


#include <behaviortree_cpp_v3/action_node.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/GazeControl.h>
#include <yarp/os/BufferedPort.h>

using namespace BT;
using namespace std;

class RobotLookAtPOI :  public SyncActionNode
{
public:
    NodeStatus tick() override;
    static PortsList providedPorts();
    RobotLookAtPOI(string name, const NodeConfiguration &config);
    yarp::dev::IGazeControl& controller();
private:
    bool init(std::string);
    bool is_ok_{false};
    yarp::dev::PolyDriver driver_gaze_;
    yarp::dev::IGazeControl* gaze_;
    const std::string log_name_ = "iCubGaze";
    yarp::os::BufferedPort<yarp::os::Bottle> port;
    int none_counter;
    int none_counter_thr = 15;
};
