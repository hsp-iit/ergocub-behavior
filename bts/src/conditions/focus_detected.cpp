#include <behaviortree_cpp_v3/condition_node.h>
#include "focus_detected.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


FocusDetected::FocusDetected(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool FocusDetected::init(std::string name)
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

NodeStatus FocusDetected::tick()
{
    auto is_focused = ecub_perception_client_.is_focused();
    if(is_focused)
        setOutput("focus", "yes" );
    else
        setOutput("focus", "no" );
    return  BT::NodeStatus::SUCCESS;
}

PortsList FocusDetected::providedPorts()
{
    return {OutputPort<std::string>("focus")};
}