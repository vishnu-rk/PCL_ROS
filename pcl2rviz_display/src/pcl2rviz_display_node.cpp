#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

ros::Publisher pub_display;

void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud_msg){
// Create a container for the data.
   sensor_msgs::PointCloud2 output;
   // Do data processing here...
   //output = *cloud_msg;

   //msg->header.frame_id = "some_tf_frame";
   pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;
   pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
   //pcl::PCLPointCloud2 cloud_filtered;

   pcl_conversions::toPCL(*cloud_msg, *cloud);
   cloud->header.frame_id = "frames/kinect2";
   pcl_conversions::fromPCL(*cloud, output);
 
   // Publish the data.
   pub_display.publish (output);
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "pcl2rviz_display");
   ros::NodeHandle nh;
 
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/kinect2/hd/points", 1, cloud_cb);
 
   // Create a ROS publisher for the output point cloud
   pub_display = nh.advertise<sensor_msgs::PointCloud2> ("/kinect2/hd/points/pcl2rviz_display", 1);
 
   // Spin
   ros::spin ();
 }