#include <behaviortree_cpp_v3/action_node.h>

#include "is_doing_action.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>

IsDoingAction::IsDoingAction(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
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
}


NodeStatus IsDoingAction::tick()
{
    #ifdef MANIPULATION
    auto fin = manipulation_client_.is_finished();
    if (fin){
        setOutput("is_doing_action", "yes");
    }
    else{
        setOutput("is_doing_action", "no");
    }
    #endif
    return NodeStatus::SUCCESS;
}


PortsList IsDoingAction::providedPorts()
{
    return {OutputPort<std::string>("is_doing_action")};
}
