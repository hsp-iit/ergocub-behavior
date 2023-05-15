#include <behaviortree_cpp_v3/condition_node.h>
#include "object_is_graspable.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <cmath>
#include <iostream>

#include "common.h"

using std::pow;


ObjectIsGraspable::ObjectIsGraspable(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    ConditionNode(name, nc),
    bt_config(bt_config)
{
    std::string server_name =  bt_config.get<std::string>("components.perception.port");
    std::string client_name = "/BT/" + name + server_name;

    client_port.open(client_name);

    if (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    ecub_perception_client_.yarp().attachAsClient(client_port);

    r_ellipse_center = as_vector<float>(bt_config, "nodes.is_graspable.r_ellipse_center");
    r_ellipse_radius = as_vector<float>(bt_config, "nodes.is_graspable.r_ellipse_radius");
    l_ellipse_center = as_vector<float>(bt_config, "nodes.is_graspable.l_ellipse_center");
    l_ellipse_radius = as_vector<float>(bt_config, "nodes.is_graspable.l_ellipse_radius");
}


NodeStatus ObjectIsGraspable::tick()
{
    std::vector<yarp::sig::Matrix> hand_poses = ecub_perception_client_.get_poses();
    yarp::sig::Matrix right = hand_poses[0];
    yarp::sig::Matrix left = hand_poses[1];

    if (right[0][3] == -1){
        return BT::NodeStatus::FAILURE;
    }

    // std::cout << "right hand" << right[0][3] << " " << right[1][3] << " " << right[2][3] << std::endl;
    // Check right hand
    if ((pow(right[0][3]-r_ellipse_center[0], 2)/r_ellipse_radius[0]) + 
       (pow(right[1][3]-r_ellipse_center[1], 2)/r_ellipse_radius[1]) + 
       (pow(right[2][3]-r_ellipse_center[2], 2)/r_ellipse_radius[2]) > 1)
    {
        return BT::NodeStatus::FAILURE;
    }

    // std::cout << "left hand" << left[0][3] << " " << left[1][3] << " " << left[2][3] << std::endl;
    // Check left hand
    if ((pow(left[0][3]-l_ellipse_center[0], 2)/l_ellipse_radius[0]) + 
       (pow(left[1][3]-l_ellipse_center[1], 2)/l_ellipse_radius[1]) + 
       (pow(left[2][3]-l_ellipse_center[2], 2)/l_ellipse_radius[2]) > 1)
    {
        return BT::NodeStatus::FAILURE;
    }

    return BT::NodeStatus::SUCCESS;
}

PortsList ObjectIsGraspable::providedPorts()
{
    return {};
}
