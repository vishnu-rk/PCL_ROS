#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <iostream>
#include <pcl/filters/passthrough.h>


ros::Publisher pub_passthrough;


void cloud_cb (const pcl::PCLPointCloud2ConstPtr& cloud_msg){

  pcl::PCLPointCloud2* cloudz = new pcl::PCLPointCloud2;
  pcl::PCLPointCloud2ConstPtr cloudPtrz(cloudz);   
  pcl::PCLPointCloud2* cloudy = new pcl::PCLPointCloud2;
  pcl::PCLPointCloud2ConstPtr cloudPtry(cloudy);
  pcl::PCLPointCloud2* cloudx = new pcl::PCLPointCloud2;
  pcl::PCLPointCloud2ConstPtr cloudPtrx(cloudx);

  pcl::PassThrough<pcl::PCLPointCloud2> pass;
  pass.setInputCloud(cloud_msg);
  pass.setFilterFieldName ("z");
  pass.setFilterLimits (0, 2);
  pass.filter (*cloudz);

  pass.setInputCloud(cloudPtrz);
  pass.setFilterFieldName ("y");
  pass.setFilterLimits (-1, 1);
  pass.filter (*cloudy);

  pass.setInputCloud(cloudPtry);
  pass.setFilterFieldName ("x");
  pass.setFilterLimits (-0.1, 0.1);
  pass.filter (*cloudx);
  //pcl_conversions::fromPCL(cloud_filteredz, output);
  pub_passthrough.publish (*cloudPtrx);
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "pcl_passthrough_filter");
   ros::NodeHandle nh;
 
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/kinect2/sd/points/pcl2rviz_display", 1, cloud_cb);
 
   // Create a ROS publisher for the output point cloud
   pub_passthrough = nh.advertise<sensor_msgs::PointCloud2> ("/kinect2/sd/cloud_filtered", 1);
 
   // Spin
   ros::spin ();
 }