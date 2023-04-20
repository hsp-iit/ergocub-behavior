#include <behaviortree_cpp_v3/condition_node.h>
#include "box_detected.h"
#include "common.h"

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
    std::string server_name = "/Components/eCubPerception"s;
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


NodeStatus BoxDetected::tick()
{
    auto object_position_yarp = ecub_perception_client_.get_center();
    std::vector<double> object_position(3);
    for (std::size_t i = 0; i < 3; ++i)
        object_position[i] = object_position_yarp[i];

    // std::cout << distance << std::endl;
    if (are_all_elements_zero(object_position)){
       throw(std::runtime_error("box_detected: received (0,0,0) as target position (maybe segmentation is dead?"));
    }
    if (are_all_elements_minus_one(object_position)){
        setOutput("poi", "none" );
        return BT::NodeStatus::FAILURE;
    }
    if(are_all_elements_minus_two(object_position)){
        if (was_true){  // continue following (special value)
            setOutput("poi", "object");
            setOutput("poi_pos", object_position);
            return  BT::NodeStatus::SUCCESS;
        }
        else{  // do not follow
        setOutput("poi", "none" );
        return BT::NodeStatus::FAILURE;
        }
    }
    setOutput("poi", "object" );
    setOutput("poi_pos", object_position);
    return  BT::NodeStatus::SUCCESS;
}

PortsList BoxDetected::providedPorts()
{
    return {OutputPort<std::string>("poi"), OutputPort<std::vector<double>>("poi_pos")};
}
