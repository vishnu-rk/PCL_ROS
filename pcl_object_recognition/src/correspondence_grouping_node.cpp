
#include <pcl_correspondence_grouping.h>
#include <ros/package.h> 

string test_model;


std::string find_in_package(std::string filename) {
	string pkg_path = ros::package::getPath("pcl_object_recognition");
	string append = "/pcd/";
	string full_path = pkg_path+append+filename;
	return full_path;
}



int main(int argc, char** argv) {
    ros::init(argc, argv, "object_recognizer"); //node name

    ros::NodeHandle nh;
    object_recognizer object(nh);
    geometry_msgs::Pose object_pose;
    ros::Publisher objectRT_publisher;
    objectRT_publisher= nh.advertise<geometry_msgs::Pose>( "/objectRT_main", 2, true );
    
    test_model = "bottle.pcd";
    //label:
    // object.set_model_ss(0.01);
    // object.set_scene_ss(0.05);
    // object.set_cg_size(0.01);
    // object.set_cg_thresh(7.0);
    object.set_model_cloud(find_in_package(test_model));
    while (true){
        bool valt=object.find_best(object_pose);
        if (valt){
            break;            
        }            
        else {
            ros::spinOnce();
            ros::Duration(1.0).sleep();
            }
    
    }

    //if (object.find_best(object_pose)){
    objectRT_publisher.publish(object_pose);
    object.pcl_visualize();
    std::cout<<"found best solution"<<endl;
   // Spin
    ros::spin();
    //goto label;

}
