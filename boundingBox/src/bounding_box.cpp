#include <ros/ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include "vision_msgs/Detection2DArray.h"


ros::Publisher pub_display;

void objects_cb (const vision_msgs::Detection2DArray& msg){
// Create a container for the data.
   std_msgs::Float32MultiArray output;
   output.data.clear();
   if(msg.results[0].id==47){
      output.data.push_back(47.0);
      output.data.push_back(bbox.center.x);
      output.data.push_back(bbox.center.y);
      output.data.push_back(bbox.size_x);
      output.data.push_back(bbox.size_y);
   }
  
   // Publish the data.
   pub_display.publish (output);
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "bounding_box");
   ros::NodeHandle nh;
 
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/objects", 1, objects_cb);
 
   // Create a ROS publisher for the output point cloud
   pub_display = nh.advertise<std_msgs::Float32MultiArray> ("/objects/PoseArray", 1);
 
   // Spin
   ros::spin ();
}