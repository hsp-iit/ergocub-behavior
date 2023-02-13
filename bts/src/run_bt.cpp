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
#include <conditions/handout_successful.h>
#include <conditions/correct_action_recognized.h>

#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/ResourceFinder.h>

using namespace std;
using namespace BT;


int main(int argc, char *argv[])
{

    std::string fileName = "/home/btuc/bt-assignment/bts/descriptions/ergotree.xml";


    BehaviorTreeFactory bt_factory;
    bt_factory.registerNodeType<AskToGetCloser>("AskToGetCloser");
    bt_factory.registerNodeType<HandoutSuccessful>("HandoutSuccessful");
    bt_factory.registerNodeType<CorrectActionRecognized>("CorrectActionRecognized");

    BT::Tree tree = bt_factory.createTreeFromFile(fileName);


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
            tree.tickRoot();
            std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    return 0;
}
