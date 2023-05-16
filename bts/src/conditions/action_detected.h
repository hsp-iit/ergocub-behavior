#pragma once


#include <behaviortree_cpp_v3/condition_node.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <eCubPerceptionInterface/eCubPerceptionInterface.h>

using namespace BT;
using namespace std;
#include <boost/property_tree/ptree.hpp>
namespace pt = boost::property_tree;

class ActionDetected :  public ConditionNode
{
public:
    ActionDetected(string name, const NodeConfiguration &nc, pt::ptree bt_config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    eCubPerceptionInterface ecub_perception_client_;
    pt::ptree bt_config;
    yarp::os::Network yarp;
    yarp::os::Port client_port;
};
