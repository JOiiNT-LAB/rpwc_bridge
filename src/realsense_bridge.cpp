#include <realsense_bridge.h>


realsense_bridge::realsense_bridge()
{
  ros::NodeHandle n;

  ROS_INFO("Starting realsense_bridge.cpp");

  flag_pc2_ = false;
  flag_img_ = false;

  // Subscibers
  points_sub_             = n.subscribe("/camera/depth_registered/points", 1, &realsense_bridge::on_points, this);
  image_color_sub_        = n.subscribe("/camera/color/image_raw", 1, &realsense_bridge::on_image_color, this);
  // Servers
  server_camera_capture_  = n.advertiseService("rpwc_camera_data", &realsense_bridge::callback_camera_data, this);
}

realsense_bridge::~realsense_bridge(){}


void realsense_bridge::on_points(const sensor_msgs::PointCloud2::ConstPtr& msg)
{
  msg_pc2_  = *msg;
  flag_pc2_ = true;
}

void realsense_bridge::on_image_color(const sensor_msgs::Image::ConstPtr& msg)
{
  msg_img_  = *msg;
  flag_img_ = true;
}

bool realsense_bridge::callback_camera_data(rpwc_bridge::CameraData::Request &req, rpwc_bridge::CameraData::Response &res)
{
  while (!flag_pc2_ || !flag_img_)
  {
    usleep(1000000);
    ros::spinOnce();
  }

  flag_pc2_ = false;
  flag_img_ = false;

  res.cloud = msg_pc2_;
  res.image = msg_img_;

  return true;
}