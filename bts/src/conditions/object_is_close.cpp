#include <behaviortree_cpp_v3/condition_node.h>
#include "object_is_close.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


ObjectIsClose::ObjectIsClose(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool ObjectIsClose::init(std::string name)
{
    std::string server_name = "/Components/ObjectDetection"s;
    std::string client_name = "/BT/" + name + "/ObjectDetection"s;

    client_port.open(client_name);

    if (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        return false;
    }
    object_detection_client_.yarp().attachAsClient(client_port);
    return true;
}

NodeStatus ObjectIsClose::tick()
{
    double distance = object_detection_client_.get_distance();
    std::cout << distance << std::endl;
    if (distance == -1 or distance == 0 or distance >= threshold){
        return BT::NodeStatus::FAILURE;
    }
    return  BT::NodeStatus::SUCCESS;
}

PortsList ObjectIsClose::providedPorts()
{
    return {};
}
