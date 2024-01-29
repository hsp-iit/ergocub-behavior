#include <behaviortree_cpp_v3/condition_node.h>
#include "robot_navigating.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


RobotNavigating::RobotNavigating(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    ConditionNode(name, nc),
    bt_config(bt_config)
{
    writer_name =  bt_config.get<std::string>("components.navigation.port");
    reader_name = "/BT/" + name;

    reader_port.open(reader_name);
}


NodeStatus RobotNavigating::tick()
{
    yarp::os::Bottle *data = reader_port.read(false);
    
    if (data == nullptr){
        return NodeStatus::SUCCESS;
    }

    if (data->get(0).asInt16() == 0){
        return NodeStatus::SUCCESS;
    }

    return NodeStatus::FAILURE;
}

