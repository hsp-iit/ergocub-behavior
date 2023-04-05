#include <behaviortree_cpp_v3/condition_node.h>
#include "poi_is_face.h"


POIIsFace::POIIsFace(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool POIIsFace::init(std::string name)
{
    return true;
}

NodeStatus POIIsFace::tick()
{
    Optional<std::string> msg = getInput<std::string>("poi");
    if(msg == "face")
        return  BT::NodeStatus::SUCCESS;
    else
        return BT::NodeStatus::FAILURE;
}

PortsList POIIsFace::providedPorts()
{
    return {InputPort<std::string>("poi")};
}
