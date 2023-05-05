#include <behaviortree_cpp_v3/action_node.h>

#include "is_doing_action.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

IsDoingAction::IsDoingAction(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool IsDoingAction::init(std::string name)
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

NodeStatus IsDoingAction::tick()
{
    #ifdef MANIPULATION
    auto fin = manipulation_client_.is_finished();
    if (fin){
        setOutput("is_doing_action", "yes");
    }
    else{
        setOutput("is_doing_action", "no");
    }
    #endif
    return NodeStatus::SUCCESS;
}


PortsList IsDoingAction::providedPorts()
{
    return {OutputPort<std::string>("is_doing_action")};
}
