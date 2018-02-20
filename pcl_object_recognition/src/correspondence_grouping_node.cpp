
#include <pcl_correspondence_grouping.h>
#include <ros/package.h> 

string test_model;
//string test_scene;
//ros::Publisher pub_passthrough;

std::string find_in_package(std::string filename) {
	string pkg_path = ros::package::getPath("pcl_object_recognition");
	string append = "/pcd/";
	string full_path = pkg_path+append+filename;
	return full_path;
}
// void load_coke1_parameters(object_recognizer& object)
// {
//     // the paramters for coke_bad and coke_scene
//     test_model = "coke_bad.pcd";
//     test_scene = "coke_scene.pcd";

//     object.set_model_ss(0.01);
//     object.set_scene_ss(0.01);
//     object.set_cg_size(0.09);
//     object.set_cg_thresh(2.9);
// }


// void load_coke2_parameters(object_recognizer& object)
// {
//     //the parameter for coke_2 and new_coke
//     test_model = "new_coke.pcd";
//     test_scene = "coke_2.pcd";

//     object.set_model_ss(0.01);
//     object.set_scene_ss(0.01);
//     object.set_cg_size(0.13);
//     object.set_cg_thresh(45);
// }


int main(int argc, char** argv) {
    ros::init(argc, argv, "object_recognizer"); //node name

    ros::NodeHandle nh;
    object_recognizer object(nh);
    geometry_msgs::Pose object_pose;
    ros::Publisher objectRT_publisher;
    objectRT_publisher= nh.advertise<geometry_msgs::Pose>( "/objectRT_main", 1, true );
    //load_coke2_parameters(object);
    test_model = "bottle.pcd";
    //test_scene = "coke_scene.pcd";
    object.set_model_ss(0.01);
    object.set_scene_ss(0.05);
    object.set_cg_size(0.01);
    object.set_cg_thresh(7.0);
    object.set_model_cloud(find_in_package(test_model));

    if (object.find_best(object_pose)){
        std::cout<<"found best solution"<<endl;
        objectRT_publisher.publish(object_pose);
    }
    else{
        std::cout<<"did not find soultion"<<std::endl;
    }

   // Spin
    ros::spin();

}
