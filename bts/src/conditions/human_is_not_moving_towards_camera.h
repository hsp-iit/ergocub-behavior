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

/*
#ifndef PERSON_IS_FOCUSED
#define PERSON_IS_FOCUSED
*/


#include <behaviortree_cpp_v3/condition_node.h>
#include <ActionRecognitionInterface.h>
#include <string>
#include <future>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

using namespace BT;
using namespace std;

class HumanIsNotMovingTowardsCamera :  public ConditionNode
{
public:
    HumanIsNotMovingTowardsCamera(string name, const NodeConfiguration &config);
    NodeStatus tick() override;
    static PortsList providedPorts();

private:
    bool init(std::string);
    ActionRecognitionInterface action_recognition_client_;
    bool is_ok_{false};
    yarp::os::Network yarp;
    yarp::os::Port client_port;
};


/*#endif PERSON_IS_FOCUSED */