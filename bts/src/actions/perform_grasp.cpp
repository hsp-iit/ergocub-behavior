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

#include <behaviortree_cpp_v3/action_node.h>

#include "perform_grasp.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <iostream>


PerformGrasp::PerformGrasp(string name, const NodeConfiguration& config) :
    CoroActionNode(name, config)
{
    this->grasp_distance_thr = 600;
    this->use_neck = false;
    this->neck_angle = 0;
    this->robot_name= "ergocub";

    is_ok_ = init(name);
}

bool PerformGrasp::init(std::string name)
{
//    Opening and Connecting Manipulation part
    std::string manip_server_name = "/Components/Manipulation"s;
    std::string manip_client_name = "/BT/" + name + "/Manipulation"s;

    manip_client_port.open(manip_client_name);

    if (!yarp.connect(manip_client_name,manip_server_name))
    {
        std::cout << "Error! Could not connect to server " << manip_server_name << '\n';
        return false;
    }
    manipulation_client_.yarp().attachAsClient(manip_client_port);

//  Opening and Connecting Object Detection part
    std::string od_server_name = "/Components/ObjectDetection"s;
    std::string od_client_name = "/BT/" + name + "/ObjectDetection"s;

    od_client_port.open(od_client_name);

    if (!yarp.connect(od_client_name,od_server_name))
    {
        std::cout << "Error! Could not connect to server " << od_server_name << '\n';
        return false;
    }
    object_detection_client_.yarp().attachAsClient(od_client_port);

    if (this->use_neck)
        head_control_ = new eCubHead(this->robot_name, name);

    return true;
}

NodeStatus PerformGrasp::tick()
{
//    auto poses = object_detection_client_.get_poses();
//
//    for (auto & element : poses)
//    {
//        std::cout << element << " ";
//    }
//    std::cout << std::endl;
//
//    manipulation_client_.grasp(poses);

    if (this->use_neck)
        head_control_->set_camera_tilt(this->neck_angle);

    std::cout << "ready 1" << std::endl;
    manipulation_client_.ready(true);
    std::cout << "ready 2" << std::endl;

    auto start = std::time(NULL);

    while((std::time(NULL) - start) < 4) {
//          setStatusRunningAndYield();
          std::cout << "consistency while" << std::endl;
          auto distance = object_detection_client_.get_distance();
          std::this_thread::sleep_for(std::chrono::milliseconds(50));
          std::cout << distance << std::endl;
          if (distance > this->grasp_distance_thr)
          {
              manipulation_client_.home(false);
              std::this_thread::sleep_for(std::chrono::milliseconds(2000));
              return NodeStatus::FAILURE;
          }

      }
    std::cout << "pre finished" << std::endl;
    while (true){
        std::cout << "test2.1" << std::endl;
        auto fin = manipulation_client_.finished();
        std::cout << "test2.2" << std::endl;
        std::cout << fin << std::endl;
        if (fin == "Si"){
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "test3" << std::endl;
    manipulation_client_.testgrasp();
    std::cout << "test4" << std::endl;

    if (this->use_neck)
        head_control_->set_camera_tilt(-this->neck_angle);

//    manipulation_client_.home(false);
//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    setOutput("message", true );
    return NodeStatus::SUCCESS;
}

void PerformGrasp::halt()
{
    CoroActionNode::halt();
}


PortsList PerformGrasp::providedPorts()
{
    return { OutputPort<bool>("message") };
}
