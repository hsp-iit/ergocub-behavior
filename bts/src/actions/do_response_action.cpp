#include <behaviortree_cpp_v3/action_node.h>

#include "do_response_action.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>


DoResponseAction::DoResponseAction(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool DoResponseAction::init(std::string name)
{
    #ifdef MANIPULATION
    // Connect to manipulation
    std::string man_server_name = "/Components/Manipulation"s;
    std::string man_client_name = "/BT/" + name + "/Manipulation"s;

    man_client_port.open(man_client_name);

    if (!yarp.connect(man_client_name,man_server_name))
    {
        throw BT::RuntimeError("Error! Could not connect to server ", man_server_name);
    }
    manipulation_client_.yarp().attachAsClient(man_client_port);
    #endif

    // Others
    last_action = "";
    auto now = std::chrono::system_clock::now();
    last_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() - 4;
    return true;
}

NodeStatus DoResponseAction::tick()
{

    auto now = std::chrono::system_clock::now();
    long this_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    Optional<std::string> msg = getInput<std::string>("action");
    if (!msg)
    {
        throw BT::RuntimeError("missing required input [message]: ", msg.error() );
    }
    std::string action = msg.value();

    std::cout << "received action " << action << std::endl;
    std::cout << "last action " << last_action << std::endl;

    if (action == "wave"){
        if(last_action != action || (this_time - last_time) > 3){
            std::cout << "WAVE" << std::endl;
            #ifdef MANIPULATION
            manipulation_client_.move_to_named_configuration("wave");
            #endif
            last_action = action;
            last_time = this_time;
        }
        return NodeStatus::SUCCESS;
    }
    if (action == "shake"){
        if(last_action != action || (this_time - last_time) > 3){
            #ifdef MANIPULATION
            manipulation_client_.move_to_named_configuration("shake");
            #endif
            last_action = action;
            last_time = this_time;
        }
        return NodeStatus::SUCCESS;
    }
    if (action == "stop"){
        if(last_action != action || (this_time - last_time) > 3){
            #ifdef MANIPULATION
            manipulation_client_.stop();
            #endif
            last_action = action;
            last_time = this_time;
        }
        return NodeStatus::SUCCESS;
    }
    last_action = -1;
    return NodeStatus::FAILURE;
}


PortsList DoResponseAction::providedPorts()
{
    return {InputPort<std::string>("action")};
}
