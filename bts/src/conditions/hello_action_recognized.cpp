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


#include <behaviortree_cpp_v3/condition_node.h>
#include "hello_action_recognized.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <ActionRecognitionInterface.h>


HelloActionRecognized::HelloActionRecognized(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool HelloActionRecognized::init(std::string name)
{
    // MANIPULATION
    std::string man_server_name = "/Components/ActionRecognition"s;
    std::string man_client_name = "/BT/" + name + "/ActionRecognition/hello_action_recognized"s;

    man_client_port.open(man_client_name);

    // connect to server
    if (!yarp.connect(man_client_name,man_server_name))
    {
     std::cout << "Error! Could not connect to server " << man_server_name << '\n';
     return false;
    }
    action_recognition_client_.yarp().attachAsClient(man_client_port);

      // NO BOX DETECTED
    std::string obj_server_name = "/Components/ObjectDetection"s;
    std::string obj_client_name = "/BT/" + name + "/ObjectDetection"s;

    obj_client_port.open(obj_client_name);

    if (!yarp.connect(obj_client_name,obj_server_name))
    {
        std::cout << "Error! Could not connect to server " << obj_server_name << '\n';
        return false;
    }
    object_detection_client_.yarp().attachAsClient(obj_client_port);

  return true;
}

NodeStatus HelloActionRecognized::tick()
{
    auto action = action_recognition_client_.get_action();
    auto distance = object_detection_client_.get_distance();
    if (distance == -1 || distance > 3000){
        return action == 1 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
    }
    else{
        return BT::NodeStatus::FAILURE;
    }

}

PortsList HelloActionRecognized::providedPorts()
{
    return { };
}
