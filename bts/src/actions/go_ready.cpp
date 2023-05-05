#include <behaviortree_cpp_v3/action_node.h>

#include "go_ready.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoReady::GoReady(string name, const NodeConfiguration& config) :
    StatefulActionNode(name, config)
{
    is_ok_ = init(name);
}

bool GoReady::init(std::string name)
{
    // MANIPULATION
    #ifdef MANIPULATION
    std::string server_name = "/Components/Manipulation"s;
    std::string client_name = "/BT/" + name + "/Manipulation"s;

    client_port.open(client_name);

    if (!yarp.connect(client_name,server_name))
    {
        throw BT::RuntimeError("Error! Could not connect to server ", server_name);
    }
    manipulation_client_.yarp().attachAsClient(client_port);
    #endif
    return true;
}

NodeStatus GoReady::onStart()
{
    #ifdef MANIPULATION
    manipulation_client_.perform_joint_space_action("ready");
    #endif
    return NodeStatus::RUNNING;
}

NodeStatus GoReady::onRunning(){
    #ifdef MANIPULATION
    if (manipulation_client_.is_finished()){
        return NodeStatus::SUCCESS;
    }
    return NodeStatus::RUNNING;
    #endif
    return NodeStatus::SUCCESS;
}

void GoReady::onHalted(){
    return;
}


PortsList GoReady::providedPorts()
{
    return {};
}
