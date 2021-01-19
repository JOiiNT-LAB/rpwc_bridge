#include <ros/ros.h>
#include <ros/package.h>
#include <geometry_msgs/Twist.h>

double vel_lin_ = 0.0;
double vel_ang_ = 0.0;
void callback_cmd_vel(const geometry_msgs::Twist::ConstPtr& msg)
{
  vel_lin_ = msg->linear.x;
  vel_ang_ = msg->angular.z;

}


int main(int argc, char** argv){
  ros::init(argc, argv, "joy_web");


  ros::NodeHandle node;
  ros::Rate r_30HZ(50);

  //Subscriber
  ros::Subscriber sub_cmd_web = node.subscribe("/cmd_vel", 1, &callback_cmd_vel);
  ros::Publisher cmd_vel_pub = node.advertise<geometry_msgs::Twist>("/iRobot_0/cmd_vel", 1);
  
  while(ros::ok())
  {
    geometry_msgs::Twist msg_cmd;
    msg_cmd.linear.x = vel_lin_;
    msg_cmd.angular.z = vel_ang_;
    cmd_vel_pub.publish(msg_cmd);

    ros::spinOnce();
    r_30HZ.sleep();
  }


  return 0;
};


