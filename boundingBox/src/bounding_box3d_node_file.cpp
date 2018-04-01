#include <ros/ros.h>
#include <fstream>
#include "vision_msgs/Detection2DArray.h"
using namespace std;


void objects_cb (const vision_msgs::Detection2DArray& msg){
   if (msg.detections.size()!=0){
      if (msg.detections[0].results[0].id==47){        
      int u= msg.detections[0].bbox.center.x;
      int v= msg.detections[0].bbox.center.y;
      ofstream myfile;
      myfile.open ("example.txt");
      myfile <<u<<" "<<v<<std::endl;
      //myfile <<v<<std::endl;
      myfile.close();
   }
   }  
   else
   {
     std::cout<<"No Object found"<<std::endl;  
   }   
   
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "bounding_box3d_node_file");
   ros::NodeHandle nh;
 
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/objects", 1, objects_cb);
   
 
   // Spin
   ros::spin ();
}