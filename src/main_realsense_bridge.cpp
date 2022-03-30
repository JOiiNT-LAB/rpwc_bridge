#include <ros/ros.h>
#include <ros/rate.h>

// my headers
#include "realsense_bridge.h"

//-----------------------------------------------------
//                                                 main
//-----------------------------------------------------
int main(int argc, char **argv)
{
	ros::init(argc, argv, "realsense_bridge_node");
	realsense_bridge Obj;
  	double rate_100Hz = 100.0;
	ros::Rate r_100HZ(rate_100Hz);
	
	Obj.dt_ = 1.0/rate_100Hz;
	
    ROS_INFO("Realsense bridge node on. Ready to receive response");

	ros::spin();

	return 0;
}