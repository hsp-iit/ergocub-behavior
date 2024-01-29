#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

// Include BehaviorTree.CPP
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_minitrace_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_file_logger.h>
#include "behaviortree_cpp_v3/loggers/bt_zmq_publisher.h" // This makes Groot work

// Include YARP
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/LogStream.h>

// Include Boost (to read config file)
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

/* INCLUDE NODES */
#include <actions/robot_look_at_poi.h>
#include <actions/robot_change_face.h>
#include <actions/do_response_action.h>
#include <actions/go_grasp.h>

#include <conditions/box_detected.h>
#include <conditions/face_detected.h>
#include <conditions/object_is_graspable.h>
#include <conditions/focus_detected.h>
#include <conditions/action_detected.h>
#include <conditions/robot_navigating.h>

#include "common.h"

using namespace std;
using namespace BT;


int main(int argc, char **argv)
{
    std::string config_file = "/usr/local/src/robot/hsp/ergocub-behavior/bts/config.json";
    pt::ptree config;
    pt::read_json(config_file, config);
    std::string bt_description = config.get<std::string>("bt_description");

    BehaviorTreeFactory bt_factory;

    /* REGISTER NODES */
    bt_factory.registerBuilder<RobotLookAtPOI>("RobotLookAtPOI", build_node<RobotLookAtPOI>(config));
    bt_factory.registerBuilder<RobotChangeFace>("RobotChangeFace", build_node<RobotChangeFace>(config));
    bt_factory.registerBuilder<DoResponseAction>("DoResponseAction", build_node<DoResponseAction>(config));
    bt_factory.registerBuilder<GoGrasp>("GoGrasp", build_node<GoGrasp>(config));  

    bt_factory.registerBuilder<BoxDetected>("BoxDetected", build_node<BoxDetected>(config));
    bt_factory.registerBuilder<FaceDetected>("FaceDetected", build_node<FaceDetected>(config));
    bt_factory.registerBuilder<ObjectIsGraspable>("ObjectIsGraspable", build_node<ObjectIsGraspable>(config));
    bt_factory.registerBuilder<FocusDetected>("FocusDetected", build_node<FocusDetected>(config));
    bt_factory.registerBuilder<ActionDetected>("ActionDetected", build_node<ActionDetected>(config));
    bt_factory.registerBuilder<RobotNavigating>("RobotNavigating", build_node<RobotNavigating>(config));

    // Initialize Blackboard and set default values
    auto blackboard = BT::Blackboard::create();
    blackboard->set("the_has_box", "no");
    blackboard->set("the_poi", "none");
    auto tree = bt_factory.createTreeFromFile(bt_description, blackboard);

    // Create some logger
    StdCoutLogger logger_cout(tree);
    MinitraceLogger logger_minitrace(tree, "/tmp/bt_trace.json");
    FileLogger logger_file(tree, "/tmp/bt_trace.fbl");

    PublisherZMQ publisher_zmq(tree);
    printTreeRecursively(tree.rootNode());

    while(true)
    {
            auto status = tree.tickRoot();
            std::this_thread::sleep_for (std::chrono::milliseconds(50));
    }

    return 0;
}
