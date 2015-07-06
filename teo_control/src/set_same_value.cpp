/***********************************************************************
	______      _           _   _            _           _     
	| ___ \    | |         | | (_)          | |         | |    
	| |_/ /___ | |__   ___ | |_ _  ___ ___  | |     __ _| |__  
	|    // _ \| '_ \ / _ \| __| |/ __/ __| | |    / _` | '_ \ 
	| |\ \ (_) | |_) | (_) | |_| | (__\__ \ | |___| (_| | |_) |
	\_| \_\___/|_.__/ \___/ \__|_|\___|___/ \_____/\__,_|_.__/ 

*************************************************************************/
/*
Author: Domingo Esteban
Universidad Carlos III de Madrid
Leganes, Madrid, Spain, 2015
*/


// ROS header files
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/JointState.h"
#include "teo_msgs/SupportFoot.h"


#define TOTAL_TIME 2

using namespace std;


//void usage(void)
//{
//    ROS_INFO("Usage:\n");
//    ROS_INFO(" -f<name>\n");
//    ROS_INFO(" -d<name>\n");
//    exit (8);
//}


//MAIN FUNCTION
int main(int argc, char **argv)
{

//    Incluir tiempo de publicacion, frecuencia y/u otros
//    for (int i = 1; i < argc; i++) {
//        if (i + 1 != argc) // Check that we haven't finished parsing already
//            if (argv[i] == "-f") {
//                // We know the next argument *should* be the filename:
//                myFile = argv[i + 1];
//            } else if (argv[i] == "-p") {
//                myPath = argv[i + 1];
//            } else if (argv[i] == "-o") {
//                myOutPath = argv[i + 1];
//            } else {
//                std::cout << "Not enough or invalid arguments, please try again.\n";
//                Sleep(2000);
//                exit(0);
//        }
//        std::cout << argv[i] << " ";
//    }

  // Initialize Ros Node
  ros::init(argc, argv, "TEOSetPointController");
  ros::NodeHandle n;	

//  ros::Time::init();

  

  // PUBLISHERS

  ros::Publisher joint0_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_hip_yaw_position_controller/command", 1);
  ros::Publisher joint1_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_hip_roll_position_controller/command", 1);
  ros::Publisher joint2_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_hip_pitch_position_controller/command", 1);
  ros::Publisher joint3_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_knee_pitch_position_controller/command", 1);
  ros::Publisher joint4_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_ankle_pitch_position_controller/command", 1);
  ros::Publisher joint5_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_ankle_roll_position_controller/command", 1);
  ros::Publisher joint6_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_hip_yaw_position_controller/command", 1);
  ros::Publisher joint7_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_hip_roll_position_controller/command", 1);
  ros::Publisher joint8_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_hip_pitch_position_controller/command", 1);
  ros::Publisher joint9_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_knee_pitch_position_controller/command", 1);
  ros::Publisher joint10_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_ankle_pitch_position_controller/command", 1);
  ros::Publisher joint11_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_ankle_roll_position_controller/command", 1);
  ros::Publisher joint12_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/waist_yaw_position_controller/command", 1);
  ros::Publisher joint13_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/waist_pitch_position_controller/command", 1);
  ros::Publisher joint14_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_shoulder_pitch_position_controller/command", 1);
  ros::Publisher joint15_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_shoulder_roll_position_controller/command", 1);
  ros::Publisher joint16_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_shoulder_yaw_position_controller/command", 1);
  ros::Publisher joint17_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_elbow_pitch_position_controller/command", 1);
  ros::Publisher joint18_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_wrist_yaw_position_controller/command", 1);
  ros::Publisher joint19_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/r_wrist_pitch_position_controller/command", 1);
  ros::Publisher joint20_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_shoulder_pitch_position_controller/command", 1);
  ros::Publisher joint21_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_shoulder_roll_position_controller/command", 1);
  ros::Publisher joint22_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_shoulder_yaw_position_controller/command", 1);
  ros::Publisher joint23_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_elbow_pitch_position_controller/command", 1);
  ros::Publisher joint24_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_wrist_yaw_position_controller/command", 1);
  ros::Publisher joint25_command_to_ROS = n.advertise<std_msgs::Float64>("/teo/l_wrist_pitch_position_controller/command", 1);


  std_msgs::Float64 joint_command;
  joint_command.data = 0;


  ros::Time begin = ros::Time::now();
  ros::Duration duration;

  // ROS
  while(n.ok()) {
      for (int ii=0; ii<14; ii++) {
          joint0_command_to_ROS.publish(joint_command);

          joint1_command_to_ROS.publish(joint_command);

          joint2_command_to_ROS.publish(joint_command);

          joint3_command_to_ROS.publish(joint_command);

          joint4_command_to_ROS.publish(joint_command);

          joint5_command_to_ROS.publish(joint_command);

          joint6_command_to_ROS.publish(joint_command);

          joint7_command_to_ROS.publish(joint_command);

          joint8_command_to_ROS.publish(joint_command);

          joint9_command_to_ROS.publish(joint_command);

          joint10_command_to_ROS.publish(joint_command);

          joint11_command_to_ROS.publish(joint_command);

          joint12_command_to_ROS.publish(joint_command);
      }


      duration = ros::Time::now() - begin;
//      if (duration.toSec() >= TOTAL_TIME)
//        break;



    ros::spinOnce();
  }


  return 0;
}
