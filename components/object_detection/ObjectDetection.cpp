/*
 *   Copyright (c) 2022 Andrea Rosasco
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>

#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>

#include <ObjectDetectionInterface.h>
#include <atomic>


const int TYPE_DISTANCE_POSE = 1;
const int TYPE_DISTANCE = 2;
const int TYPE_POSE = 3;
const int TYPE_NONE = 4;


#define  BUFF_SIZE   258+24

typedef struct {
	long  data_type;
	unsigned char  data_buff[BUFF_SIZE];
} t_data;


class ObjectDetection : public ObjectDetectionInterface
{
public:
    ObjectDetection() = default;
    bool open()
    {

        this->yarp().attachAsServer(server_port);
        if (!server_port.open("/Components/ObjectDetection")) {
            yError("Could not open ");
            return false;
        }

        if ( -1 == (this->msqid = msgget( (key_t)1234, IPC_CREAT | 0666)))
        {
            perror( "msgget() failed");
            exit( 1);
        }

        return true;
    }

    void close()
    {
        server_port.close();
    }


    std::vector<double> get_poses()
    {
        this->read_queue();
        auto poses = this->poses;

        return poses;
    }

    std::int16_t get_distance()
    {
        yInfo("[ObjectDetection] get_distance called!");
        this->read_queue();
        auto distance = this->distance;

        return distance;
    }

    std::vector<double> get_object_position()
    {
        this->read_queue();
        auto object_position = this->object_position;

        return object_position;
    }

private:
    yarp::os::RpcServer server_port;
    std::vector<double> poses = std::vector<double>(32);
    std::int16_t distance;
    std::vector<double> object_position = std::vector<double>(3);
    long msg_type;
    int msqid;

    void read_queue()
    {
	    t_data   data;

        int success = msgrcv( this->msqid, &data, sizeof( t_data) - sizeof( long), 0, IPC_NOWAIT);
//        std::cout << "received" << success << "bytes" << std::endl;
        success = success != -1;
//        printf("%d", success);
        if (success) {
//            printf("Reading message of type %ld", data.data_type);
            this->msg_type = data.data_type; }
        else if ( !success && errno == ENOMSG) {
//            printf("Empty queue");
//          If the queue is empty we use the last recorded value
//          This is necessary aas the bt calls the component at higher frequency than
//          the one at which the module is running
//            this->distance = -1;
//		    std::fill(this->poses.begin(), this->poses.end(), -1);
		    return;}
		else {
		    this->msg_type = TYPE_NONE;
		    perror( "msgrcv() failed");
			exit( 1);
		}

        memcpy(&this->distance, data.data_buff, 2);
        memcpy(this->poses.data(), data.data_buff+2, 256);
        memcpy(this->object_position.data(), data.data_buff+258, 24);


//		printf("*** New message received ***\nRaw data: ");
//		int i;
//		for(i = 0; i<BUFF_SIZE; i++)
//			printf("%02X ", data.data_buff[i]);
//		printf("\n");
//        printf("Interpreted as a 16t integer: %hd, ", this->distance);
//        for(i = 0; i<256; i++)
//				printf("%f ", this->poses[i]);
//			printf("\n");
    }
};

int main()
{
    yarp::os::Network yarp;

    ObjectDetection objectDetection;
    if (!objectDetection.open()) {
        return 1;
    }

    while (true) {
        yInfo("Server running happily");
        yarp::os::Time::delay(10);
    }

    objectDetection.close();

    return 0;
}
