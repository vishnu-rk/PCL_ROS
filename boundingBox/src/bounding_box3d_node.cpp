#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <iostream>
#include <geometry_msgs/Point.h>
using namespace std;

ros::Publisher pub_3d_bounding_box;
void pixelTo3DPoint(const sensor_msgs::PointCloud2& pCloud, const int u, const int v, geometry_msgs::Point &p)
    {
      // get width and height of 2D point cloud data
      int width = pCloud.width;
      int height = pCloud.height;
      std::cout<<"Display 4.0"<<std::endl;
      // Convert from u (column / width), v (row/height) to position in array
      // where X,Y,Z data starts
      int arrayPosition = v*pCloud.row_step + u*pCloud.point_step;

      // compute position in array where x,y,z data start
      int arrayPosX = arrayPosition + pCloud.fields[0].offset; // X has an offset of 0
      int arrayPosY = arrayPosition + pCloud.fields[1].offset; // Y has an offset of 4
      int arrayPosZ = arrayPosition + pCloud.fields[2].offset; // Z has an offset of 8
      std::cout<<"Display 5.0 "<<arrayPosX<<" "<<arrayPosY<<" "<<arrayPosZ<<" "<<std::endl;
      float X = 0.0;
      float Y = 0.0;
      float Z = 0.0;
      std::cout<<"Display 6.0"<<std::endl;
      memcpy(&X, &pCloud.data[arrayPosX], sizeof(float));
      memcpy(&Y, &pCloud.data[arrayPosY], sizeof(float));
      memcpy(&Z, &pCloud.data[arrayPosZ], sizeof(float));
      // X=pCloud.data[arrayPosX];
      std::cout<<"Display 7.0"<<std::endl;

     // put data into the point p
      p.x = X;
      p.y = Y;
      p.z = Z;
      std::cout<<"Display 8.0"<<std::endl;
    }

void cloud_cb (const sensor_msgs::PointCloud2& pCloud){
      geometry_msgs::Point p;
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
      pixelTo3DPoint(pCloud,arr[0],arr[1],p);
      std::cout<<"Display 4.0"<<std::endl;
      }
      
      pub_3d_bounding_box.publish(p); 
 }
 
 int main (int argc, char** argv)
 {
   // Initialize ROS
   ros::init (argc, argv, "pcl_passthrough_filter");
   ros::NodeHandle nh;
 
   // Create a ROS subscriber for the input point cloud
   ros::Subscriber sub = nh.subscribe ("/kinect2/sd/points", 1, cloud_cb);
 
   // Create a ROS publisher for the output point cloud
   pub_3d_bounding_box = nh.advertise<geometry_msgs::Point> ("/objects/3d", 1);
 
   // Spin
   ros::spin ();
}