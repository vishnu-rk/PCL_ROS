#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <iostream>
using namespace std;

image_transport::Publisher pub_crop ;
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv::Mat image;
  cv::Mat cropped;
  sensor_msgs::ImagePtr msg1;
  ifstream inFile;
  int arr[4];
  int i=0;
  int x;
  try
  {
    image = cv_bridge::toCvShare(msg, "bgr8")->image;
    int height=image.size().height;
    int width=image.size().width;
    // cout<<height<<" "<<width<<endl;

    inFile.open ("example.txt");
    while (inFile>>x) {
          arr[i]=(int)x;
          // std::cout<<x<<std::endl;
          i++;
          if (i==4){
            //inFile.close();
            break;
        }
      }
    inFile.close();
    cv::Rect myROI((int)(arr[0]-arr[2]/2.0), (int)(arr[1]-arr[3]/2.0), arr[2], arr[3]);
    cropped=image(myROI);
    msg1 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", cropped).toImageMsg();
    pub_crop.publish(msg1);   
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  // cv::namedWindow("view");
  // cv::startWindowThread();
  pub_crop = it.advertise("kinect2/cropped_image/bounding_box", 1);
  image_transport::Subscriber sub = it.subscribe("kinect2/cropped_image", 1, imageCallback);
  ros::spin();
  // cv::destroyWindow("view");
}
