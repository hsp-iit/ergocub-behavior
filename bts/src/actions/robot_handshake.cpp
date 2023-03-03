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

#include "robot_handshake.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>


RobotHandshake::RobotHandshake(string name, const NodeConfiguration& config) :
    CoroActionNode(name, config)
{
    is_ok_ = init(name);
}

bool RobotHandshake::init(std::string name)
{
    // MANIPULATION
    std::string man_server_name = "/Components/Manipulation"s;
    std::string man_client_name = "/BT/" + name + "/Manipulation"s;

    man_client_port.open(man_client_name);

    if (!yarp.connect(man_client_name,man_server_name))
    {
        std::cout << "Error! Could not connect to server " << man_server_name << '\n';
        return false;
    }
    manipulation_client_.yarp().attachAsClient(man_client_port);

    // ACTION
    std::string act_server_name = "/Components/ActionRecognition"s;
    std::string act_client_name = "/BT/" + name + "/ActionRecognition/handshake_action_recognized"s;

    act_client_port.open(act_client_name);

    // connect to server
    if (!yarp.connect(act_client_name,act_server_name))
    {
       std::cout << "Error! Could not connect to server " << act_server_name << '\n';
       return false;
    }
    action_recognition_client_.yarp().attachAsClient(act_client_port);
    return true;
}

NodeStatus RobotHandshake::tick()
{
    cout << "===============================================================================================" << endl;
    cout << "===============================================================================================" << endl;
    cout << "===============================================================================================" << endl;
    // write in file
    std::ofstream outfile;
    outfile.open("robot_commands.txt", std::ios_base::app); // append instead of overwrite
    outfile << "Doing handshake..."  << std::endl;
    // end
//    setStatusRunningAndYield();
    manipulation_client_.shake(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    while (true){
        auto action = action_recognition_client_.get_action();
        if (action == 2){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else{
            break;
        }
    }
//    return NodeStatus::FAILURE;
    manipulation_client_.home(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return NodeStatus::SUCCESS;
}

void RobotHandshake::halt()
{
    CoroActionNode::halt();
}


PortsList RobotHandshake::providedPorts()
{
}
