#include <behaviortree_cpp_v3/condition_node.h>
#include "face_detected.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

#include <iostream>


FaceDetected::FaceDetected(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    ConditionNode(name, nc),
    bt_config(bt_config)
{
    std::string server_name =  bt_config.get<std::string>("components.perception.port");
    std::string client_name = "/BT/" + name + server_name;

    client_port.open(client_name);

    while (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    ecub_perception_client_.yarp().attachAsClient(client_port);
}


NodeStatus FaceDetected::tick()
{
    auto face_position_yarp = ecub_perception_client_.get_face_position();
    std::vector<double> face_position(3);

    for (std::size_t i = 0; i < 3; ++i)
        face_position[i] = face_position_yarp[i];
    if (are_all_elements_zero(face_position)){
       throw(std::runtime_error("face_detected: received (0,0,0) as target position (maybe focus is dead?"));
    }
    if(are_all_elements_minus_one(face_position)){
        setOutput("poi", "none");
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