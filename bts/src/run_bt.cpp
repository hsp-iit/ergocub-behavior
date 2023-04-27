#ifdef ZMQ_FOUND
#include "behaviortree_cpp_v3/loggers/bt_zmq_publisher.h"
#endif

#include <iostream>
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <yarp/os/LogStream.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_minitrace_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_file_logger.h>

#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/ResourceFinder.h>

/* INCLUDE NODES */
#include <actions/robot_look_at_poi.h>
#include <actions/robot_change_face.h>
#include <actions/do_response_action.h>
#include <actions/go_home.h>
#include <actions/go_ready.h>
#include <actions/go_grasp.h>
#include <actions/go_release.h>

#include <conditions/box_detected.h>
#include <conditions/face_detected.h>
#include <conditions/object_is_graspable.h>
#include <conditions/focus_detected.h>
#include <conditions/action_detected.h>

/* END */

using namespace std;
using namespace BT;


int main(int argc, char **argv)
{
    std::string fileName = "./ergocub-behavior/bts/descriptions/ergotree.xml";

    BehaviorTreeFactory bt_factory;

    /* REGISTER NODES */
    bt_factory.registerNodeType<RobotLookAtPOI>("RobotLookAtPOI");
    bt_factory.registerNodeType<RobotChangeFace>("RobotChangeFace");
    bt_factory.registerNodeType<DoResponseAction>("DoResponseAction");
    bt_factory.registerNodeType<GoHome>("GoHome");
    bt_factory.registerNodeType<GoReady>("GoReady");
    bt_factory.registerNodeType<GoGrasp>("GoGrasp");
    bt_factory.registerNodeType<GoRelease>("GoRelease");

    bt_factory.registerNodeType<BoxDetected>("BoxDetected");
    bt_factory.registerNodeType<FaceDetected>("FaceDetected");
    bt_factory.registerNodeType<ObjectIsGraspable>("ObjectIsGraspable");
    bt_factory.registerNodeType<FocusDetected>("FocusDetected");
    bt_factory.registerNodeType<ActionDetected>("ActionDetected");
    /* END */

//    auto tree = bt_factory.createTreeFromFile(fileName);
     auto blackboard = BT::Blackboard::create();
     blackboard->set("the_has_box", "no");
     auto tree = bt_factory.createTreeFromFile(fileName, blackboard);

    // Create some logger
    StdCoutLogger logger_cout(tree);
    MinitraceLogger logger_minitrace(tree, "/tmp/bt_trace.json");
    FileLogger logger_file(tree, "/tmp/bt_trace.fbl");

#ifdef ZMQ_FOUND
    PublisherZMQ publisher_zmq(tree);
#endif
    printTreeRecursively(tree.rootNode());


    while(true)
    {
            auto status = tree.tickRoot();
            std::this_thread::sleep_for (std::chrono::milliseconds(50));

            //if (status == BT::NodeStatus::SUCCESS) {
                // std::cout << "Success" << std::endl;
//                break;
            //}

            //if (status == BT::NodeStatus::FAILURE) {
                // std::cout << "Failure" << std::endl;
//                break;
            //}
    }

    return 0;
}
