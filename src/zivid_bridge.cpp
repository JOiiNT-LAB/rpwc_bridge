#include <zivid_bridge.h>


zivid_bridge::zivid_bridge()
{
  ros::NodeHandle n;
  const ros::Duration default_wait_duration{ 30 };

  ROS_INFO("Starting zivid_bridge.cpp");

  CHECK(ros::service::waitForService(ca_suggest_settings_service_name_, default_wait_duration));

  ROS_INFO("Configuring image settings");
  capture_assistant_suggest_settings();

  flag_pc2_ = false;
  flag_img_ = false;

  // Subscibers
  points_sub_             = n.subscribe("/zivid_camera/points/xyzrgba", 1, &zivid_bridge::on_points, this);
  image_color_sub_        = n.subscribe("/zivid_camera/color/image_color", 1, &zivid_bridge::on_image_color, this);
  // Publishers
  points_pub_ 		        = n.advertise<sensor_msgs::PointCloud2>("/camera/depth_registered/points", 1);
  image_color_pub_ 	      = n.advertise<sensor_msgs::Image>("/camera/color/image_raw", 1);
  // Servers
  server_camera_capture_  = n.advertiseService("rpwc_camera_data", &zivid_bridge::callback_camera_data, this);
}

zivid_bridge::~zivid_bridge(){}


void zivid_bridge::on_points(const sensor_msgs::PointCloud2::ConstPtr& msg)
{
  ROS_INFO("PointCloud received");
  msg_pc2_  = *msg;
  points_pub_.publish(msg_pc2_);
  flag_pc2_ = true;
}

void zivid_bridge::on_image_color(const sensor_msgs::Image::ConstPtr& msg)
{
  ROS_INFO("2D color image received");
  cv_bridge::CvImagePtr cv_ptr;
  cv_ptr   = cv_bridge::toCvCopy(msg, "rgb8");
  msg_img_ = cv_bridge::CvImage(std_msgs::Header(), "rgb8", cv_ptr->image).toImageMsg();
  image_color_pub_.publish(*msg_img_);
  flag_img_ = true;
}

bool zivid_bridge::callback_camera_data(rpwc_bridge::CameraData::Request &req, rpwc_bridge::CameraData::Response &res)
{
  capture();

  while (!flag_pc2_ || !flag_img_)
  {
    usleep(1000000);
    ros::spinOnce();
  }

  flag_pc2_ = false;
  flag_img_ = false;

  res.cloud = msg_pc2_;
  res.image = *msg_img_;

  return true;
}

void zivid_bridge::capture()
{
  ROS_INFO("Calling capture service");
  zivid_camera::Capture capture;
  CHECK(ros::service::call("/zivid_camera/capture", capture));
}

void zivid_bridge::capture_assistant_suggest_settings()
{
  zivid_camera::CaptureAssistantSuggestSettings cass;
  cass.request.max_capture_time = ros::Duration{ 1.20 };
  cass.request.ambient_light_frequency =
      zivid_camera::CaptureAssistantSuggestSettings::Request::AMBIENT_LIGHT_FREQUENCY_NONE;

  ROS_INFO_STREAM("Calling " << ca_suggest_settings_service_name_
                             << " with max capture time = " << cass.request.max_capture_time << " sec");
  CHECK(ros::service::call(ca_suggest_settings_service_name_, cass));
}