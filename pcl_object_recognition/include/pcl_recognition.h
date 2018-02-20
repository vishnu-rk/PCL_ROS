#ifndef PCL_UTILS_H_
#define PCL_UTILS_H_

#include <ros/ros.h> //generic C++ stuff
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>

#include <Eigen/Eigen> //for the Eigen library
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>

#include <sensor_msgs/PointCloud2.h> //useful ROS message types
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/TransformStamped.h>

#include <tf/transform_listener.h>  // transform listener headers
#include <tf/transform_broadcaster.h>

#include <pcl_ros/point_cloud.h>
#include <pcl/ros/conversions.h>
#include <pcl_ros/transforms.h>

#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>  //point-cloud library headers; likely don't need all these
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/correspondence.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/features/shot_omp.h>
#include <pcl/features/board.h>
#include <pcl/keypoints/uniform_sampling.h>
#include <pcl/recognition/cg/hough_3d.h>
#include <pcl/recognition/cg/geometric_consistency.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/kdtree/impl/kdtree_flann.hpp>
#include <pcl/common/transforms.h>
#include <pcl/common/common_headers.h>
#include <pcl/console/parse.h>


using namespace std;  //just to avoid requiring std::, Eigen:: ...
using namespace Eigen;
using namespace pcl;
using namespace pcl::io;

// define a class, including a constructor, member variables and member functions
class PclUtils
{
public:
    PclUtils(ros::NodeHandle* nodehandle); //constructor

     // insert doxygen documentation of member fncs;  run "doxywizard" to create documentation

     // this fnc is a copy of plane-fitter from exmaple_ros_library
     /**provide an array of 3-D points (in columns), and this function will use and eigen-vector approach to find the best-fit plane
     * It returns the plane's normal vector and the plane's (signed) distance from the origin.
     * @param points_array input: points_array is a matrix of 3-D points to be plane-fitted; coordinates are in columns
     * @param plane_normal output: this function will compute components of the plane normal here
     * @param plane_dist output: scalar (signed) distance of the plane from the origin
     */
    void fit_points_to_plane(Eigen::MatrixXf points_array, 
        Eigen::Vector3f &plane_normal, 
        double &plane_dist); 
    Eigen::Vector3f compute_centroid(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud_ptr);
    Eigen::Vector3f  compute_centroid(pcl::PointCloud<pcl::PointXYZ> &input_cloud);
    
    void fit_points_to_plane(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud_ptr,Eigen::Vector3f &plane_normal, double &plane_dist);

// 
// 
// 
    /**a utility fnc to convert tf::Transform type into an Eigen::Affine3f
     * Affine type "f" is needed for use with point clouds, since "floats" not "doubles" are more practical, considering
     * the expected (limited) resolution of the sensor, as well as the large size of point clouds
     * @param t  [in] provide a transform, e.g. per:
     *     g_tfListenerPtr->lookupTransform("torso", "kinect_pc_frame", ros::Time(0), tf_sensor_frame_to_torso_frame);
     * @return an Eigen Affine object, A, such that point_in_new_frame = A*point_in_original_frame
     */    
    Eigen::Affine3f transformTFToEigen(const tf::Transform &t);
    
    /** function to create an Eigen-style Affine transform based on construction of a coordinate frame
     * placed on the surface of a plane
     */
    Eigen::Affine3f make_affine_from_plane_params(Eigen::Vector3f plane_normal, double plane_dist);
    /**
     * returns an Eigen::Affine transform to a coordinate frame constructed on a plane defined by
     * plane_parameters (normal_x, normal_y, normal_z, distance)
     * useful for transforming data to find planar surfaces with z-axis vertical
     */
    Eigen::Affine3f make_affine_from_plane_params(Eigen::Vector4f plane_parameters);


    void transform_kinect_cloud(Eigen::Affine3f A);
    void transform_selected_points_cloud(Eigen::Affine3f A);
    void transform_cloud(Eigen::Affine3f A,pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud_ptr, 
        pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud_ptr); 
    //color version:
    void transform_cloud(Eigen::Affine3f A, pcl::PointCloud<pcl::PointXYZRGB>::Ptr input_cloud_ptr,
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr output_cloud_ptr);
    void reset_got_kinect_cloud() {got_kinect_cloud_= false;};
    void reset_got_selected_points() {got_selected_points_= false;};    
    bool got_kinect_cloud() { return got_kinect_cloud_; };
    bool got_selected_points() {return got_selected_points_;};
    void save_kinect_snapshot(string filename) {    pcl::io::savePCDFileASCII (filename, *pclKinect_ptr_);}; //B/W
    int read_pcd_file(string fname); 
    int read_clr_pcd_file(string fname);
    
    //alternative "save" fnc: save as a colored pointcloud
    void save_kinect_clr_snapshot(string filename) {pcl::io::savePCDFileASCII (filename, *pclKinect_clr_ptr_);};
    int  save_kinect_clr_snapshot_binary(string filename) {return(pcl::io::savePCDFile (filename, *pclKinect_clr_ptr_,true));};

    void save_transformed_kinect_snapshot(string filename) { pcl::io::savePCDFileASCII (filename, *pclTransformed_ptr_);};
    void get_transformed_selected_points(pcl::PointCloud<pcl::PointXYZ> & outputCloud );
    void get_copy_selected_points(pcl::PointCloud<pcl::PointXYZ>::Ptr &outputCloud );
    
