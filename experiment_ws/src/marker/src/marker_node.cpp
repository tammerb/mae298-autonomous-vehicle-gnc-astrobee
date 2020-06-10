#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <gazebo_msgs/ModelState.h>
#include <gazebo_msgs/SetModelState.h>
#include <std_msgs/Float32MultiArray.h>

// file operations
#include <fstream>
#include <vector> 
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    ros::init(argc, argv, "marker_node");
    ros::NodeHandle n;
    ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);

    ros::Publisher marker_loc_pub = n.advertise<std_msgs::Float32MultiArray>("marker_locs", 1000);

    tf::TransformBroadcaster broadcaster;
    ros::Rate loop_rate(60); // from Casey. Jason thinks its a tad fast
    ros::Duration half_sec(0.5);

    // make sure gazebo service is available before attempting to proceed, else node will crash
    bool service_ready = false;
    while (!service_ready) {
      service_ready = ros::service::exists("/gazebo/set_model_state",true);
      ROS_INFO("waiting for set_model_state service");
      half_sec.sleep();
    }
    ros::ServiceClient set_model_state_client = 
    n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");   
    gazebo_msgs::SetModelState model_state_srv_msg;

    // read csv
    std::ifstream  data ("/experiment_ws/src/shoulderdata.csv", std::ifstream::in);
    std::string line;
    std::vector<std::vector<float> > parsedCsv;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<float> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(atof( cell.c_str() ));
        }

        parsedCsv.push_back(parsedRow);
    }

    const int numpoints = parsedCsv.size();
    int currentpoint=0;

    const double degree = M_PI/180;

    // message declarations
    geometry_msgs::TransformStamped odom_trans;
    sensor_msgs::JointState joint_state;
    odom_trans.header.frame_id = "rviz";
    odom_trans.child_frame_id = "base_link";

    std_msgs::Float32MultiArray marker_loc;

    while (ros::ok()) {
        //update joint_state
        joint_state.header.stamp = ros::Time::now();

        // update transform
        // here we can vary the position of the marker with time
        // The constants are the middle of the jem
        std::vector<float> thisrow = parsedCsv.at(currentpoint);
        float x = 11.0 + thisrow.at(0);
        float y = -5.0 + thisrow.at(1);
        float z = 5.0 + thisrow.at(2);

        joint_state.name.resize(3);
        joint_state.position.resize(3);


        // set marker locations to publish
        // first clear the array
        marker_loc.data.clear();

        marker_loc.data.push_back(x);
        marker_loc.data.push_back(y);
        marker_loc.data.push_back(z);

        // uber NASA HAcKy
        joint_state.name[0] = "x";
        joint_state.position[0] = x;
        joint_state.name[1] = "y";
        joint_state.position[1] = y;
        joint_state.name[2] = "z";
        joint_state.position[2] = z;

        //report for rviz
        odom_trans.header.stamp = ros::Time::now();
        odom_trans.transform.translation.x = x;
        odom_trans.transform.translation.y = y;
        odom_trans.transform.translation.z = z;
        odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(0+M_PI/2);

        //set model state in gazebo
        model_state_srv_msg.request.model_state.model_name = "marker";
        model_state_srv_msg.request.model_state.pose.position.x = x;
        model_state_srv_msg.request.model_state.pose.position.y = y;
        model_state_srv_msg.request.model_state.pose.position.z = z;
        
        model_state_srv_msg.request.model_state.pose.orientation.x = 0.0;
        model_state_srv_msg.request.model_state.pose.orientation.y = 0.0;
        model_state_srv_msg.request.model_state.pose.orientation.z = 0.0;
        model_state_srv_msg.request.model_state.pose.orientation.w = 1.0;
        
        
        model_state_srv_msg.request.model_state.twist.linear.x= 0.0;
        model_state_srv_msg.request.model_state.twist.linear.y= 0.0;
        model_state_srv_msg.request.model_state.twist.linear.z= 0.0;
        
        model_state_srv_msg.request.model_state.twist.angular.x= 0.0;
        model_state_srv_msg.request.model_state.twist.angular.y= 0.0;
        model_state_srv_msg.request.model_state.twist.angular.z= 0.0;
            
        model_state_srv_msg.request.model_state.reference_frame = "world";

        set_model_state_client.call(model_state_srv_msg);

        //make sure service call was successful
        bool result = model_state_srv_msg.response.success;
        if (!result)
            ROS_WARN("service call to set_model_state failed!");

        //update what point we're at
        currentpoint++;
        if(currentpoint >= numpoints)
        {
            currentpoint = 0;
        }

        //send the joint state and transform
        joint_pub.publish(joint_state);
        broadcaster.sendTransform(odom_trans);

        // publish the marker locs
        marker_loc_pub.publish(marker_loc);

        // This will adjust as needed per iteration
        loop_rate.sleep();
    }


    return 0;
}