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


NodeStatus GoGrasp::onStart()
{
    #ifdef MANIPULATION
    std::cout << "CIAOOOOOOOOO" << std::endl;
    manipulation_client_.perform_cartesian_action("testgrasp");
    #endif
    return NodeStatus::RUNNING;
}

NodeStatus GoGrasp::onRunning(){
    #ifdef MANIPULATION
    if (manipulation_client_.is_finished()){
        setOutput<std::string>("has_box", "yes");
        manipulation_client_.grasp();
        return NodeStatus::SUCCESS;
    }
    return NodeStatus::RUNNING;
    #endif
    return NodeStatus::SUCCESS;
}

void GoGrasp::onHalted(){
    std::cout << "ALTATO" << std::endl;
    manipulation_client_.perform_joint_space_action("home");
    return;
}


PortsList GoGrasp::providedPorts()
{
    return {OutputPort<std::string>("has_box")};
}
