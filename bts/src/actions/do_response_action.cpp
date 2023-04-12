#include <behaviortree_cpp_v3/action_node.h>

#include "do_response_action.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>


DoResponseAction::DoResponseAction(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool DoResponseAction::init(std::string name)
{
    #ifdef REAL_ROBOT
    // Connect to manipulation
    std::string man_server_name = "/Components/Manipulation"s;
    std::string man_client_name = "/BT/" + name + "/Manipulation"s;

    man_client_port.open(man_client_name);

    if (!yarp.connect(man_client_name,man_server_name))
    {
        throw BT::RuntimeError("Error! Could not connect to server ", man_server_name);
    }
    manipulation_client_.yarp().attachAsClient(man_client_port);
    #endif

    // Connect to action recognition
    std::string ar_server_name = "/Components/ActionRecognition"s;
    std::string ar_client_name = "/BT/" + name + "/ActionRecognition/DoResponseAction"s;

    ar_client_port.open(ar_client_name);

    if (!yarp.connect(ar_client_name,ar_server_name))
    {
        throw BT::RuntimeError("Error! Could not connect to server ", ar_server_name);
    }
    action_recognition_client_.yarp().attachAsClient(ar_client_port);

    // Others
    last_action = 0;
    auto now = std::chrono::system_clock::now();
    last_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() - 4;
    return true;
}

NodeStatus DoResponseAction::tick()
{

    int action = action_recognition_client_.get_action();
    auto now = std::chrono::system_clock::now();
    long this_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    std::cout << "received action " << action << std::endl;
    std::cout << "last action " << last_action << std::endl;

    if(action==1){
        #ifdef REAL_ROBOT
        // Check if this action is different w.r.t. last one and more than 3 seconds have passed
        if(last_action != action && (this_time - last_time) > 3){
            manipulation_client_.wave(false);
            last_action = action;
            last_time = this_time;
        }
        #endif
        return NodeStatus::SUCCESS;
    }
    else if(action==2 || action==9){  // TODO REMOVE
        #ifdef REAL_ROBOT
        if(last_action != action && (this_time - last_time) > 3){
            manipulation_client_.shake(false);
            last_action = action;
            last_time = this_time;
        }
        return NodeStatus::SUCCESS;
        #endif
    }
    else if(action==5){
        #ifdef REAL_ROBOT
        if(last_action != action && (this_time - last_time) > 3){
            manipulation_client_.stop();
            last_action = action;
            last_time = this_time;
        }
        #endif
        return NodeStatus::SUCCESS;
    }
    else if(action==-2){
        if(last_action == 1 || last_action == 2 || last_action == 9 || last_action == 5)
            return NodeStatus::SUCCESS;
        return NodeStatus::FAILURE;
    }
    last_action = -1;
    return NodeStatus::FAILURE;
}


PortsList DoResponseAction::providedPorts()
{
    return {};
}
