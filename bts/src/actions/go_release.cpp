#include <behaviortree_cpp_v3/action_node.h>

#include "go_release.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

GoRelease::GoRelease(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    SyncActionNode(name, nc),
    bt_config(bt_config)
{
    // MANIPULATION
    std::string server_name =  bt_config.get<std::string>("components.manipulation.port");
    std::string client_name = "/BT/" + name + server_name;

    client_port.open(client_name);

    while (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    manipulation_client_.yarp().attachAsClient(client_port);
}

NodeStatus GoRelease::tick()
{
    #ifdef MANIPULATION   
    manipulation_client_.perform_joint_space_action("ready");
    #endif
    setOutput<std::string>("has_box", "no");
    return NodeStatus::SUCCESS;
}


PortsList GoRelease::providedPorts()
{
    return {OutputPort<std::string>("has_box")};
}
