#include <ros/ros.h>
#include <rpwc_bridge/robot_curr_pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <eigen3/Eigen/Eigen>
#include <sensor_msgs/JointState.h>


class gazebo_bridge
{
public:
	gazebo_bridge();
	~gazebo_bridge();

  	double dt_;

private:
	void callback_curr_pose(const geometry_msgs::Pose::ConstPtr& msg);
	void callback_rpwc_pose_des(const geometry_msgs::Pose::ConstPtr& msg);
	void callback_joint_states(const sensor_msgs::JointState::ConstPtr& msg);
	bool callback_robot_curr_pose(rpwc_bridge::robot_curr_pose::Request  &req, rpwc_bridge::robot_curr_pose::Response &res);

	ros::Subscriber sub_curr_pos_, sub_rpwc_pose_des_, sub_joint_states_;
	ros::Publisher pub_pos_des_, pub_curr_pos_, pub_joint_states_;
	ros::ServiceServer server_robot_curr_pose_;

	geometry_msgs::Pose 	msg_pose_;
	sensor_msgs::JointState msg_joint_;
};//End of class SubscribeAndPublish