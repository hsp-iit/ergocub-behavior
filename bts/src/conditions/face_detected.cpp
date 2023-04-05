#include <behaviortree_cpp_v3/condition_node.h>
#include "face_detected.h"

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
    std::string server_name = "/Components/ActionRecognition"s;
    std::string client_name = "/BT/" + name + "/ActionRecognition"s;

    client_port.open(client_name);

    if (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        return false;
    }
    action_recognition_client_.yarp().attachAsClient(client_port);
    return true;
}

bool ar_are_all_elements_minus_one(const std::vector<double>& vec) {
    for (double element : vec) {
        if (element != -1.) {
            return false;
        }
    }
    return true;
}

NodeStatus FaceDetected::tick()
{
    auto face_position = action_recognition_client_.get_face_position();
    // for(int i=0; i<3; i++)
        // std::cout << face_position[i] << std::endl;
    if(ar_are_all_elements_minus_one(face_position)){
        setOutput("poi", "none" );
        return BT::NodeStatus::FAILURE;
    }
    setOutput("poi", "face");
    setOutput("poi_pos", face_position);
    return  BT::NodeStatus::SUCCESS;
}

PortsList FaceDetected::providedPorts()
{
    return {OutputPort<std::string>("poi"), OutputPort<std::vector<double>>("poi_pos")};
}