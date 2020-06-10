#include <string>
#include <ros/ros.h>
#include <ff_msgs/AckCompletedStatus.h>
#include <ff_msgs/AckStamped.h>
#include <ff_msgs/AckStatus.h>
#include <ff_msgs/AgentStateStamped.h>
#include <ff_msgs/CommandArg.h>
#include <ff_msgs/CommandConstants.h>
#include <ff_msgs/CommandStamped.h>
#include <ff_msgs/DockAction.h>
#include <ff_msgs/DockState.h>
#include <ff_msgs/FaultState.h>
#include <ff_msgs/MotionAction.h>
#include <ff_msgs/PerchState.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Float32MultiArray.h>
#include <PublisherSubscriber.h>

// file operations
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

class Position {
  private:
    float marker_x;
    float marker_y;
    float marker_z;

  public:
    void setPosition(float pos_array[3]) {
      marker_x = pos_array[0];
      marker_y = pos_array[1];
      marker_z = pos_array[2];
    }

    float get_x(){
      return marker_x;
    }
    float get_y(){
      return marker_y;
    }
    float get_z(){
      return marker_z;
    }
};

Position pos_global;




void MarkerLocsCallback(std_msgs::Float32MultiArray::ConstPtr const& msg) {

  float x = msg->data[0];
  float y = msg->data[1];
  float z = msg->data[2];
  ROS_INFO("\n I can haz marker loc data? \n ");
  ROS_INFO("Data x = %f\n", msg->data[0]);

  // float pos_array [] = {11, -5, 4.7};
  // pos_global.setPosition(pos_array);
}


void MoveFeedbackCallback(ff_msgs::MotionActionFeedbackConstPtr const& fb) {
  std::cout << '\r' << std::flush;
  std::cout << std::fixed << std::setprecision(2)
    << "pos: x: " << fb->feedback.progress.setpoint.pose.position.x
    << " y: " << fb->feedback.progress.setpoint.pose.position.y
    << " z: " << fb->feedback.progress.setpoint.pose.position.z
    << " att: x: " << fb->feedback.progress.setpoint.pose.orientation.x
    << " y: " << fb->feedback.progress.setpoint.pose.orientation.y
    << " z: " << fb->feedback.progress.setpoint.pose.orientation.z
    << " w: " << fb->feedback.progress.setpoint.pose.orientation.w;
}

void AckCallback(ff_msgs::AckStampedConstPtr const& ack) {
  ROS_INFO("\n Checking Ack\n");
  if (ack->completed_status.status == ff_msgs::AckCompletedStatus::NOT) {
    return;
  } else if (ack->completed_status.status == ff_msgs::AckCompletedStatus::OK) {
    std::cout << "\n" << ack->cmd_id << " command completed successfully!\n";
  } else if (ack->completed_status.status ==
                                        ff_msgs::AckCompletedStatus::CANCELED) {
    std::cout << "\n" << ack->cmd_id << " command was cancelled. This may have";
    std::cout << " been due to a fault in the system or if someone had issued ";
    std::cout << "another command. Aborting!\n";
    ros::shutdown();
    return;
  } else {
    // Command failed due to bad syntax or an actual failure
    std::cout << "\n" << ack->cmd_id << " command failed! " << ack->message;
    std::cout << "\n";
    ros::shutdown();
    return;
  }
}

class PubSub






