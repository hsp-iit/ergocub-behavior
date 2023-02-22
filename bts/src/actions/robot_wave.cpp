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

#include "robot_wave.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <fstream>


RobotWave::RobotWave(string name, const NodeConfiguration& config) :
    CoroActionNode(name, config)
{
    is_ok_ = init(name);
}

bool RobotWave::init(std::string name)
{
    std::string server_name = "/command"s;
    std::string client_name = "/BT/" + name + "/Manipulation"s;

    client_port.open(client_name);

    if (!yarp.connect(client_name,server_name))
    {
        std::cout << "Error! Could not connect to server " << server_name << '\n';
        return false;
    }
    manipulation_client_.yarp().attachAsClient(client_port);
    return true;
}

NodeStatus RobotWave::tick()
{
    cout << "Waving..." << endl;
    // write in file
    std::ofstream outfile;
    outfile.open("robot_commands.txt", std::ios_base::app); // append instead of overwrite
    outfile << "Waving..." << std::endl;
    // end
    setOutput("message", "true" );
    manipulation_client_.wave();

    auto start = std::time(NULL);

    while((std::time(NULL) - start) < 5) {
        setStatusRunningAndYield();
    }

    setOutput("message", "true" );
    return NodeStatus::SUCCESS;
}

void RobotWave::halt()
{
    CoroActionNode::halt();
}


PortsList RobotWave::providedPorts()
{
    return { OutputPort<std::string>("message") };
}
