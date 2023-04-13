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
    // Connect to realsense-holder-publisher
    std::string rhp_server_name = "/realsense-holder-publisher/pose:o"s;
    std::string rhp_client_name = "/BT/" + name + "/realsense-holder-publisher"s;

    rhp_client_port.open(rhp_client_name);

    if (!yarp.connect(rhp_server_name, rhp_client_name))
    {
        throw(std::runtime_error(log_name_ + "::ctor. Error: cannot connect to raelsense-holder-publisher."));
        return false;
    }

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

    // Connect to Object Detection
    std::string od_server_name = "/Components/ObjectDetection"s;
    std::string od_client_name = "/BT/" + name + "/ObjectDetection"s;

    od_client_port.open(od_client_name);

    if (!yarp.connect(od_client_name, od_server_name))
    {
        throw(std::runtime_error(log_name_ + "::ctor. Error: cannot connect to ObjectDetection interface."));
    }
    object_detection_client_.yarp().attachAsClient(od_client_port);

    // Connect to Action Recognition
    ar_server_name = "/Components/ActionRecognition"s;
    ar_client_name = "/BT/" + name + "/RobotLookAtPOIAR"s;

    ar_client_port.open(ar_client_name);

    if (!yarp.connect(ar_client_name, ar_server_name))
    {
       throw(std::runtime_error(log_name_ + "::ctor. Error: cannot connect to ActionRecognition interface."));
    }
    action_recognition_client_.yarp().attachAsClient(ar_client_port);
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
        Optional<std::vector<double>> poi_pos = getInput<std::vector<double>>("poi_pos");
        if (!msg)
        {
            throw BT::RuntimeError("missing required input [message]: ",
                              msg.error() );
        }
        std::vector<double> poi_position = poi_pos.value();
        std::cout << poi_position[0] << " " <<  poi_position[1] << " " <<  poi_position[2] << std::endl;

        if(are_all_elements_minus_two(poi_position)){ // special value, do not send command
            return NodeStatus::SUCCESS;
        }
        setpoint.push_back(poi_position[0]);
        setpoint.push_back(poi_position[1]);
        setpoint.push_back(poi_position[2] + 0.1);
        /*
        iDynTree::Position poi_position_converted;
        poi_position_converted(0) = poi_position[0];
        poi_position_converted(1) = poi_position[1];
        poi_position_converted(2) = poi_position[2] - 0.4;

        // read actual camera position
        #ifdef GAZE
        yarp::os::Bottle input;
        rhp_client_port.read(input);
        iDynTree::Rotation rot;
        iDynTree::Direction dir(input.get(3).asFloat64(), input.get(4).asFloat64(), input.get(5).asFloat64());
        rot = iDynTree::Rotation::RotAxis(dir, input.get(6).asFloat64());
        double x, y, z, w;
        if(!rot.getQuaternion(w, x, y, z)){
            std::cout << "unable to get quaternion" << std::endl;
        }

        iDynTree::Position pos;
        pos(0) = input.get(0).asFloat64();
        pos(1) = input.get(1).asFloat64();
        pos(2) = input.get(2).asFloat64();  //+0.2; TODO remove translation on eCub -> keep it only for iCub
        iDynTree::Transform T(rot, pos);

        auto final = T * poi_position_converted;

        setpoint.push_back(final(0));
        setpoint.push_back(final(1));
        setpoint.push_back(final(2));  // TODO try to put +0.2 here (should look a bit higher)
        #endif
        */
    }
    else{
        setpoint.push_back(-1);
        setpoint.push_back(0);
        setpoint.push_back(0.7);
    }
    #ifdef GAZE
    gaze_->lookAtFixationPoint(setpoint);
    #endif
    return NodeStatus::SUCCESS;
}

/*
void RobotLookAtPOI::halt()
{
    SyncActionNode::halt();
}
*/

PortsList RobotLookAtPOI::providedPorts()
{
    return {InputPort<std::string>("poi"),
            InputPort<std::vector<double>>("poi_pos")};
}
