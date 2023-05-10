#pragma once


#include <behaviortree_cpp_v3/action_node.h>

#include <string>
#include <future>
#include <yarp/os/RpcClient.h>


using namespace BT;
using namespace std;

class RobotChangeFace :  public SyncActionNode
{
public:
    RobotChangeFace(string name, const NodeConfiguration &config);
    //void halt() override;
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    bool init(std::string);
    bool is_ok_{false};
    yarp::os::RpcClient port;
    std::string last_sent_emotion;
};
