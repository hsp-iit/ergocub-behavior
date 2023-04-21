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
    Network::connect("/BT/emotions/out","/icub/face/emotions/in");
    #endif
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

    // change face depending on object
    std::string emotion = "";
    if(msg=="none"){
        emotion = "neu";
    }
    else if(msg=="face"){
        emotion = "hap";
    }
    else if(msg=="object"){
        emotion = "ang";
    }

    Bottle cmd;
    cmd.addString("set");
    cmd.addString("all");
    cmd.addString(emotion);
    Bottle response;
    #ifdef EMOTIONS
    if (port.write(cmd, response)) {
        cout << response.toString() << endl;
    }
    #endif
    return NodeStatus::SUCCESS;
}

PortsList RobotChangeFace::providedPorts()
{
    return {InputPort<std::string>("poi")};
}
