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


using namespace std;


void usage(void)
{
    ROS_INFO("Usage:\n");
    ROS_INFO(" -f<name>\n");
    ROS_INFO(" -d<name>\n");
    exit (8);
}


//MAIN FUNCTION
int main(int argc, char **argv)
{

    for (int i = 1; i < argc; i++) {
        if (i + 1 != argc) // Check that we haven't finished parsing already
            if (argv[i] == "-f") {
                // We know the next argument *should* be the filename:
                myFile = argv[i + 1];
            } else if (argv[i] == "-p") {
                myPath = argv[i + 1];
            } else if (argv[i] == "-o") {
                myOutPath = argv[i + 1];
            } else {
                std::cout << "Not enough or invalid arguments, please try again.\n";
                Sleep(2000);
                exit(0);
        }
        std::cout << argv[i] << " ";
    }

  // Initialize Ros Node
  ros::init(argc, argv, "TEOSetPointController");
  ros::NodeHandle n;	
  ros::Time::init();

  

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


  // ROS
  while(n.ok()) {
    if (client_sockfd == -1) {
      //Ask O.S. for conections and accept one
      sin_size = sizeof(struct sockaddr_in);
      ROS_INFO("Waiting for TEOTraGen trajectories...");
      if ((client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_addr,
                          &sin_size))==-1) {
        ROS_ERROR("Error in accept()\n");
      	close(server_sockfd);
        exit(-1);
      }
      ROS_INFO("Receiving trajectories from TEOTraGen ...");
      usleep(2000); // Wait 2 ms
    }
    else {
      if ((numbytes_received = recv(client_sockfd, buffer_received, sizeof(struct Humanoid_joints_structure), 0)) == -1){  
        printf("Error in recv() \n");
	      close(server_sockfd); close(client_sockfd);
        client_sockfd=-1;
        exit(-1);
      }
      if (numbytes_received == 0) {
        ROS_WARN("Comunication with TEOTraGen lost... \n");
        close(client_sockfd);
        client_sockfd=-1;

        // Temporal

        usleep(2000000); // Wait 2 s
        joint_command.data = 0;
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


      }
      else {
        num_config_received += 1;
        TEO_joints = *(struct Humanoid_joints_structure *) &buffer_received;
        //printf("Recibido dato %lu con %d Bytes\n",num_config_received, numbytes_received);

        joints_to_send.header.stamp = ros::Time::now();
        //ROS_WARN("SF es %d TEO_joints.support_foot \n", TEO_joints.support_foot);

        joints_to_send.name.resize ( 0 );
        joints_to_send.position.resize ( 0 );
        joints_to_send.velocity.resize ( 0 );
        joints_to_send.effort.resize ( 0 );

        for (int ii=0; ii<26; ii++) {
          joint_command.data = TEO_joints.joints[ii];


          joint0_command_to_ROS.publish(joint_command);


          switch (ii) {
            case 0:
              joints_to_send.name.push_back("r_hip_yaw");
              joint0_command_to_ROS.publish(joint_command);
              break;
            case 1:
              joints_to_send.name.push_back("r_hip_roll");
              joint1_command_to_ROS.publish(joint_command);
              break;
            case 2:
              joints_to_send.name.push_back("r_hip_pitch");
              joint2_command_to_ROS.publish(joint_command);
              break;
            case 3:
              joints_to_send.name.push_back("r_knee_pitch");
              joint3_command_to_ROS.publish(joint_command);
              break;
            case 4:
              joints_to_send.name.push_back("r_ankle_pitch");
              joint4_command_to_ROS.publish(joint_command);
              break;
            case 5:
              joints_to_send.name.push_back("r_ankle_roll");
              joint5_command_to_ROS.publish(joint_command);
              break;
            case 6:
              joints_to_send.name.push_back("l_hip_yaw");
              joint6_command_to_ROS.publish(joint_command);
              break;
            case 7:
              joints_to_send.name.push_back("l_hip_roll");
              joint7_command_to_ROS.publish(joint_command);
              break;
            case 8:
              joints_to_send.name.push_back("l_hip_pitch");
              joint8_command_to_ROS.publish(joint_command);
              break;
            case 9:
              joints_to_send.name.push_back("l_knee_pitch");
              joint9_command_to_ROS.publish(joint_command);
              break;
            case 10:
              joints_to_send.name.push_back("l_ankle_pitch");
              joint10_command_to_ROS.publish(joint_command);
              break;
            case 11:
              joints_to_send.name.push_back("l_ankle_roll");
              joint11_command_to_ROS.publish(joint_command);
              break;
            case 12:
              joints_to_send.name.push_back("waist_yaw");
              joint12_command_to_ROS.publish(joint_command);
              break;
            case 13:
              joints_to_send.name.push_back("waist_pitch");
              break;
            case 14:
              joints_to_send.name.push_back("r_shoulder_pitch");
              break;
            case 15:
              joints_to_send.name.push_back("r_shoulder_roll");
              break;
            case 16:
              joints_to_send.name.push_back("r_shoulder_yaw");
              break;
            case 17:
              joints_to_send.name.push_back("r_elbow_pitch");
              break;
            case 18:
              joints_to_send.name.push_back("r_wrist_yaw");
              break;
            case 19:
              joints_to_send.name.push_back("r_wrist_pitch");
              break;
            case 20:
              joints_to_send.name.push_back("l_shoulder_pitch");
              break;
            case 21:
              joints_to_send.name.push_back("l_shoulder_roll");
              break;
            case 22:
              joints_to_send.name.push_back("l_shoulder_yaw");
              break;
            case 23:
              joints_to_send.name.push_back("l_elbow_pitch");
              break;
            case 24:
              joints_to_send.name.push_back("l_wrist_yaw");
              break;
            case 25:
              joints_to_send.name.push_back("l_wrist_pitch");
              break;
          }


        }


      }
    }

    ros::spinOnce();

  }


  return 0;
}


