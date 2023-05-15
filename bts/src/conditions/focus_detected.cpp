#include <behaviortree_cpp_v3/condition_node.h>
#include "focus_detected.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


FocusDetected::FocusDetected(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    ConditionNode(name, nc),
    bt_config(bt_config)
{
    std::string server_name =  bt_config.get<std::string>("components.perception.port");
    std::string client_name = "/BT/" + name + server_name;

    client_port.open(client_name);

    while (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    ecub_perception_client_.yarp().attachAsClient(client_port);
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