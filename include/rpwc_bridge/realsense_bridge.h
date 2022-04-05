#include <ros/ros.h>

#include <rpwc_bridge/CameraData.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud2.h>


#define CHECK(cmd)                                                                                                     \
  do                                                                                                                   \
  {                                                                                                                    \
    if (!cmd)                                                                                                          \
    {                                                                                                                  \
      throw std::runtime_error{ "\"" #cmd "\" failed!" };                                                              \
    }                                                                                                                  \
  } while (false)

class realsense_bridge
{
public:
	realsense_bridge();
	~realsense_bridge();

  	double dt_;

private:
	void on_points(const sensor_msgs::PointCloud2::ConstPtr& msg);
	void on_image_color(const sensor_msgs::Image::ConstPtr& msg);
	bool callback_camera_data(rpwc_bridge::CameraData::Request &req, rpwc_bridge::CameraData::Response &res);

	ros::Subscriber 			points_sub_, image_color_sub_;
	ros::ServiceServer 			server_camera_capture_;

	sensor_msgs::PointCloud2	msg_pc2_;
	sensor_msgs::Image			msg_img_;

	bool 						flag_pc2_;
	bool 						flag_img_;
};