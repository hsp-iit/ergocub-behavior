#pragma once


#include <behaviortree_cpp_v3/condition_node.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <eCubPerceptionInterface/eCubPerceptionInterface.h>

using namespace BT;
using namespace std;

class ActionDetected :  public ConditionNode
{
public:
    ActionDetected(string name, const NodeConfiguration &config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    bool init(std::string);
    eCubPerceptionInterface ecub_perception_client_;
    bool is_ok_{false};
    yarp::os::Network yarp;
    yarp::os::Port client_port;
};
