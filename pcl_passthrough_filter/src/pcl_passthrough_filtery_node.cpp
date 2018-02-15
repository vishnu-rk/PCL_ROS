#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <iostream>
#include <pcl/filters/passthrough.h>


ros::Publisher pub_passthrough_y;


void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud_msg){
// Create a container for the data.
  sensor_msgs::PointCloud2 output;
  pcl::PCLPointCloud2 cloud_filteredy;  
  pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;
  pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);   

  pcl_conversions::toPCL(*cloud_msg, *cloud);
  pcl::PassThrough<pcl::PCLPointCloud2> passy;
  passy.setInputCloud(cloudPtr);
  passy.setFilterFieldName ("y");
  passy.setFilterLimits (-1, 2);
  passy.filter (cloud_filteredy);
  pcl_conversions::fromPCL(cloud_filteredy, output);
  pub_passthrough_y.publish (output);
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "pcl_passthroughy_filter");
   ros::NodeHandle nh;
 
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/kinect2/sd/cloud_filteredx", 1, cloud_cb);
 
   // Create a ROS publisher for the output point cloud
   pub_passthrough_y = nh.advertise<sensor_msgs::PointCloud2> ("/kinect2/sd/cloud_filteredy", 1);
 
   // Spin
   ros::spin ();
 }