#include <behaviortree_cpp_v3/condition_node.h>
#include "face_detected.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


FaceDetected::FaceDetected(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool FaceDetected::init(std::string name)
{
    std::string server_name = "/Components/eCubPerception"s;
    std::string client_name = "/BT/" + name + "/eCubPerception"s;

    client_port.open(client_name);

    if (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        return false;
    }
    ecub_perception_client_.yarp().attachAsClient(client_port);
    was_true = false;
    return true;
}


NodeStatus FaceDetected::tick()
{
    auto face_position_yarp = ecub_perception_client_.get_face_position();
    std::vector<double> face_position(3);
    for (std::size_t i = 0; i < 3; ++i)
        face_position[i] = face_position_yarp[i];

    if(are_all_elements_minus_one(face_position)){
        setOutput("poi", "none" );
        was_true = false;
        return BT::NodeStatus::FAILURE;
    }
    if(are_all_elements_minus_two(face_position)){
        if (was_true){  // continue following (special value)
            setOutput("poi", "face");
            setOutput("poi_pos", face_position);
            return  BT::NodeStatus::SUCCESS;
        }
        else{  // do not follow
        setOutput("poi", "none" );
        return BT::NodeStatus::FAILURE;
        }
    }
    setOutput("poi", "face");
    setOutput("poi_pos", face_position);
    was_true = true;
    return  BT::NodeStatus::SUCCESS;
}

PortsList FaceDetected::providedPorts()
{
    return {OutputPort<std::string>("poi"), OutputPort<std::vector<double>>("poi_pos")};
}