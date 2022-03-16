#include <gazebo_gripper_bridge.h>


gazebo_gripper_bridge::gazebo_gripper_bridge()
{
	ros::NodeHandle n1_;
	ros::NodeHandle n2_("/prismatic_ee_controller");

	//Subscriber
	sub_curr_grip_ = n2_.subscribe("get_grip", 1, &gazebo_gripper_bridge::callback_curr_pos, this);
	sub_rpwc_gripper_cmd_ = n1_.subscribe("rpwc_EE_cmd", 1, &gazebo_gripper_bridge::callback_rpwc_gripper_cmd, this);
	//Publisher
    pub_grip_des_ = n2_.advertise<std_msgs::Float64>("command", 1);
	pub_curr_grip_ = n1_.advertise<std_msgs::Float64>("rpwc_EE_curr_pos", 1);
	//Service Server
	server_rpwc_gripper_cmd_ = n1_.advertiseService("rpwc_EE_single_cmd", &gazebo_gripper_bridge::callback_rpwc_gripper_single_cmd, this);
	server_curr_grip_ = n1_.advertiseService("rpwc_EE_curr_pos", &gazebo_gripper_bridge::callback_EE_curr_pos, this);
}

gazebo_gripper_bridge::~gazebo_gripper_bridge(){}


void gazebo_gripper_bridge::callback_curr_pos(const std_msgs::Float64::ConstPtr& msg)
{
	curr_grip_ = *msg;
	pub_curr_grip_.publish(curr_grip_);
}

bool gazebo_gripper_bridge::callback_EE_curr_pos(rpwc::rpwc_gripper_pos::Request &req, rpwc::rpwc_gripper_pos::Response &res)
{
	res.EE_pos_ = curr_grip_;
	return true;
}

void gazebo_gripper_bridge::callback_rpwc_gripper_cmd(const std_msgs::Float64::ConstPtr& msg)
{
	grip_des_ = *msg;
	pub_grip_des_.publish(grip_des_);
}

bool gazebo_gripper_bridge::callback_rpwc_gripper_single_cmd(rpwc::rpwc_gripper_cmd::Request &req, rpwc::rpwc_gripper_cmd::Response &res)
{
	grip_des_ = req.EE_cmd_;
	pub_grip_des_.publish(grip_des_);
	return true;
}