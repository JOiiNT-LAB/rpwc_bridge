#include <ros/ros.h>
#include <ros/rate.h>
#include <rpwc_bridge/rpwc_gripper_cmd.h>
#include <std_msgs/Float64.h>
#include <onrobot_msgs/onrobot_gripper.h>
#include <rpwc_bridge/rpwc_gripper_pos.h>



ros::Publisher pub_gripper_des_;
bool grasping_ = false;
onrobot_msgs::onrobot_gripper cmd_msg_;
std_msgs::Float64 curr_grip_;

bool callback_rpwc_gripper_single_cmd(rpwc_bridge::rpwc_gripper_cmd::Request  &req, rpwc_bridge::rpwc_gripper_cmd::Response &res)
{
	if(req.EE_cmd_.data >= 0.5 && !grasping_)
	{
		grasping_ = true;
		cmd_msg_.position = int(0);
		pub_gripper_des_.publish(cmd_msg_);
	}
	else if(req.EE_cmd_.data < 0.5 && grasping_)
	{
		grasping_ = false;
		cmd_msg_.position = int(45);
		pub_gripper_des_.publish(cmd_msg_);
	}

	return true;
}

bool callback_EE_curr_pos(rpwc_bridge::rpwc_gripper_pos::Request &req, rpwc_bridge::rpwc_gripper_pos::Response &res)
{
	res.EE_pos_ = curr_grip_;
	return true;
}

void callback_rpwc_gripper_cmd(const std_msgs::Float64::ConstPtr& msg)
{
	curr_grip_ = *msg;
	cmd_msg_.position = int((msg->data*45)/0.038);
	pub_gripper_des_.publish(cmd_msg_);
}


//-----------------------------------------------------
//                                                 main
//-----------------------------------------------------
int main(int argc, char **argv)
{

	ros::init(argc, argv, "onrobot_twofg_bridge_node");
	ros::NodeHandle nh;


  	double rate_30Hz = 30.0;
	ros::Rate r_30HZ(rate_30Hz);

	cmd_msg_.force = 20;
	cmd_msg_.velocity = 80;

	//Subscriber
	ros::Subscriber sub_rpwc_gripper_cmd = nh.subscribe("rpwc_EE_cmd", 1, &callback_rpwc_gripper_cmd);
  	//Publisher
    pub_gripper_des_ = nh.advertise<onrobot_msgs::onrobot_gripper>("set_width", 1);

	//Service Server
  	ros::ServiceServer server_rpwc_gripper_cmd = nh.advertiseService("rpwc_EE_single_cmd", &callback_rpwc_gripper_single_cmd);
	ros::ServiceServer server_curr_grip_ = nh.advertiseService("rpwc_EE_curr_pos", &callback_EE_curr_pos);


	while(ros::ok())
	{
		ros::spinOnce();
		r_30HZ.sleep();
	}// end while()
	return 0;
}