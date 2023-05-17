
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Property.h>
#include "RPCServerInterface.h"

class MockGazeController: RPCServerInterface
{

    public:
        
        MockGazeController(){}   

        bool open()
        {
            this->yarp().attachAsServer(server_port);
            if (!server_port.open("/Components/GazeController")) {
                yError("Could not open ");
                return false;
            }
            return true;
        }

        void close()
        {
            server_port.close();
        }
        
        bool look_at(const std::vector<double>& point) { return true; };

        bool set_gain(const double gain) { return true; };
		
    private:
        yarp::os::RpcServer server_port;
};

int main(int argc, char *argv[])
{
    yarp::os::Network yarp;

    MockGazeController server;
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