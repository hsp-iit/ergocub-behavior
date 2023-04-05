#include <behaviortree_cpp_v3/condition_node.h>
#include "box_detected.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


BoxDetected::BoxDetected(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool BoxDetected::init(std::string name)
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

bool obj_are_all_elements_minus_one(const std::vector<double>& vec) {
    for (double element : vec) {
        if (element != -1.) {
            return false;
        }
    }
    return true;
}

NodeStatus BoxDetected::tick()
{
    auto object_position = object_detection_client_.get_object_position();
    // std::cout << distance << std::endl;
    if (obj_are_all_elements_minus_one(object_position)){
        setOutput("poi", "none" );
        return BT::NodeStatus::FAILURE;
    }
    setOutput("poi", "object" );
    setOutput("poi_pos", object_position);
    return  BT::NodeStatus::SUCCESS;
}

PortsList BoxDetected::providedPorts()
{
    return {OutputPort<std::string>("poi"), OutputPort<std::vector<double>>("poi_pos")};
}
