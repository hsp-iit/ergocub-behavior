#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>
#include <vector>

#include <behaviortree_cpp_v3/action_node.h>
#include <yarp/sig/Matrix.h>

#include "go_ready.h"
#include "common.h"

GoReady::GoReady(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    StatefulActionNode(name, nc),
    bt_config(bt_config)
{
    // MANIPULATION
    std::string manipulation_server_name =  bt_config.get<std::string>("components.manipulation.port");
    std::string manipulation_client_name = "/BT/" + name + manipulation_server_name;

    manipulation_client_port.open(manipulation_client_name);

    while (!yarp.connect(manipulation_client_name,manipulation_server_name))
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



NodeStatus GoReady::onStart()
{
    #ifdef MANIPULATION
    manipulation_client_.perform_joint_space_action("ready");
    ready = false;
    #endif
    return NodeStatus::RUNNING;
}

NodeStatus GoReady::onRunning(){
    // while (true){
    //         std::vector<yarp::sig::Matrix> hand_poses = ecub_perception_client_.get_poses();
    //         yarp::sig::Matrix left = hand_poses[0].transposed();
    //         yarp::sig::Matrix right = hand_poses[1].transposed();
    //         for (int i = 0; i < 4; i++){
    //             for (int j = 0; j < 4; j++){
    //                 std::cout << left[i][j] << " ";
    //             }
    //             std::cout << std::endl;
    //         }
    //     }

    #ifdef MANIPULATION
    if (manipulation_client_.is_finished() && !ready){
        ready = true;
        std::vector<yarp::sig::Matrix> hand_poses = ecub_perception_client_.get_poses();
        yarp::sig::Matrix right = hand_poses[0];
        yarp::sig::Matrix left = hand_poses[1];
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                std::cout << left[i][j] << " ";
            }
            std::cout << std::endl;
        }
        // manipulation_client_.move_hands_to_pose(left, right, 5.0);
        manipulation_client_.perform_cartesian_action("testgrasp");
        return NodeStatus::RUNNING;
    }
    if (manipulation_client_.is_finished() && ready){
        ready = false;
        setOutput<std::string>("has_box", "yes");
        // manipulation_client_.grasp();
        return NodeStatus::SUCCESS;
    }
    return NodeStatus::RUNNING;
    #endif
    return NodeStatus::SUCCESS;
}

void GoReady::onHalted(){
    manipulation_client_.perform_joint_space_action("home");
    return;
}


PortsList GoReady::providedPorts()
{
    return {OutputPort<std::string>("has_box")};
}
