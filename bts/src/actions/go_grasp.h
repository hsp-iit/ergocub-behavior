#pragma once


#include <behaviortree_cpp_v3/action_node.h>
#include <CommandInterface.h>
#include <eCubPerceptionInterface/eCubPerceptionInterface.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

using namespace BT;
using namespace std;
#include <boost/property_tree/ptree.hpp>
namespace pt = boost::property_tree;

class GoGrasp :  public StatefulActionNode
{
public:
    GoGrasp(string name, const NodeConfiguration &nc, pt::ptree bt_config);
    NodeStatus onStart() override;
    NodeStatus onRunning() override;
    void onHalted() override;
    static PortsList providedPorts();
private:
    pt::ptree bt_config;
    CommandInterface manipulation_client_;
    eCubPerceptionInterface ecub_perception_client_;
    yarp::os::Network yarp;
    yarp::os::Port manipulation_client_port;
    yarp::os::Port perception_client_port;
    bool ready;
};
