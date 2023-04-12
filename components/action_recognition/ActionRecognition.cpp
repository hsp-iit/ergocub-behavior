#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>

#include <ActionRecognitionInterface.h>
#include <atomic>

#include <iostream>


const int TYPE_DISTANCE_POSE = 1;
const int TYPE_DISTANCE = 2;
const int TYPE_POSE = 3;
const int TYPE_NONE = 4;


#define  BUFF_SIZE   8+2+1+24

typedef struct {
	long  data_type;
	unsigned char  data_buff[BUFF_SIZE];
} t_data;


class ActionRecognition : public ActionRecognitionInterface
{
public:
    ActionRecognition() = default;
    bool open()
    {

        this->yarp().attachAsServer(server_port);
        if (!server_port.open("/Components/ActionRecognition")) {
            yError("Could not open ");
            return false;
        }

        if ( -1 == (this->msqid = msgget( (key_t)5678, IPC_CREAT | 0666)))
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


    std::int16_t get_action()
    {
        this->read_queue();
        return this->action;
    }

    bool is_focused()
    {
        this->read_queue();
        return this->focus;
    }

    double get_distance()
    {
        this->read_queue();
        return this->distance;
    }

    std::vector<double> get_face_position()
    {
        this->read_queue();
        auto face_position = this->face_position;

        return face_position;
    }

private:
    yarp::os::RpcServer server_port;
    double distance;
    std::int16_t action;
    bool focus;
    std::vector<double> face_position = std::vector<double>(3);
    long msg_type;
    int msqid;

    void read_queue()
    {
	    t_data   data;

        int success = msgrcv( this->msqid, &data, sizeof( t_data) - sizeof( long), 0, IPC_NOWAIT);
//        std::cout << "received" << success << "bytes" << std::endl;
        success = success != -1;
        if (success) {
//            printf("Reading message of type %ld", data.data_type);
            this->msg_type = data.data_type; }
        else if ( !success && errno == ENOMSG) {
//            printf("Empty queue");
            this->distance = -2.;
//            this->action = -2;
//            this->focus = false;
            std::fill(this->face_position.begin(), this->face_position.end(), -2.);
            return;}
		else {
		    this->msg_type = TYPE_NONE;
		    perror( "msgrcv() failed");
			exit( 1);
		}

        memcpy(&this->action, data.data_buff, 2);
        memcpy(&this->distance, data.data_buff+2, 8);
        memcpy(&this->focus, data.data_buff+10, 2);
        memcpy(this->face_position.data(), data.data_buff+12, 24);
    }
};

int main()
{
    yarp::os::Network yarp;

    ActionRecognition actionRecognition;
    if (!actionRecognition.open()) {
        return 1;
    }

    while (true) {
        yInfo("Server running happily");
        yarp::os::Time::delay(10);
    }

    actionRecognition.close();

    return 0;
}
