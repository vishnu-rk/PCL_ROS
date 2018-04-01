#include <ros/ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Float32MultiArray.h>
#include <fstream>
#include "vision_msgs/Detection2DArray.h"
using namespace std;

ros::Publisher pub_2d_bounding_box;

void objects_cb (const vision_msgs::Detection2DArray& msg){
// Create a container for the data.
   std_msgs::Float32MultiArray output;
   output.data.clear();
   if (msg.detections.size()!=0){
      for(int i=0;i<msg.detections.size();i++){
         for(int j=0;j<msg.detections[i].results.size();j++){
         if (msg.detections[i].results[j].id==44){
            float u=msg.detections[i].bbox.center.x/540*960.0;//2.11;
            float v=msg.detections[i].bbox.center.y/960*540.0;///3.75;
            float s1=msg.detections[i].bbox.size_x;
            float s2=msg.detections[i].bbox.size_y;
            output.data.push_back(msg.detections[i].results[j].id);     
            output.data.push_back(u);
            output.data.push_back(v);
            output.data.push_back(s1);
            output.data.push_back(s2);
            ofstream myfile;
            myfile.open ("example.txt");
            myfile <<(int)u<<" "<<(int)v<<" "<<(int)s1<<" "<<(int)s2<<std::endl;
            //myfile <<v<<std::endl;
            myfile.close();
            pub_2d_bounding_box.publish (output);
      }
      }
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