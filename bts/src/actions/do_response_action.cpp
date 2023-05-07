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
    #ifdef MANIPULATION
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

    last_sent_command = "";
    return true;
}

NodeStatus DoResponseAction::tick()
{

    // READ MBLACKBOARD
    Optional<std::string> msg1 = getInput<std::string>("action");
    if (!msg1)
    {
        throw BT::RuntimeError("missing required input [message]: ", msg1.error() );
    }
    std::string action = msg1.value();

    Optional<std::string> msg2 = getInput<std::string>("focus");
    if (!msg2)
    {
        throw BT::RuntimeError("missing required input [message]: ", msg2.error() );
    }
    std::string focus = msg2.value();

    Optional<std::string> msg3 = getInput<std::string>("has_box_in");
    if (!msg3)
    {
        throw BT::RuntimeError("missing required input [message]: ", msg3.error() );
    }
    std::string has_box = msg3.value();
    
    // std::cout << "received action " << action << std::endl;
    // std::cout << "last_sent_command " << last_sent_command << std::endl;

    // CHECK IF AN ACTION IS IN EXECUTION
    auto fin = manipulation_client_.is_finished();
    std::cout << "DEBUGGING DO RESPONSE ACTION " << std::endl << std::endl << std::endl << std::endl;
    std::cout << "fin: " <<  fin << std::endl;
    std::cout << "action: " << action << std::endl;
    std::cout << "focus: " << focus << std::endl;
    std::cout << "has_box: " <<  has_box << std::endl;
    std::cout << "last_sent_command: " <<  last_sent_command << std::endl;

    if(fin){

        // HAS_BOX COMMANDS
        if(has_box == "yes"){
            if(action == "release"){
                if(last_sent_command != "release"){
                    #ifdef MANIPULATION
                    manipulation_client_.release_object();
                    manipulation_client_.perform_joint_space_action("ready");
                    #endif
                    setOutput<std::string>("has_box_out", "no");
                    last_sent_command = action;
                    return NodeStatus::SUCCESS;
                }
                else{
                    return NodeStatus::FAILURE;
                }
                    
            }
        }
        // HRI COMMANDS
        else{
            // ACTIONS
            if(action == "wave"){
                if(last_sent_command != "wave" && focus == "yes"){
                    #ifdef MANIPULATION
                    manipulation_client_.perform_joint_space_action(action);
                    #endif
                    last_sent_command = action;
                    return NodeStatus::SUCCESS;
                }
                else{
                    return NodeStatus::FAILURE; 
                }
            }
            else if(action == "shake"){
                if(last_sent_command != "shake" && focus == "yes"){
                    #ifdef MANIPULATION
                    manipulation_client_.perform_joint_space_action(action);
                    #endif
                    last_sent_command = action;
                    return NodeStatus::SUCCESS;
                }
                else{
                    return NodeStatus::FAILURE;
                }
            }
            // GO HOME
            else if(last_sent_command != "home"){
                #ifdef MANIPULATION
                manipulation_client_.perform_joint_space_action("home");
                #endif
                last_sent_command = "home";
                return NodeStatus::SUCCESS;
            }
            return NodeStatus::FAILURE;
        }
    }
    return NodeStatus::FAILURE;
}


PortsList DoResponseAction::providedPorts()
{
    return {InputPort<std::string>("action"), InputPort<std::string>("focus"), InputPort<std::string>("has_box_in"), InputPort<std::string>("has_box_out")};
}
