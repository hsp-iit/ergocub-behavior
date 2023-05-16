#pragma once


#include <behaviortree_cpp_v3/action_node.h>
#include <CommandInterface.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <vector>
#include <map>

using namespace BT;
using namespace std;
#include <boost/property_tree/ptree.hpp>
namespace pt = boost::property_tree;

class DoResponseAction :  public SyncActionNode
{
public:
    DoResponseAction(string name, const NodeConfiguration &nc, pt::ptree bt_config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    CommandInterface manipulation_client_;
    pt::ptree bt_config;
    yarp::os::Network yarp;
    std::string last_sent_command;
    yarp::os::Port man_client_port;
    bool was_releasing;
};
