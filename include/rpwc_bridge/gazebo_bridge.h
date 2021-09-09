#include <ros/ros.h>
#include <rpwc/robot_curr_pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <eigen3/Eigen/Eigen>


class gazebo_bridge
{
public:
	gazebo_bridge();
	~gazebo_bridge();

  	double dt_;

private:
	void callback_curr_pose(const geometry_msgs::Pose::ConstPtr& msg);
	void callback_rpwc_pose_des(const geometry_msgs::Pose::ConstPtr& msg);
	bool callback_robot_curr_pose(rpwc::robot_curr_pose::Request  &req, rpwc::robot_curr_pose::Response &res);

	ros::Subscriber sub_curr_pos_, sub_rpwc_pose_des_;
	ros::Publisher pub_pos_des_, pub_curr_pos_;
	ros::ServiceServer server_robot_curr_pose_;

	geometry_msgs::Pose msg_pose_;

};//End of class SubscribeAndPublish