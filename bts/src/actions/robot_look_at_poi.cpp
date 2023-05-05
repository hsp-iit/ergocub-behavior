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
#include <iDynTree/Core/Rotation.h>
#include <iDynTree/Core/Direction.h>
#include <iDynTree/Core/Transform.h>
#include <iDynTree/Core/VectorFixSize.h>

using yarp::os::BufferedPort;
using yarp::os::Bottle;
using yarp::os::Property;
using yarp::dev::PolyDriver;


RobotLookAtPOI::RobotLookAtPOI(string name, const NodeConfiguration& config) :
    SyncActionNode(name, config)
{
    is_ok_ = init(name);
}

bool RobotLookAtPOI::init(std::string name){

    yarp::os::Network yarp;

    #ifdef GAZE
    // Connect to gaze controller
    Property props;
    props.put("device", "gazecontrollerclient");
    props.put("local", "/FollowObject/iKinGazeCtrl");
    props.put("remote", "/iKinGazeCtrl");

    bool ok = driver_gaze_.open(props) && driver_gaze_.view(gaze_) && (gaze_ != nullptr);

    if (!ok)
        throw(std::runtime_error(log_name_ + "::ctor. Error: cannot open IGazeControl interface."));

    gaze_->setTrackingMode(false);
    gaze_->setNeckTrajTime(1);
    // gaze_->blockEyes(0.0);
    // gaze_->blockNeckRoll(0.0);
    #endif
    none_counter = 0;
    return true;
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
    yarp::sig::VectorOf<double> setpoint;

    // Get poi_pos if there is poi
    if(msg!="none"){
        #ifdef GAZE
        gaze_->setNeckTrajTime(1);
        #endif
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
        #ifdef GAZE
        gaze_->setNeckTrajTime(1);
        gaze_->lookAtFixationPoint(setpoint);
        #endif
    }
    else{
        none_counter++;
        if(none_counter > none_counter_thr){
            #ifdef GAZE
            gaze_->setNeckTrajTime(2);
            #endif
            setpoint.push_back(-1);
            setpoint.push_back(0);
            setpoint.push_back(0.7);
            #ifdef GAZE
            gaze_->setNeckTrajTime(2);
            gaze_->lookAtFixationPoint(setpoint);
            #endif
        }
    }

    return NodeStatus::SUCCESS;
}

PortsList RobotLookAtPOI::providedPorts()
{
    return {InputPort<std::string>("poi"),
            InputPort<std::vector<double>>("poi_pos")};
}
