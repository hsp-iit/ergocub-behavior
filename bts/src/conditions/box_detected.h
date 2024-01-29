#pragma once


#include <behaviortree_cpp_v3/condition_node.h>
#include <eCubPerceptionInterface/eCubPerceptionInterface.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

using namespace BT;
using namespace std;
#include <boost/property_tree/ptree.hpp>
namespace pt = boost::property_tree;

class BoxDetected :  public ConditionNode
{
public:
    BoxDetected(string name, const NodeConfiguration &nc, pt::ptree bt_config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    eCubPerceptionInterface ecub_perception_client_;
    pt::ptree bt_config;
    yarp::os::Network yarp;
    yarp::os::Port client_port;
};
