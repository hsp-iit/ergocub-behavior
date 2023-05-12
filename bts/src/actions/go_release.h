#pragma once


#include <behaviortree_cpp_v3/action_node.h>
#include <ManipulationInterface.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

using namespace BT;
using namespace std;
#include <boost/property_tree/ptree.hpp>
namespace pt = boost::property_tree;

class GoRelease :  public SyncActionNode
{
public:
    GoRelease(string name, const NodeConfiguration &nc, pt::ptree bt_config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    pt::ptree bt_config;
    CommandInterface manipulation_client_;
    yarp::os::Network yarp;
    yarp::os::Port client_port;
};
