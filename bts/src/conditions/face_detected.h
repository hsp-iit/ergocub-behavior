#pragma once


#include <behaviortree_cpp_v3/condition_node.h>
#include <ActionRecognitionInterface.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

using namespace BT;
using namespace std;

class FaceDetected :  public ConditionNode
{
public:
    FaceDetected(string name, const NodeConfiguration &config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    bool init(std::string);
    ActionRecognitionInterface action_recognition_client_;
    bool is_ok_{false};
    yarp::os::Network yarp;
    yarp::os::Port client_port;
    bool was_true;
};
