#include <vector>
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;
using namespace BT;
#pragma once

bool are_all_elements_minus_one(const std::vector<double>& vec);
bool are_all_elements_zero(const std::vector<double>& vec);

// Templates needs to be instantiated in the header file
template <class T>
BT::NodeBuilder build_node (boost::property_tree::ptree config) {
  return [config](const std::string& name, const NodeConfiguration& nc)
    {
        return std::make_unique<T>(name, nc, config);
    };
}

template <class T>
std::vector<T> as_vector(pt::ptree const& pt, pt::ptree::key_type const& key)
{
    std::vector<T> r;
    for (auto& item : pt.get_child(key))
        r.push_back(item.second.get_value<T>());
    return r;
};
