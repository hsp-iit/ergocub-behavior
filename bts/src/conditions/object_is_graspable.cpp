#include <behaviortree_cpp_v3/condition_node.h>
#include "object_is_graspable.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <cmath>

#include <iostream>

using std::pow;


ObjectIsGraspable::ObjectIsGraspable(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool ObjectIsGraspable::init(std::string name)
{
    std::string server_name = "/eCubPerception/rpc:i"s;
    std::string client_name = "/BT/" + name + "/eCubPerception"s;

    client_port.open(client_name);

    if (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        return false;
    }
    ecub_perception_client_.yarp().attachAsClient(client_port);
    return true;
}

NodeStatus ObjectIsGraspable::tick()
{
    std::vector<yarp::sig::Matrix> hand_poses = ecub_perception_client_.get_poses();
    yarp::sig::Matrix right = hand_poses[0];
    yarp::sig::Matrix left = hand_poses[1];

    double px, py, pz, cx, cy, cz, rx, ry, rz;

    // Check right hand
    px = right[0][3];  // distance 
    py = right[1][3];  // left-right
    pz = right[2][3];  // height

    std::cout << "right hand" << px << " " << py << " " << pz << std::endl;

    if(px==-1 && py==-1 && pz==-1){
        return BT::NodeStatus::FAILURE;
    }
    cx = 0.60;  // distance
    cy = -0.15;  // left-right
    cz = 0.15;  //height
    rx = 0.1;
    ry = 0.1;
    rz = 0.1;
    if ((pow(px-cx, 2)/rx) + (pow(py-cy, 2)/ry) + (pow(pz-cz, 2)/rz) > 1){
        return BT::NodeStatus::FAILURE;
    }

    // Check left hand
    px = left[0][3];  // distance 
    py = left[1][3];  // left-right
    pz = left[2][3];  // height
    std::cout << "left hand" << px << " " << py << " " << pz << std::endl;
    if(px==-1 && py==-1 && pz==-1){
        return BT::NodeStatus::FAILURE;
    }
    cx = 0.60;  // distance
    cy = 0.15;  // left-right
    cz = 0.15;  //height
    rx = 0.1;
    ry = 0.1;
    rz = 0.1;
    if ((pow(px-cx, 2)/rx) + (pow(py-cy, 2)/ry) + (pow(pz-cz, 2)/rz) > 1){
        return BT::NodeStatus::FAILURE;
    }

    return BT::NodeStatus::SUCCESS;

    // if(obj_pos[0]>-0.50 && obj_pos[0]<-0.20 &&  // between 25 and 45 centimeter distant from torso
    //    obj_pos[1]>-0.10 && obj_pos[1]<0.10 &&  // 5 centemiters on left or right
    //    obj_pos[2]>-0.10 && obj_pos[2]<0.40
    // )
    //     return BT::NodeStatus::SUCCESS;
    // else
    //     return BT::NodeStatus::FAILURE;


    // std::cout << distance << std::endl;
    // if (distance == -1 or distance == 0 or distance >= threshold){
    //     return BT::NodeStatus::FAILURE;
    // }
    // return  BT::NodeStatus::SUCCESS;
}

PortsList ObjectIsGraspable::providedPorts()
{
    return {};
}
