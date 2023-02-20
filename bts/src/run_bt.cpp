/*
 *   Copyright (c) 2022 Michele Colledanchise
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/
/**
 * @file main.cpp
 * @authors: Michele Colledanchise <michele.colledanchise@iit.it>
 */

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

#include <actions/ask_to_get_closer.h>
#include <actions/ask_to_keep_the_box_still.h>
#include <actions/ask_to_move_object_closer.h>
#include <actions/ask_to_pay_attention.h>
#include <actions/perform_grasp.h>

#include <conditions/are_points_reachable.h>
#include <conditions/correct_action_recognized.h>
#include <conditions/handout_successful.h>
#include <conditions/human_is_not_moving_towards_camera.h>
#include <conditions/human_not_getting_closer.h>
#include <conditions/human_not_too_far.h>
#include <conditions/human_paying_attention.h>
#include <conditions/is_box_still.h>

#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/ResourceFinder.h>

using namespace std;
using namespace BT;


int main(int argc, char *argv[])
{

    std::string fileName = "/home/ecub/Ergocub-Behaviour-Trees/bts/descriptions/ergotree.xml";


    BehaviorTreeFactory bt_factory;

    bt_factory.registerNodeType<HandoutSuccessful>("HandoutSuccessful");

    bt_factory.registerNodeType<HandoutSuccessful>("CorrectActionRecognized");
    bt_factory.registerNodeType<HandoutSuccessful>("HumanNotTooFar");
    bt_factory.registerNodeType<HandoutSuccessful>("HumanIsNotMovingTowardsCamera");
    bt_factory.registerNodeType<HandoutSuccessful>("AskToGetCloser");
    bt_factory.registerNodeType<HumanNotGettingCloser>("HumanNotGettingCloser");
    bt_factory.registerNodeType<HumanPayingAttention>("HumanPayingAttention");
    bt_factory.registerNodeType<AskToPayAttention>("AskToPayAttention");

    bt_factory.registerNodeType<ArePointsReachable>("ArePointsReachable");
    bt_factory.registerNodeType<AskToMoveObjectCloser>("AskToMoveObjectCloser");
    bt_factory.registerNodeType<IsBoxStill>("IsBoxStill");
    bt_factory.registerNodeType<AskToKeepTheBoxStill>("AskToKeepTheBoxStill");

    bt_factory.registerNodeType<PerformGrasp>("PerformGrasp");


    auto blackboard = BT::Blackboard::create();
    blackboard->set("grasp_success", false);

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
            std::this_thread::sleep_for (std::chrono::milliseconds(100));

            if (status == BT::NodeStatus::SUCCESS) {
                std::cout << "Success" << std::endl;
                break;
            }
//
//            if (status == BT::NodeStatus::FAILURE) {
//                std::cout << "Failure" << std::endl;
//                break;
//            }
    }

    return 0;
}
