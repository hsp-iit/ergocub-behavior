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
#include "human_not_too_far.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <ActionRecognitionInterface.h>


HumanNotTooFar::HumanNotTooFar(string name, const NodeConfiguration& config) :
    ConditionNode(name, config)
{
    is_ok_ = init(name);
}

bool HumanNotTooFar::init(std::string name)
{
    // ACT
    std::string act_server_name = "/Components/ActionRecognition"s;
    std::string act_client_name = "/BT/" + name + "/ActionRecognition/human_not_too_far"s;

    act_client_port.open(act_client_name);

    // connect to server
    if (!yarp.connect(act_client_name,act_server_name))
    {
     std::cout << "Error! Could not connect to server " << act_server_name << '\n';
     return false;
    }
    action_recognition_client_.yarp().attachAsClient(act_client_port);

    // OD
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

NodeStatus HumanNotTooFar::tick()
{
    auto human_distance = action_recognition_client_.get_distance();
    if (human_distance == -1){
        return BT::NodeStatus::FAILURE;
    }
    auto box_distance = object_detection_client_.get_distance();
    if (box_distance == -1 or box_distance >= 1500){
        return human_distance < 2 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
    }
    else{
        return BT::NodeStatus::FAILURE;
    }

}

PortsList HumanNotTooFar::providedPorts()
{
    return { };
}
