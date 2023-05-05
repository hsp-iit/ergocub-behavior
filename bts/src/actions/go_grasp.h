#pragma once


#include <behaviortree_cpp_v3/action_node.h>
#include <ManipulationInterface.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

using namespace BT;
using namespace std;

class GoGrasp :  public SyncActionNode
{
public:
    GoGrasp(string name, const NodeConfiguration &config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    bool init(std::string);
    CommandInterface manipulation_client_;
    bool is_ok_{false};
    yarp::os::Network yarp;
    yarp::os::Port client_port;
};
