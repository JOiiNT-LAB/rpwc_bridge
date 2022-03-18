#include <gazebo_bridge.h>


gazebo_bridge::gazebo_bridge()
{
    ros::NodeHandle n1_;
	ros::NodeHandle n2_("cartesian_quat_pose_controller");

	//Subscriber
	sub_curr_pos_ 		= n2_.subscribe("get_pose", 1, &gazebo_bridge::callback_curr_pose, this);
	sub_rpwc_pose_des_ 	= n1_.subscribe("rpwc_pose_des", 1, &gazebo_bridge::callback_rpwc_pose_des, this);
	sub_joint_states_ 	= n1_.subscribe("/joint_states", 1, &gazebo_bridge::callback_joint_states, this);
	//Publisher
    pub_pos_des_ 		= n2_.advertise<geometry_msgs::PoseStamped>("command", 1);
    pub_curr_pos_ 		= n1_.advertise<geometry_msgs::Pose>("rpwc_robot_curr_pose", 1);
    pub_joint_states_ 	= n1_.advertise<sensor_msgs::JointState>("joint_states", 1);
	//Service Server
  	server_robot_curr_pose_ = n1_.advertiseService("rpwc_robot_curr_pose", &gazebo_bridge::callback_robot_curr_pose, this);
}

gazebo_bridge::~gazebo_bridge(){}


void gazebo_bridge::callback_curr_pose(const geometry_msgs::Pose::ConstPtr& msg)
{
	msg_pose_ = *msg;
	pub_curr_pos_.publish(msg_pose_);
}

void gazebo_bridge::callback_rpwc_pose_des(const geometry_msgs::Pose::ConstPtr& msg)
{
	geometry_msgs::PoseStamped send_pose;
	send_pose.pose.position.x = msg->position.x;
	send_pose.pose.position.y = msg->position.y;
	send_pose.pose.position.z = msg->position.z;
	send_pose.pose.orientation.w = msg->orientation.w;
  	send_pose.pose.orientation.x = msg->orientation.x;
  	send_pose.pose.orientation.y = msg->orientation.y;
  	send_pose.pose.orientation.z = msg->orientation.z;
  	send_pose.header.stamp = ros::Time::now();
	pub_pos_des_.publish(send_pose);
}

void gazebo_bridge::callback_joint_states(const sensor_msgs::JointState::ConstPtr& msg)
{
	msg_joint_ = *msg;
	pub_joint_states_.publish(msg_joint_);
}

bool gazebo_bridge::callback_robot_curr_pose(rpwc::robot_curr_pose::Request &req, rpwc::robot_curr_pose::Response &res)
{
	res.robot_curr_pose = msg_pose_;
	return true;
}