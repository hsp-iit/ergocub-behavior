#include <behaviortree_cpp_v3/action_node.h>

#include "move_box.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

MoveBox::MoveBox(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool MoveBox::init(std::string name)
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

NodeStatus MoveBox::tick()
{

    std::cout << "##########################################################################################################" << std::endl;
    // Read action
    Optional<std::string> msg = getInput<std::string>("action");
    if (!msg)
    {
      throw BT::RuntimeError("missing required input [message]: ",
                              msg.error() );
    }

    std::string action = msg.value();
    std::cout << action << std::endl << std::endl << std::endl << std::endl << std::endl;

    #ifdef MANIPULATION
    if(action=="up"){
        manipulation_client_.perform_joint_space_action("up");
    }
    else if (action=="down"){
        manipulation_client_.perform_joint_space_action("down");
    }
    else if (action=="right"){
        manipulation_client_.perform_joint_space_action("right");
    }
    else if (action=="left"){
        manipulation_client_.perform_joint_space_action("left");
    }
    #endif
    return NodeStatus::SUCCESS;
}


PortsList MoveBox::providedPorts()
{
    return {InputPort<std::string>("action")};
}
