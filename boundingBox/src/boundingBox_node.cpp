#include <ros/ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Float32MultiArray.h>
#include "vision_msgs/Detection2DArray.h"


ros::Publisher pub_2d_bounding_box;

void objects_cb (const vision_msgs::Detection2DArray& msg){
// Create a container for the data.
   std_msgs::Float32MultiArray output;
   output.data.clear();
   if (msg.detections.size()!=0){
      if (msg.detections[0].results[0].id==47){
      output.data.push_back(msg.detections[0].results[0].id);     
      output.data.push_back(msg.detections[0].bbox.center.x);
      output.data.push_back(msg.detections[0].bbox.center.y);
      output.data.push_back(msg.detections[0].bbox.size_x);
      output.data.push_back(msg.detections[0].bbox.size_y);
      pub_2d_bounding_box.publish (output);
   }
   }
  
   else
   {
      output.data.push_back(0.0);
      pub_2d_bounding_box.publish (output);   
   }
   
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "boundingBox_node");
   ros::NodeHandle nh;
 
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/objects", 1, objects_cb);
 
   // Create a ROS publisher for the output point cloud
   pub_2d_bounding_box = nh.advertise<std_msgs::Float32MultiArray> ("/objects/PoseArray", 1);
 
   // Spin
   ros::spin ();
}