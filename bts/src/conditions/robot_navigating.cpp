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
    yarp::os::Network::connect(writer_name, reader_name);
}


NodeStatus RobotNavigating::tick()
{
    yarp::os::Bottle *data = reader_port.read(false);

    if (data != nullptr){
        auto cmd = data->get(0).asInt32();
        if (cmd == 0){
            is_navigating = true;
        } else {
            is_navigating = false;
        }
    }

    return is_navigating ? NodeStatus::FAILURE : NodeStatus::SUCCESS;
}

