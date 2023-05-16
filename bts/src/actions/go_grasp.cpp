#include <behaviortree_cpp_v3/action_node.h>

#include "go_grasp.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoGrasp::GoGrasp(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    StatefulActionNode(name, nc),
    bt_config(bt_config)
{
    std::string manipulation_server_name =  bt_config.get<std::string>("components.manipulation.port");
    std::string manipulation_client_name = "/BT/" + name + manipulation_server_name;

    manipulation_client_port.open(manipulation_client_name);

    while (!yarp.connect(manipulation_client_name, manipulation_server_name))
    {
        std::cout << "Error! Could not connect to server " << manipulation_server_name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    manipulation_client_.yarp().attachAsClient(manipulation_client_port);

    // PERCEPTION
    std::string perception_server_name =  bt_config.get<std::string>("components.perception.port");
    std::string perception_client_name = "/BT/" + name + manipulation_server_name;

    perception_client_port.open(perception_client_name);

    while (!yarp.connect(perception_client_name, perception_server_name))
    {
        std::cout << "Error! Could not connect to server " << perception_server_name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    ecub_perception_client_.yarp().attachAsClient(perception_client_port);
}


NodeStatus GoGrasp::onStart()
{
    #ifdef MANIPULATION

    auto hand_poses = ecub_perception_client_.get_poses();
    manipulation_client_.move_hands_to_pose(hand_poses[0], hand_poses[1], 5.0);

    // manipulation_client_.grasp();

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
