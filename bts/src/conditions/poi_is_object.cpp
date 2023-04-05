#include <behaviortree_cpp_v3/condition_node.h>
#include "poi_is_object.h"


POIIsObject::POIIsObject(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool POIIsObject::init(std::string name)
{
    return true;
}

NodeStatus POIIsObject::tick()
{
    Optional<std::string> msg = getInput<std::string>("poi");
    if(msg == "object")
        return  BT::NodeStatus::SUCCESS;
    else
        return BT::NodeStatus::FAILURE;
}

PortsList POIIsObject::providedPorts()
{
    return {InputPort<std::string>("poi")};
}
