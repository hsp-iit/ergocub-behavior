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
    last_time = 0;

    return true;
}

NodeStatus DoResponseAction::tick()
{

    int action = action_recognition_client_.get_action();
    auto now = std::chrono::system_clock::now();
    long this_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    if(action==-1 || action==0){
        std::cout << "no action detected" << action << std::endl;
        return NodeStatus::FAILURE;
    }
    else if(action==0){
        std::cout << "detected stand" << std::endl;
    }
    else if(action==1){
        std::cout << "detected hello" << std::endl;
        #ifdef REAL_ROBOT
        // Check if this action is different w.r.t. last one and more than 3 seconds have passed
        if(last_action != action && (this_time - last_time) > 3){
            std::cout << "sent hello" << std::endl;
            manipulation_client_.wave(false);
            last_action = action;
            last_time = this_time;
        }
        #endif
    }
    else if(action==2){
        std::cout << "detected handshake" << std::endl;
        #ifdef REAL_ROBOT
        if(last_action != action && (this_time - last_time) > 3){
            manipulation_client_.shake(false);
            last_action = action;
            last_time = this_time;
        }
        #endif
    }
    else if(action==3){
        std::cout << "detected lift" << std::endl;
    }
    else if(action==4){
        std::cout << "detected get" << std::endl;
    }
    else if(action==5){
        std::cout << "detected stop" << std::endl;
        #ifdef REAL_ROBOT
        if(last_action != action && (this_time - last_time) > 3){
            manipulation_client_.stop();
            last_action = action;
            last_time = this_time;
        }
        #endif
    }
    else {
        std::cout << "detected unknown action" << std::endl;
    }
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    return NodeStatus::SUCCESS;
}


PortsList DoResponseAction::providedPorts()
{
    return {};
}
