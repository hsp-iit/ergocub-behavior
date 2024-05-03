#include <behaviortree_cpp_v3/condition_node.h>
#include "robot_navigating.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/LogStream.h>

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
    if (data == nullptr){
        return NodeStatus::SUCCESS;
    }

    switch (data->get(0).asInt32())
    {
    case 0: // STATUS_UNKNOWN
        return NodeStatus::SUCCESS;
        break;
    case 1: // STATUS_ACCEPTED The goal has been accepted and is awaiting execution
        return NodeStatus::FAILURE;
        break;
    case 2: // STATUS_EXECUTING The goal is currently being executed by the action server
        return NodeStatus::FAILURE;
        break;
    case 3: // STATUS_CANCELING The client has requested that the goal be canceled 
        return NodeStatus::SUCCESS;
        break;
    case 4: // STATUS_SUCCEEDED The goal was achieved successfully by the action server
        return NodeStatus::SUCCESS;
        break;
    case 5: // STATUS_CANCELED The goal was canceled after an external request from an action client
        return NodeStatus::SUCCESS;
        break;
    case 6: // STATUS_ABORTED The goal was terminated by the action server without an external request
        return NodeStatus::SUCCESS;
        break;
    default:
        yWarning() << "[RobotNavigating::tick] Received an unexpected value from the navigation status: " << data->get(0).asInt32() << " Carrying on...";
        return NodeStatus::SUCCESS;
        break;
    }
    //yarp::os::Bottle *data = reader_port.read(false);

    //if (data != nullptr){
    //    auto cmd = data->get(0).asInt32();
    //    if (cmd == 0){
    //        is_navigating = true;
    //    } else {
    //        is_navigating = false;
    //    }
    //}

    //return is_navigating ? NodeStatus::FAILURE : NodeStatus::SUCCESS;
}

