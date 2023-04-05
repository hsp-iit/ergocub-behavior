#pragma once


#include <behaviortree_cpp_v3/action_node.h>
#include <ObjectDetectionInterface.h>
#include <ActionRecognitionInterface.h>
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
    ObjectDetectionInterface object_detection_client_;
    ActionRecognitionInterface action_recognition_client_;
    yarp::os::Port ar_client_port;
    yarp::os::Port od_client_port;
    yarp::os::Port rhp_client_port;
    yarp::dev::PolyDriver driver_gaze_;
    yarp::dev::IGazeControl* gaze_;
    const std::string log_name_ = "iCubGaze";
    yarp::os::BufferedPort<yarp::os::Bottle> port;
    //void halt() override;

    std::string ar_server_name;
    std::string ar_client_name;
};
