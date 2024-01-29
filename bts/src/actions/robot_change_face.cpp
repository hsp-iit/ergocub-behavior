#include <behaviortree_cpp_v3/action_node.h>
#include "robot_change_face.h"
#include "common.h"
#include <chrono>
#include <thread>
#include <unistd.h>
#include <yarp/os/Network.h>

using yarp::os::Bottle;


RobotChangeFace::RobotChangeFace(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    SyncActionNode(name, nc),
    bt_config(bt_config)
{
    std::string server_name =  bt_config.get<std::string>("components.emotions.port");
    std::string client_name = "/BT/" + name + server_name;

    client_port.open(client_name);

    while (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    ecub_emotions_client.yarp().attachAsClient(client_port);

    last_sent_emotion = "";
}

NodeStatus RobotChangeFace::tick()
{
    Optional<std::string> msg = getInput<std::string>("poi");
    if (!msg)
    {
      throw BT::RuntimeError("missing required input [message]: ",
                              msg.error() );
    }

    Optional<std::string> has_box = getInput<std::string>("has_box");
    if (!has_box)
    {
      throw BT::RuntimeError("missing required input [message]: ",
                              msg.error() );
    }

    // change face depending on object

    if(has_box=="yes" && last_sent_emotion!="has_box"){
        ecub_emotions_client.setEmotion("shy");
        last_sent_emotion = "has_box";
    }
    else if(msg=="none" && last_sent_emotion!="none" && has_box=="no"){
        ecub_emotions_client.setEmotion("neutral");
        last_sent_emotion = "none";
    }
    else if(msg=="face" && last_sent_emotion!="face" && has_box=="no"){
        ecub_emotions_client.setEmotion("happy");
        last_sent_emotion = "face";
    }
    else if(msg=="object" && last_sent_emotion!="object" && has_box=="no"){
        ecub_emotions_client.setEmotion("alert");
        last_sent_emotion = "object";
    }

    return NodeStatus::SUCCESS;
}

PortsList RobotChangeFace::providedPorts()
{
    return {InputPort<std::string>("poi"),
            InputPort<std::string>("has_box")};
}
