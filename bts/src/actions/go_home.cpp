#include <behaviortree_cpp_v3/action_node.h>

#include "go_home.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoHome::GoHome(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool GoHome::init(std::string name)
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
    auto now = std::chrono::system_clock::now();
    last_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() - 4;
    return true;
}

NodeStatus GoHome::tick()
{
    auto now = std::chrono::system_clock::now();
    long this_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    if((this_time - last_time) > 3){
        #ifdef MANIPULATION
        manipulation_client_.home(false);
        #endif
        last_time = this_time;
    }

    return NodeStatus::SUCCESS;
}


PortsList GoHome::providedPorts()
{
    return {};
}
