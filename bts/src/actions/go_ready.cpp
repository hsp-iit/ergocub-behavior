#include <behaviortree_cpp_v3/action_node.h>

#include "go_ready.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoReady::GoReady(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool GoReady::init(std::string name)
{
    // MANIPULATION
    #ifdef REAL_ROBOT
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

NodeStatus GoReady::tick()
{
  // Here I simulate with sleep, this way should work, but the robot cannot be interrupted
//  manipulation_client_.ready(true);
  // std::this_thread::sleep_for( std::chrono::milliseconds(5000) );
  return NodeStatus::SUCCESS;
 }


PortsList GoReady::providedPorts()
{
    return {};
}
