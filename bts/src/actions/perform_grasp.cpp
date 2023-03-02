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

    head_control_ = new eCubHead("ergocubSim", name);

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

    head_control_->set_camera_tilt(0.5);

    manipulation_client_.ready(true);

    auto start = std::time(NULL);

    while((std::time(NULL) - start) < 2) {
//          setStatusRunningAndYield();
          auto poses = object_detection_client_.get_poses();
          std::this_thread::sleep_for(std::chrono::milliseconds(50));
          if (poses[0] == -1)
          {
              return NodeStatus::FAILURE;
          }

      }
    while (true){
        if (manipulation_client_.finished() == "Si"){
            std::cout << manipulation_client_.finished() << std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    manipulation_client_.grasp(true);

    head_control_->set_camera_tilt(-0.5);

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
