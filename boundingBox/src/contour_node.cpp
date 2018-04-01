#include <ros/ros.h>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <geometry_msgs/Point.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <fstream>
#include <sensor_msgs/image_encodings.h>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
ros::Publisher pub_contour;
namespace enc = sensor_msgs::image_encodings;
void find_corners(const sensor_msgs::ImageConstPtr& msg,int u, int v,int off,boundingBox::point_array&  arry){

  int x=u-off/2;
  int y=v-off/2;
  cv_bridge::CvImagePtr cv_ptr;
  try{
   cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
  }
  catch (cv_bridge::Exception& e){
   ROS_ERROR("cv_bridge exception: %s", e.what());
   return;
  }
  
  cv::Mat ROI(cv_ptr->image, cv::Rect(x,y,off,off));
  cv::Mat img;
  ROI.copyTo(img);
  int minHessian = 400;
  std::vector<KeyPoint> keypoints;
  Ptr<SURF> detector = SURF::create( minHessian );
  detector->detect( img, keypoints );
  for(int i=0; i < keypoints.size(); i++){
      geometry_msgs::Point p;
      p.x=keypoints[i].pt.x+x;
      p.y=keypoints[i].pt.y+y;
      p.z=0;
      arry.push_back(p);
  }

}
void cloud_cb (const sensor_msgs::ImageConstPtr& msg){
      boundingBox::point_array arry;
      geometry_msgs::Point p
      ifstream inFile;
      int arr[2];
      int i=0;
      int x;
      std::cout<<"Display 1.0"<<std::endl;
      inFile.open ("example.txt");
      std::cout<<"Display 2.0"<<std::endl;
      if (!inFile) {
        p.x=0;
        p.y=0;
        p.z=0;   // call system to stop
        arry.push_back(p);
      }
      else{
        while (inFile>>x) {
          arr[i]=x;
          std::cout<<x<<std::endl;
          i++;
          if (i==2){
            //inFile.close();
            break;
        }
      }
      std::cout<<"Display 3.0"<<std::endl;
      find_corners(msg,arr[0],arr[1],40,arry);
      std::cout<<"Display 4.0"<<std::endl;
      }
      
      pub_contour.publish(p); 
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "contour_node");
   ros::NodeHandle nh;
   
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/kinect2/sd/points", 1, cloud_cb);
 
   // Create a ROS publisher for the output point cloud
   pub_contour = nh.advertise<boundingBox::point_array> ("/objects/contour", 1);
 
   // Spin
   ros::spin ();
}