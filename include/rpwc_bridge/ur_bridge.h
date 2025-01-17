#include <ros/ros.h>
#include <rpwc_bridge/robot_curr_pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>


#include <eigen3/Eigen/Eigen>



class ur_bridge
{
public:
	ur_bridge();
	~ur_bridge();


  double dt_;

private:
	ros::NodeHandle n_;

	void callback_curr_pose(const geometry_msgs::Pose::ConstPtr& msg);
	void callback_rpwc_pose_des(const geometry_msgs::Pose::ConstPtr& msg);
	bool callback_robot_curr_pose(rpwc_bridge::robot_curr_pose::Request  &req, rpwc_bridge::robot_curr_pose::Response &res);

	ros::Subscriber sub_curr_pos_, sub_rpwc_pose_des_;
	ros::Publisher pub_pos_des_, pub_curr_pos_;
	ros::ServiceServer server_robot_curr_pose_;

	Eigen::Quaterniond quat_base2EE_, quat_base2EE_old_;
	Eigen::Matrix4d T_base_2_EE_;
	bool first_quat_base_EE_;

};//End of class SubscribeAndPublish