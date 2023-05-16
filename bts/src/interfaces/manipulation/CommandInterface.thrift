struct Transform
{
}
(
	yarp.name="yarp::sig::Matrix"
	yarp.includefile="yarp/sig/Matrix.h"
)

service CommandInterface
{
	bool grasp();                                                                               # Activate grasp constraints
	
	bool is_finished();                                                                         # Query if the robot has finished moving
	
	bool perform_cartesian_action(1:string actionName);                                         # Move the hands with a prescribed motion
	
	bool perform_grasp_action(1:string actionName);                                             # Move the object with a prescribed action
	
	bool perform_joint_space_action(1:string actionName);                                       # Move the joints by a prescribed action
	
	bool move_hands_to_pose(1:Transform leftPose,
	                        2:Transform rightPose,
	                        3:double time);                                                     # Move the hands to specified poses
	
	bool move_joints_to_position(1:list<double> position,
	                             2:double time);                                                # Move the joints to the given position

	bool move_object_to_pose(1:Transform pose,
	                         2:double time);                                                    # Move the grasped object to a specified pose

	bool release_object();                                                                      # As it says on the label
	
	void stop();                                                                                # Stop the robot moving immediately

	void shut_down();                                                                           # Shut down the command server
}