int main(int argc, char **argv)
{
    ros::init(argc, argv, "move_node");
    ros::NodeHandle n;
    ros::Publisher cmd_pub = n.advertise<ff_msgs::CommandStamped>("command", 10, true);

    ros::Duration half_sec(0.5);


    // subscribe to stuff
    ros::Subscriber ack_sub, move_sub, marker_locs_sub;
    ack_sub = n.subscribe("mgt/ack", 10, &AckCallback);
    move_sub = n.subscribe("mob/motion/feedback", 10, &MoveFeedbackCallback);
    marker_locs_sub = n.subscribe("marker_locs", 10, &MarkerLocsCallback);

    ros::spinOnce();
    
    int count = 0;
    while (ack_sub.getNumPublishers() == 0) {
        ros::Duration(0.2).sleep();
        // Only wait 2 seconds
        if (count == 9) {
            std::cout << "No publisher for acks topics. This tool will not work ";
            std::cout << "without this.\n\n";
            return 1;
        }
        count++;
    }

    while (ros::ok()) {
        // message declarations
        // ff_msgs::CommandStamped stopcommand;
        ff_msgs::CommandStamped movecommand;


        // we want this to happen in the subscriber
        float pos_array [] = {11, -5, 4.7};
        pos_global.setPosition(pos_array);

        // get x, y, z from  marker_loc publisher:
        float vec_pos_x = pos_global.get_x();
        float vec_pos_y = pos_global.get_y();
        float vec_pos_z = pos_global.get_z();

        std::cout << "\nCommanded Position:\n";
        std::cout << "x: " << vec_pos_x << "\n";
        std::cout << "y: " << vec_pos_y << "\n";
        std::cout << "z: " << vec_pos_z << "\n";

        //update FLAGS_move) {
        // stopcommand.header.stamp = ros::Time::now();
        // stopcommand.subsys_name = "Astrobee";

        /// stop ///
        // stopcommand.cmd_name = ff_msgs::CommandConstants::CMD_NAME_STOP_ALL_MOTION;
        // stopcommand.cmd_id = ff_msgs::CommandConstants::CMD_NAME_STOP_ALL_MOTION;
        /////////////////////////////
        /// move ///
        movecommand.header.stamp = ros::Time::now();
        movecommand.subsys_name = "Astrobee";
        movecommand.cmd_name = ff_msgs::CommandConstants::CMD_NAME_SIMPLE_MOVE6DOF;
        movecommand.cmd_id = ff_msgs::CommandConstants::CMD_NAME_SIMPLE_MOVE6DOF;

        // move command has 4 args
        movecommand.args.resize(4);
        movecommand.args[0].data_type = ff_msgs::CommandArg::DATA_TYPE_STRING;

        // not sure what any of these are but they don't seem to change
        movecommand.args[0].b = false;
        movecommand.args[0].d = 0.0;
        movecommand.args[0].f = 0.0;
        movecommand.args[0].i = 0;
        movecommand.args[0].ll = 0;

        // sending coords in ISS_world frame, not relative
        movecommand.args[0].s = "world";
        // movecommand.args[0].s = "body";   // if doing relative work

        // set the new coords to move to
        movecommand.args[1].data_type = ff_msgs::CommandArg::DATA_TYPE_VEC3d;
        movecommand.args[1].vec3d[0] = vec_pos_x;
        movecommand.args[1].vec3d[1] = vec_pos_y;
        movecommand.args[1].vec3d[2] = vec_pos_z;

        // Set tolerance. Currently not used but needs to be in the command
        movecommand.args[2].data_type = ff_msgs::CommandArg::DATA_TYPE_VEC3d;
        movecommand.args[2].vec3d[0] = 0;
        movecommand.args[2].vec3d[1] = 0;
        movecommand.args[2].vec3d[2] = 0;

        // rotation stuff, might need to specify since we're in world (absolute) mode not relative.
        movecommand.args[3].data_type = ff_msgs::CommandArg::DATA_TYPE_MAT33f;
        movecommand.args[3].mat33f[0] = 0;
        movecommand.args[3].mat33f[1] = 0;
        movecommand.args[3].mat33f[2] = 0;
        movecommand.args[3].mat33f[3] = 1;

        //send the move command
        // move_pub.publish(stopcommand);
        cmd_pub.publish(movecommand);

        std::cout << "\nStarted " << movecommand.cmd_id << " command. We are HACKYING IT";
        half_sec.sleep();
    }

    return 0;
}