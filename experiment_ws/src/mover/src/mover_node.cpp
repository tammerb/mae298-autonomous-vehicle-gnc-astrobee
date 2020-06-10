#include <ros/ros.h>
#include <PublisherSubscriber.h>
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
#include <std_msgs/Float32MultiArray.h>

// file operations
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

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
    ROS_INFO("\n Ack pinging completed status, not completed.\n");
    return;
  } else if (ack->completed_status.status == ff_msgs::AckCompletedStatus::OK) {
    std::cout << "\n" << ack->cmd_id << " command completed successfully!\n";
  } else if (ack->completed_status.status ==
                                        ff_msgs::AckCompletedStatus::CANCELED) {
    std::cout << "\n" << ack->cmd_id << " command was cancelled. This may have";
    std::cout << " been due to a fault in the system or if someone had issued ";
    std::cout << "another command. Aborting!\n";
    // ros::shutdown();
    return;
  } else {
    // Command failed due to bad syntax or an actual failure
    std::cout << "\n" << ack->cmd_id << " command failed! " << ack->message;
    std::cout << "\n";
    // ros::shutdown();
    return;
  }
}

// specialize the subscriber callback
template<>
void PublisherSubscriber<ff_msgs::CommandStamped, std_msgs::Float32MultiArray>::subscriberCallback(const std_msgs::Float32MultiArray::ConstPtr& recievedMsg) {
  
  std::cout << "I've recieved the HACKEY-AF message:" << recievedMsg->data[0] << "\n";

  // construct a move command message
  ff_msgs::CommandStamped movecommand;

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
  //movecommand.args[1].vec3d[0] = vec_pos_x;
  //movecommand.args[1].vec3d[1] = recievedMsg->data[2];
  //movecommand.args[1].vec3d[2] = recievedMsg->data[3];
  movecommand.args[1].vec3d[0] = recievedMsg->data[0];
  movecommand.args[1].vec3d[1] = -5.555;
  movecommand.args[1].vec3d[2] = 4.444;

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

  publisherObject.publish(movecommand);
  // std::cout << "Sleeping for 10s.\n\n";
  // ros::Duration(10).sleep();
  // std::cout << "Doen sleeping.\n\n";
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mover_node");
    
    ros::NodeHandle n;
    ros::Subscriber ack_sub, move_sub;
    std::cout << "Constructing the ack_sub\n";
    ack_sub = n.subscribe("mgt/ack", 10, &AckCallback);
    move_sub = n.subscribe("mob/motion/feedback", 10, &MoveFeedbackCallback);
    

    int count = 0;
    while (ack_sub.getNumPublishers() == 0) {
      std::cout << "In the ack_sub getnum while loop.\n\n";
       ros::Duration(0.2).sleep();
        // Only wait 2 seconds
        if (count == 9) {
          std::cout << "No publisher for acks topics. This tool will not work ";
          std::cout << "without this.\n\n";
          return 1;
       }
       count++;
    }


    PublisherSubscriber<ff_msgs::CommandStamped, std_msgs::Float32MultiArray> seeAndMove("command", "marker_locs", 1);
    

    
    ros::spin();
}