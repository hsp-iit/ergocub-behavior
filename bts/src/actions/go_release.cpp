#include <behaviortree_cpp_v3/action_node.h>

#include "go_release.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoRelease::GoRelease(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool GoRelease::init(std::string name)
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

NodeStatus GoRelease::tick()
{
    #ifdef MANIPULATION   
    manipulation_client_.release(false);
    #endif
    setOutput<std::string>("has_box", "no");
    return NodeStatus::SUCCESS;
}


PortsList GoRelease::providedPorts()
{
    return {OutputPort<std::string>("has_box")};
}
