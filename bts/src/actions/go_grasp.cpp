#include <behaviortree_cpp_v3/action_node.h>

#include "go_grasp.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoGrasp::GoGrasp(string name, const NodeConfiguration& config) :
    StatefulActionNode(name, config)
{
    is_ok_ = init(name);
}

bool GoGrasp::init(std::string name)
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

NodeStatus GoGrasp::onStart()
{
//  manipulation_client_.grasp(true);
  return NodeStatus::RUNNING;
}

NodeStatus GoGrasp::onRunning(){
    std::cout << "grasp" << std::endl;
    return NodeStatus::SUCCESS;
//    auto fin = manipulation_client_.finished();
//    if (fin == "Si"){
//        return NodeStatus::SUCCESS;
//    }
//    return NodeStatus::RUNNING;
}

void GoGrasp::onHalted(){
    return;
}


PortsList GoGrasp::providedPorts()
{
    return {};
}
