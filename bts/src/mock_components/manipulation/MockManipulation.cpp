
#include <thread>
#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Property.h>
#include "CommandInterface.h"

class MockManipulation: CommandInterface
{

    public:
        
        MockManipulation(){}   

        bool open()
        {
            this->yarp().attachAsServer(server_port);
            if (!server_port.open("/Components/Manipulation")) {
                yError("Could not open ");
                return false;
            }
            return true;
        }

        void close()
        {
            server_port.close();
        }
        
            
        bool grasp() { return true; }

        bool is_finished() { return true; }
        
        bool perform_cartesian_action(const std::string& actionName) { return true; }

        bool perform_grasp_action(const std::string& actionName) { return true; }

        bool perform_joint_space_action(const std::string& actionName) { return true; }

        bool move_hands_to_pose(const yarp::sig::Matrix& leftPose,
                                const yarp::sig::Matrix& rightPose,
                                const double time) { return true; }

        bool move_joints_to_position(const std::vector<double>& position, const double time) {return true;}

        bool move_object_to_pose(const yarp::sig::Matrix& pose, const double time) { return true; }

        bool release_object() { return true; }

        void stop() { return; }
        
        void shut_down() { return; }	
		
    private:
        yarp::os::RpcServer server_port;
};

int main(int argc, char *argv[])
{
    yarp::os::Network yarp;

    MockManipulation server;
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