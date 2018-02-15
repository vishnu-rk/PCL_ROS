#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <iostream>
#include <pcl/filters/passthrough.h>


ros::Publisher pub_passthrough_x;


void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud_msg){
// Create a container for the data.
  sensor_msgs::PointCloud2 output;
  pcl::PCLPointCloud2 cloud_filteredx;  
  pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;
  pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);   

  pcl_conversions::toPCL(*cloud_msg, *cloud);
  pcl::PassThrough<pcl::PCLPointCloud2> passx;
  passx.setInputCloud(cloudPtr);
  passx.setFilterFieldName ("x");
  passx.setFilterLimits (-1, 1);
  passx.filter (cloud_filteredx);
  pcl_conversions::fromPCL(cloud_filteredx, output);
  pub_passthrough_x.publish (output);
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "pcl_passthroughx_filter");
   ros::NodeHandle nh;
 
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/kinect2/sd/points/pcl2rviz_display", 1, cloud_cb);
 
   // Create a ROS publisher for the output point cloud
   pub_passthrough_x = nh.advertise<sensor_msgs::PointCloud2> ("/kinect2/sd/cloud_filteredx", 1);
 
   // Spin
   ros::spin ();
 }