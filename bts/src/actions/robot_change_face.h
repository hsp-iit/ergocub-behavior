#pragma once


#include <behaviortree_cpp_v3/action_node.h>
#include <ergoCubEmotions_IDL.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;
using namespace BT;
using namespace std;

class RobotChangeFace :  public SyncActionNode
{
public:
    RobotChangeFace(string name, const NodeConfiguration &nc, pt::ptree bt_config);
    //void halt() override;
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    ergoCubEmotions_IDL ecub_emotions_client;
    pt::ptree bt_config;
    yarp::os::Network yarp;
    yarp::os::Port client_port;
    std::string last_sent_emotion;
};
