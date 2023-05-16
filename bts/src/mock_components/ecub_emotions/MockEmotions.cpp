
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Property.h>
#include "ergoCubEmotions_IDL.h"

class MockEmotions: ergoCubEmotions_IDL
{

    public:
        
        MockEmotions(){}   

        bool open()
        {
            this->yarp().attachAsServer(server_port);
            if (!server_port.open("/ergoCubEmotions/rpc")) {
                yError("Could not open ");
                return false;
            }
            return true;
        }

        void close()
        {
            server_port.close();
        }
        
        std::vector<std::string> availableEmotions() { return {"none"}; };

        bool setEmotion(std::string command) { return true; };
		
    private:
        yarp::os::RpcServer server_port;
};

int main(int argc, char *argv[])
{
    yarp::os::Network yarp;

    MockEmotions server;
    if (!server.open()) {
        return 1;
    }

    while (true) {
        yInfo("Server running happily");
        yarp::os::Time::delay(10);
    }

    server.close();
    return 0;
}