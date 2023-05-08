#include <behaviortree_cpp_v3/condition_node.h>
#include "object_is_graspable.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


ObjectIsGraspable::ObjectIsGraspable(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool ObjectIsGraspable::init(std::string name)
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

NodeStatus ObjectIsGraspable::tick()
{
    Optional<std::vector<double>> msg = getInput<std::vector<double>>("poi_pos");
    if (!msg)
    {
        throw BT::RuntimeError("missing required input [message]: ", msg.error() );
    }
    std::vector<double> obj_pos = msg.value();

    if(obj_pos[0]>-0.50 && obj_pos[0]<-0.20 &&  // between 25 and 45 centimeter distant from torso
       obj_pos[1]>-0.1 && obj_pos[1]<0.1 &&  // 5 centemiters on left or right
       obj_pos[2]>-0.1    && obj_pos[2]<0.20
    )
        return BT::NodeStatus::SUCCESS;
    else
        return BT::NodeStatus::FAILURE;


    // std::cout << distance << std::endl;
    // if (distance == -1 or distance == 0 or distance >= threshold){
    //     return BT::NodeStatus::FAILURE;
    // }
    // return  BT::NodeStatus::SUCCESS;
}

PortsList ObjectIsGraspable::providedPorts()
{
    return {InputPort<std::vector<double>>("poi_pos")};
}
