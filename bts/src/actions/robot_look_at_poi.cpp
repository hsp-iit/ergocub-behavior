#include <behaviortree_cpp_v3/action_node.h>

#include "robot_look_at_poi.h"
#include "common.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Property.h>
#include <yarp/dev/PolyDriver.h>
#include <iostream>
#include <list>

using yarp::os::BufferedPort;
using yarp::os::Bottle;
using yarp::os::Property;
using yarp::dev::PolyDriver;


RobotLookAtPOI::RobotLookAtPOI(string name, const NodeConfiguration& nc, pt::ptree bt_config) :
    SyncActionNode(name, nc),
    bt_config(bt_config)
{
     yarp::os::Network yarp;
    
    std::string server_name = bt_config.get<std::string>("components.gaze.port");
    std::string client_name = "/BT/" + name + server_name;

    client_port.open(client_name);

    while (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    this->gaze_controller.yarp().attachAsClient(client_port);
    this->gaze_controller.set_gain(0.01);
    none_counter = 0;
    last_poi = "";
}

NodeStatus RobotLookAtPOI::tick()
{
    // Read poi type
    Optional<std::string> msg = getInput<std::string>("poi");
    if (!msg)
    {
      throw BT::RuntimeError("missing required input [message]: ",
                              msg.error() );
    }
    std::cout << "ROBOT LOOK AT POI: Robot says: " << msg.value() << std::endl;

    // Declare final point
    std::vector<double> setpoint;

    // Get poi_pos if there is poi
    if(msg!="none"){
        if(last_poi != "not none"){
            this->gaze_controller.set_gain(0.01);
            last_poi = "not none";
        }
        none_counter = 0;
        Optional<std::vector<double>> poi_pos = getInput<std::vector<double>>("poi_pos");
        if (!poi_pos)
        {
            throw BT::RuntimeError("missing required input [message]: ",
                              msg.error() );
        }
        std::vector<double> poi_position = poi_pos.value();
        std::cout << poi_position[0] << " " <<  poi_position[1] << " " <<  poi_position[2] << std::endl;

        if(are_all_elements_minus_two(poi_position)){ // special value, do not send command TODO REMOVE
            return NodeStatus::SUCCESS;
        }
        setpoint.push_back(poi_position[0]);
        setpoint.push_back(poi_position[1]);
        setpoint.push_back(poi_position[2]);  // offset for camera, remove it in ergoCub
        this->gaze_controller.look_at(setpoint);
    }
    else{
        if(last_poi != "none"){
            this->gaze_controller.set_gain(0.001);
            last_poi = "none";
        }
        none_counter++;
        if(none_counter > none_counter_thr){
            setpoint.push_back(-1);
            setpoint.push_back(0);
            setpoint.push_back(0.8);
            this->gaze_controller.look_at(setpoint);
        }
    }

    return NodeStatus::SUCCESS;
}

PortsList RobotLookAtPOI::providedPorts()
{
    return {InputPort<std::string>("poi"),
            InputPort<std::vector<double>>("poi_pos")};
}
