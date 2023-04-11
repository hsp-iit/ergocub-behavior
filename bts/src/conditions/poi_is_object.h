#pragma once


#include <behaviortree_cpp_v3/condition_node.h>

using namespace BT;
using namespace std;

class POIIsObject :  public ConditionNode
{
public:
    POIIsObject(string name, const NodeConfiguration &config);
    NodeStatus tick() override;
    static PortsList providedPorts();
private:
    bool init(std::string);
    bool is_ok_{false};
};
