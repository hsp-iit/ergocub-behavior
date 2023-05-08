#pragma once


#include <behaviortree_cpp_v3/action_node.h>
#include <ManipulationInterface.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <vector>
#include <map>

using namespace BT;
using namespace std;

class DoResponseAction :  public SyncActionNode
{
public:
    DoResponseAction(string name, const NodeConfiguration &config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    bool init(std::string);
    CommandInterface manipulation_client_;
    bool is_ok_{false};
    yarp::os::Network yarp;
    std::string last_sent_command;
    yarp::os::Port man_client_port;
};
