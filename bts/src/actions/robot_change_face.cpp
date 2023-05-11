#include <behaviortree_cpp_v3/action_node.h>
#include "robot_change_face.h"
#include "common.h"
#include <chrono>
#include <thread>
#include <unistd.h>
#include <yarp/os/Network.h>

using yarp::os::Network;
using yarp::os::Bottle;


RobotChangeFace::RobotChangeFace(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool RobotChangeFace::init(std::string){
    #ifdef EMOTIONS
    port.open("/BT/emotions/out");
    #ifdef ICUB
    Network::connect("/BT/emotions/out","/icub/face/emotions/in");
    #endif
    #ifdef ECUB
    Network::connect("/BT/emotions/out","/ergoCubEmotions/rpc");
    #endif
    #endif
    last_sent_emotion = "";
    return true;
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
    #ifdef EMOTIONS
    Bottle cmd;
    Bottle response;
    #ifdef ICUB
    cmd.addString("set");
    cmd.addString("all");
    #endif
    #ifdef ECUB
    cmd.addString("setEmotion");
    #endif

    if(has_box=="yes" && last_sent_emotion!="has_box"){
        #ifdef ICUB
        cmd.addString("evi");
        #endif
        #ifdef ECUB
        cmd.addString("shy");
        #endif
        last_sent_emotion = "has_box";
    }
    else if(msg=="none" && last_sent_emotion!="none" && has_box=="no"){
        #ifdef ICUB
        cmd.addString("hap");
        #endif
        #ifdef ECUB
        cmd.addString("neutral");
        #endif
        last_sent_emotion = "none";
    }
    else if(msg=="face" && last_sent_emotion!="face" && has_box=="no"){
        #ifdef ICUB
        cmd.addString("hap");
        #endif
        #ifdef ECUB
        cmd.addString("happy");
        #endif
        last_sent_emotion = "face";
    }
    else if(msg=="object" && last_sent_emotion!="object" && has_box=="no"){
        #ifdef ICUB
        cmd.addString("evi");
        #endif
        #ifdef ECUB
        cmd.addString("alert");
        #endif
        last_sent_emotion = "object";
    }

    if (port.write(cmd, response)) {
        cout << response.toString() << endl;
    }
    #endif
    return NodeStatus::SUCCESS;
}

PortsList RobotChangeFace::providedPorts()
{
    return {InputPort<std::string>("poi"),
            InputPort<std::string>("has_box")};
}
