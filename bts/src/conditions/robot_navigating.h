#pragma once


#include <behaviortree_cpp_v3/condition_node.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <eCubPerceptionInterface/eCubPerceptionInterface.h>

using namespace BT;
using namespace std;
#include <boost/property_tree/ptree.hpp>
namespace pt = boost::property_tree;

class RobotNavigating :  public ConditionNode
{
public:
    RobotNavigating(string name, const NodeConfiguration &nc, pt::ptree bt_config);
    NodeStatus tick() override;
private:
    pt::ptree bt_config;
    yarp::os::Network yarp;

    std::string reader_name;
    std::string writer_name;
    yarp::os::BufferedPort<yarp::os::Bottle> reader_port;

    bool is_navigating = false;
};
