#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

image_transport::Publisher publish_cropped ;
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv::Mat image;
  cv::Mat cropped;
  sensor_msgs::ImagePtr msg1;
  try
  {
    image = cv_bridge::toCvShare(msg, "bgr8")->image;
    int height=image.size().height;
    int width=image.size().width;
    cv::Rect myROI(width-3.0/4*width, height-3.0/4*height, width/2.0, height/2.0);
    cropped=image(myROI);
    msg1 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", cropped).toImageMsg();
    publish_cropped.publish(msg1);   
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  // cv::namedWindow("view");
  // cv::startWindowThread();
  publish_cropped = it.advertise("kinect2/cropped_image", 1);
  image_transport::Subscriber sub = it.subscribe("/kinect2/hd/image_color", 1, imageCallback);
  ros::spin();
  // cv::destroyWindow("view");
}
