#include <behaviortree_cpp_v3/action_node.h>

#include "do_response_action.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>


DoResponseAction::DoResponseAction(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    SyncActionNode(name, nc),
    bt_config(bt_config)
{
    // connect to perception
    std::string per_server_name =  bt_config.get<std::string>("components.perception.port");
    std::string per_client_name = "/BT/" + name + per_server_name;

    per_client_port.open(per_client_name);

    if (!yarp.connect(per_client_name,per_server_name))
    {
        std::cout << "Error! Could not connect to server " << per_server_name << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    ecub_perception_client_.yarp().attachAsClient(per_client_port);

    // Connect to manipulation
    std::string server_name =  bt_config.get<std::string>("components.manipulation.port");
    std::string client_name = "/BT/" + name + server_name;
    m_posePortName = bt_config.get<std::string>("components.human_position.port");
    std::string client_pose_name = "/BT/" + m_posePortName;

    man_client_port.open(client_name);
    m_posePort.open(client_pose_name);

    while (!yarp.connect(client_name, server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    if (!yarp.connect(client_pose_name, m_posePortName))
    {
        std::cout << "Error! Could not connect to server " << m_posePortName << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    manipulation_client_.yarp().attachAsClient(man_client_port);

    last_sent_command = "";
    was_releasing = false;
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

    // CHECK IF AN ACTION IS IN EXECUTION
    auto fin = manipulation_client_.is_finished();
    std::cout << "DEBUGGING DO RESPONSE ACTION " << std::endl << std::endl << std::endl << std::endl;
    std::cout << "fin: " <<  fin << std::endl;
    std::cout << "action: " << action << std::endl;
    std::cout << "focus: " << focus << std::endl;
    std::cout << "has_box: " <<  has_box << std::endl;
    std::cout << "last_sent_command: " <<  last_sent_command << std::endl;

    if(fin){
        //  COME ACTION TODO add estimated focus
        if(action == "come"){
            yarp::os::Bottle & b = m_posePort.prepare();
            auto human_position = ecub_perception_client_.get_human_position();
            b.clear();
            b.addFloat64(human_position(0));
            b.addFloat64(human_position(1));
            b.addFloat64(human_position(2));
            m_posePort.write();
        }

        // HAS_BOX COMMANDS
        if(has_box == "yes"){
            if(was_releasing){
                // manipulation_client_.release_object();
                setOutput<std::string>("has_box_out", "no");
                was_releasing = false;
            }
            if(action == "release"){
                if(last_sent_command != "release" && focus == "yes"){
                    // manipulation_client_.release_object();
                    // manipulation_client_.perform_cartesian_action("out");
                    manipulation_client_.perform_joint_space_action("ready");
                    was_releasing = true;
                    last_sent_command = action;
                    return NodeStatus::SUCCESS;
                }
                else{
                    return NodeStatus::FAILURE;
                }
            }
            // if(action == "up" || action == "down" || action == "left" || action == "right" || action == "forward" || action == "back"){
            //     if(last_sent_command != action && focus == "yes"){
            //         manipulation_client_.perform_cartesian_action(action);
            //         last_sent_command = action;
            //         return NodeStatus::SUCCESS;
            //     }
            //     else{
            //         return NodeStatus::FAILURE;
            //     }
            // }
        }
        // HRI COMMANDS
        else{
            // ACTIONS
            if(action == "wave" || action == "shake" || action == "t_pose"){
                if(last_sent_command != action && focus == "yes"){
                    manipulation_client_.perform_joint_space_action(action);
                    last_sent_command = action;
                    return NodeStatus::SUCCESS;
                }
                else{
                    return NodeStatus::FAILURE; 
                }
            }
            // GO HOME
            else if(last_sent_command != "home"){
                manipulation_client_.perform_joint_space_action("home");
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
    return {InputPort<std::string>("action"), InputPort<std::string>("focus"), InputPort<std::string>("has_box_in"), OutputPort<std::string>("has_box_out")};
}