    void copy_cloud(PointCloud<pcl::PointXYZ>::Ptr inputCloud, PointCloud<pcl::PointXYZ>::Ptr outputCloud); 
    void copy_cloud_xyzrgb_indices(PointCloud<pcl::PointXYZRGB>::Ptr inputCloud, vector<int> &indices, PointCloud<pcl::PointXYZRGB>::Ptr outputCloud); 

    void get_indices(vector<int> &indices) {   indices = indices_;};
    //same as above, but assumes 
    void copy_indexed_pts_to_output_cloud(vector<int> &indices,PointCloud<pcl::PointXYZRGB> &outputCloud);

    void get_gen_purpose_cloud(pcl::PointCloud<pcl::PointXYZ> & outputCloud ); 
    void get_kinect_points(pcl::PointCloud<pcl::PointXYZ> & outputCloud );
    void get_kinect_points(pcl::PointCloud<pcl::PointXYZ>::Ptr &outputCloudPtr );
    void get_kinect_points(pcl::PointCloud<pcl::PointXYZRGB> & outputCloudPtr );
    void get_kinect_points(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &outputCloud );
    void get_selected_points(pcl::PointCloud<pcl::PointXYZ>::Ptr &outputCloudPtr );
    void get_selected_points(pcl::PointCloud<pcl::PointXYZ> &outputCloud);

    //operate on transformed Kinect data and identify point indices within +/-z_eps of specified height
    void filter_cloud_z(PointCloud<pcl::PointXYZ>::Ptr inputCloud, double z_nom, double z_eps, vector<int> &indices);
    void filter_cloud_z(PointCloud<pcl::PointXYZRGB>::Ptr inputCloud, double z_nom, double z_eps, vector<int> &indices);    
    // as above, specifically for transformed kinect cloud:
    void find_coplanar_pts_z_height(double plane_height,double z_eps,vector<int> &indices);
    // find pts within +/- z_eps of z_height, AND within "radius" of "centroid"
    void filter_cloud_z(PointCloud<pcl::PointXYZ>::Ptr inputCloud, double z_nom, double z_eps, 
                double radius, Eigen::Vector3f centroid, vector<int> &indices);
    //same as above, but specifically operates on transformed kinect cloud
    void filter_cloud_z(double z_nom, double z_eps, 
                double radius, Eigen::Vector3f centroid, vector<int> &indices);   
    
    void analyze_selected_points_color();
    
    Eigen::Vector3f get_centroid() { return centroid_; };
    Eigen::Vector3f get_major_axis() { return major_axis_; };
    Eigen::Vector3f get_patch_normal() { return patch_normal_;};
    double get_patch_dist() {return patch_dist_;};
    Eigen::Vector3d find_avg_color();
    Eigen::Vector3d find_avg_color_selected_pts(vector<int> &indices);
    void find_indices_color_match(vector<int> &input_indices,
                    Eigen::Vector3d normalized_avg_color,
                    double color_match_thresh, vector<int> &output_indices);    

private:
    ros::NodeHandle nh_; 
    // some objects to support subscriber, service, and publisher
    ros::Subscriber pointcloud_subscriber_; //use this to subscribe to a pointcloud topic
    ros::Subscriber real_kinect_subscriber_; //use this to subscribe to a physical kinect device
    
    ros::Subscriber selected_points_subscriber_; // this to subscribe to "selectedPoints" topic from Rviz

    
    //ros::ServiceServer minimal_service_; //maybe want these later
    ros::Publisher  pointcloud_publisher_;
    ros::Publisher patch_publisher_;    
    
    
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr pclKinect_clr_ptr_; //pointer for color version of pointcloud
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr pclSelectedPtsClr_ptr_; //pointer for color version of pointcloud

    pcl::PointCloud<pcl::PointXYZ>::Ptr pclKinect_ptr_; //(new PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr pclTransformed_ptr_;
    pcl::PointCloud<pcl::PointXYZ>::Ptr pclSelectedPoints_ptr_;
    pcl::PointCloud<pcl::PointXYZ>::Ptr pclTransformedSelectedPoints_ptr_;
    pcl::PointCloud<pcl::PointXYZ>::Ptr pclGenPurposeCloud_ptr_;
    
    bool got_kinect_cloud_;
    bool got_selected_points_;
    // member methods as well:
    void initializeSubscribers(); // we will define some helper methods to encapsulate the gory details of initializing subscribers, publishers and services
    void initializePublishers();
    //void initializeServices();
    
    void kinectCB(const sensor_msgs::PointCloud2ConstPtr& cloud); //prototype for callback fnc
    void selectCB(const sensor_msgs::PointCloud2ConstPtr& cloud); // callback for selected points  
    
    Eigen::Vector3f major_axis_,centroid_; 
    Eigen::Vector3d avg_color_;
    Eigen::Vector3f patch_normal_;
    double patch_dist_;
    vector<int> indices_; // put interesting indices here
    //prototype for example service
    //bool serviceCallback(example_srv::simple_bool_service_messageRequest& request, example_srv::simple_bool_service_messageResponse& response);

}; // note: a class definition requires a semicolon at the end of the definition

#endif  // this closes the header-include trick...ALWAYS need one of these to match #ifndef
