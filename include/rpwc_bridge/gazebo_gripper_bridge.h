#include <ros/ros.h>
#include <rpwc/rpwc_gripper_cmd.h>
#include <rpwc/rpwc_gripper_pos.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>


class gazebo_gripper_bridge
{
public:
	gazebo_gripper_bridge();
	~gazebo_gripper_bridge();

  	double dt_;

private:
	void callback_curr_grip(const std_msgs::Float64::ConstPtr& msg);
	bool callback_curr_single_grip(rpwc::rpwc_gripper_pos::Request &req, rpwc::rpwc_gripper_pos::Response &res);
	void callback_rpwc_gripper_cmd(const std_msgs::Float64::ConstPtr& msg);
	bool callback_rpwc_gripper_single_cmd(rpwc::rpwc_gripper_cmd::Request  &req, rpwc::rpwc_gripper_cmd::Response &res);

	ros::Subscriber sub_curr_grip_, sub_rpwc_gripper_cmd_;
	ros::Publisher pub_grip_des_, pub_curr_grip_;
	ros::ServiceServer server_rpwc_gripper_cmd_, server_curr_grip_;

	std_msgs::Float64 curr_grip_, grip_des_;

};//End of class SubscribeAndPublish