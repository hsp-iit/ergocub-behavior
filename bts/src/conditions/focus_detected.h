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

class FocusDetected :  public ConditionNode
{
public:
    FocusDetected(string name, const NodeConfiguration &nc, pt::ptree bt_config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    bool init(std::string);
    pt::ptree bt_config;
    eCubPerceptionInterface ecub_perception_client_;
    bool is_ok_{false};
    yarp::os::Network yarp;
    yarp::os::Port client_port;
};
