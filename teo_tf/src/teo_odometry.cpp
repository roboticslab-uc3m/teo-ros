

#include "ros/ros.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>
#include <tf/message_filter.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Transform.h>
#include <sensor_msgs/JointState.h>
#include "teo_msgs/SupportFoot.h"
#include "tf2_msgs/TFMessage.h"


class HumanoidOdometry
{
  public:
    HumanoidOdometry();
//    ~HumanoidOdometry();

	private:
	  ros::NodeHandle nh; 
		ros::Publisher odom_pub;
		ros::Subscriber sf_sub;
		int8_t support_foot;
	  nav_msgs::Odometry odometry;
    bool firstdata, receiving_tf;
		
		tf::TransformBroadcaster m_broadcaster;
		tf::TransformListener m_listener;
    tf::StampedTransform averiguar;
    tf::StampedTransform tf_rf_to_base, tf_lf_to_base;
		tf::StampedTransform tf_base_to_rf, tf_base_to_lf;
		tf::Transform map_to_odom;
		tf::Transform odom_to_base, odom_to_rf, odom_to_lf;
    void UpdateSupportFoot (const teo_msgs::SupportFoot::ConstPtr& msg);


		void SetInitialPos(tf::StampedTransform tf_base_to_rf, tf::StampedTransform tf_base_to_lf);
};



HumanoidOdometry::HumanoidOdometry() : firstdata(1), receiving_tf(0) {

  odom_pub = nh.advertise<nav_msgs::Odometry>("odom",1000);

  sf_sub = nh.subscribe<teo_msgs::SupportFoot> ("support_foot", 10, &HumanoidOdometry::UpdateSupportFoot, this);

  ROS_INFO ("\n Waiting for JointStates and Support Foot Data...");
}

void HumanoidOdometry::UpdateSupportFoot (const teo_msgs::SupportFoot::ConstPtr& msg) {
  support_foot = msg->support_foot;

//	ROS_WARN("SF:%d", support_foot);


	try {
		m_listener.lookupTransform("base_link", "r_sole", ros::Time(0), tf_base_to_rf);
		m_listener.lookupTransform("base_link", "l_sole", ros::Time(0), tf_base_to_lf);
		m_listener.lookupTransform("r_sole", "base_link", ros::Time(0), tf_rf_to_base);
		m_listener.lookupTransform("l_sole", "base_link", ros::Time(0), tf_lf_to_base);
		receiving_tf = 1;
	}
	catch (tf::TransformException ex) {
		ROS_ERROR("%s",ex.what());
		receiving_tf = 0;
	}

	if (receiving_tf == 1) {
		if	(firstdata) {
      odom_to_rf = tf_base_to_rf;
      odom_to_lf = tf_base_to_lf;
		  HumanoidOdometry::SetInitialPos(tf_base_to_rf, tf_base_to_lf);
      firstdata = 0;
		}
		else {

			if (support_foot == -1) {
				odom_to_base = odom_to_rf * tf_rf_to_base;
				odom_to_lf = odom_to_base * tf_base_to_lf;
			}
			else if (support_foot == 1) {
				odom_to_base = odom_to_lf * tf_lf_to_base;
				odom_to_rf = odom_to_base * tf_base_to_rf;
			}
			else 
			ROS_ERROR("Wrong support_foot");

/*
			ROS_INFO("Odom_to_Base --> X:%.4f Y:%.4f Z:%.4f. Orient: X:%.4f", odom_to_base.getOrigin().getX(), 
			odom_to_base.getOrigin().getY(), odom_to_base.getOrigin().getZ(), odom_to_base.getRotation().getAxis().getX());
*/
       
			m_broadcaster.sendTransform(tf::StampedTransform(map_to_odom, ros::Time::now(), "map", "odom"));
			m_broadcaster.sendTransform(tf::StampedTransform(odom_to_base, ros::Time::now(), "odom", "base_link"));

      // **** PUBLISH ODOMETRY TOPIC **** 

      // Odometry
      odometry.pose.pose.position.x = odom_to_base.getOrigin().getX();
      odometry.pose.pose.position.y = odom_to_base.getOrigin().getY();
      odometry.pose.pose.position.z = odom_to_base.getOrigin().getZ();

      odometry.pose.pose.orientation.x = 0.0;
      odometry.pose.pose.orientation.y = 0.0;
      odometry.pose.pose.orientation.z = 0.0;
      odometry.pose.pose.orientation.w = 1.0;
      
	    // Pose covariance
    	for(int i = 0; i < 6; i++)
      	odometry.pose.covariance[i*6+i] = 0.1;

    	//set the velocity
    	odometry.child_frame_id = "base_link";
	    // Linear velocities
    	odometry.twist.twist.linear.x = 0.0;
    	odometry.twist.twist.linear.y = 0.0;
    	odometry.twist.twist.linear.z = 0.0;
    	// Angular velocities
    	odometry.twist.twist.angular.x = 0.0;
    	odometry.twist.twist.angular.y = 0.0;
    	odometry.twist.twist.angular.z = 0.0;
    	// Twist covariance
	    for(int i = 0; i < 6; i++)
      	odometry.twist.covariance[6*i+i] = 0.1;

    	//publish the message
    	odom_pub.publish(odometry);
		}
	}
	else
	  ROS_ERROR("No TF data");

}


void HumanoidOdometry::SetInitialPos(tf::StampedTransform tf_base_to_rf, tf::StampedTransform tf_base_to_lf) {
  
   tf::Vector3 new_origin = (tf_base_to_rf.getOrigin() + tf_base_to_lf.getOrigin())/2.0; // middle of both feet
   double height = std::min(tf_base_to_rf.getOrigin().getZ(), tf_base_to_lf.getOrigin().getZ()); // fix to lowest foot
   new_origin.setZ(-height);

   map_to_odom.setOrigin(new_origin);
   map_to_odom.setRotation(tf::Quaternion(0.0, 0.0, 0.0));
 
}

int main(int argc, char **argv)
{
	//Initiate a ROS node
  ros::init(argc, argv,"teo_pub_tf");

	//Create an object of class SubscribeAndPublish that will take care of everything
	HumanoidOdometry TeoOdometry;

	ros::spin();

	return 0;
}
