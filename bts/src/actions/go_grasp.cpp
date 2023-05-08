#include <behaviortree_cpp_v3/action_node.h>

#include "go_grasp.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoGrasp::GoGrasp(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool GoGrasp::init(std::string name)
{
    // MANIPULATION
    #ifdef MANIPULATION
    std::string manipulation_server_name = "/Components/Manipulation"s;
    std::string manipulation_client_name = "/BT/" + name + "/Manipulation"s;

    manipulation_client_port.open(manipulation_client_name);

    if (!yarp.connect(manipulation_client_name,manipulation_server_name))
    {
        throw BT::RuntimeError("Error! Could not connect to server ", manipulation_server_name);
    }
    manipulation_client_.yarp().attachAsClient(manipulation_client_port);
    #endif

    // PERCEPTION
    std::string perception_server_name = "/eCubPerception/rpc:i"s;
    std::string perception_client_name = "/BT/" + name + "/eCubPerception"s;

    perception_client_port.open(perception_client_name);

    if (!yarp.connect(perception_client_name,perception_server_name))
    {
        std::cout << "Error! Could not connect to server " << perception_server_name << '\n';
        return false;
    }
    ecub_perception_client_.yarp().attachAsClient(perception_client_port);
    return true;
}

NodeStatus GoGrasp::tick(){
    #ifdef MANIPULATION
    auto hand_poses = ecub_perception_client_.get_poses();
    manipulation_client_.move_hands_to_pose(hand_poses[0], hand_poses[1], 5.0);
    #endif
    setOutput<std::string>("has_box", "yes");
    return NodeStatus::SUCCESS;
}


PortsList GoGrasp::providedPorts()
{
    return {OutputPort<std::string>("has_box")};
}
