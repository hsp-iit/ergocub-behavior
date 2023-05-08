#include <behaviortree_cpp_v3/condition_node.h>
#include "action_detected.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


ActionDetected::ActionDetected(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool ActionDetected::init(std::string name)
{
    std::string server_name = "/eCubPerception/rpc:i"s;
    std::string client_name = "/BT/" + name + "/eCubPerception"s;

    client_port.open(client_name);

    if (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        return false;
    }
    ecub_perception_client_.yarp().attachAsClient(client_port);
    return true;
}


NodeStatus ActionDetected::tick()
{
    std::string action = ecub_perception_client_.get_action();
    setOutput<std::string>("action", action);
    return NodeStatus::SUCCESS;
}

PortsList ActionDetected::providedPorts()
{
    return {OutputPort<std::string>("action")};
}