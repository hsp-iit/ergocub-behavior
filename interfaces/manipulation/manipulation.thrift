service ManipulationInterface
{
	bool grasp_object(1:list<double> handPoses);                                                # Grasp object with two hands
	
	bool is_finished();                                                                         # Check if the robot has finished excuting a task
	
	bool move_object_to_pose(1:list<double> pose,                                               # Move a grasped object to the given pose in the given time
	                         2:double time);
	                 
	bool move_hands_to_pose(1:list<double> poses,                                               # Move the hands to given poses in the givne time
	                        2:double time);

	bool move_hands_by_action(1:string actionName);                                             # Move the hands based on a prescribed action
	
	bool move_to_configuration(1:list<double> jointConfiguration,
	                           2:double time);                                                  # Move the joints to a given configuration
	
	bool move_to_named_configuration(1:string configName);                                      # Move joints to prescribed configuration
	
	bool release_object();                                                                      # As it says on the label
	
	bool stop();                                                                                # Stop the robot moving immediately
}
