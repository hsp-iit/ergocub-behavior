#include <behaviortree_cpp_v3/action_node.h>

#include "go_home.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoHome::GoHome(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    SyncActionNode(name, nc),
    bt_config(bt_config)
{
    std::string server_name =  bt_config.get<std::string>("components.manipulation.port");
    std::string client_name = "/BT/" + name + server_name;

    client_port.open(client_name);

    while (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    manipulation_client_.yarp().attachAsClient(client_port);

    auto now = std::chrono::system_clock::now();
    last_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() - 4;
}

NodeStatus GoHome::tick()
{
    auto now = std::chrono::system_clock::now();
    long this_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    if((this_time - last_time) > 3){
        #ifdef MANIPULATION
        std::cout << "Before" << std::endl;
        manipulation_client_.perform_joint_space_action("home");
        std::cout << "asfter" << std::endl;
        #endif
        last_time = this_time;
        return NodeStatus::SUCCESS;
    }
    return NodeStatus::FAILURE;
}


PortsList GoHome::providedPorts()
{
    return {};
}
