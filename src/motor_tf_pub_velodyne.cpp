#include <ros/ros.h>
#include <dynamixel_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/PointCloud2.h>


using namespace std;
using namespace sensor_msgs;

double pi = 3.14159265359;
ros::Publisher publish_cloud_time;

double degree2rad(const double & d)
{
	return pi * d / 180;
} 

void stateCallback(const std_msgs::Float32 & msg) {
	//Wait for pointcloud
	boost::shared_ptr<sensor_msgs::PointCloud2 const> sharedPtr;
	sharedPtr = ros::topic::waitForMessage<sensor_msgs::PointCloud2>("velodyne_points", ros::Duration(0.45));
	sensor_msgs::PointCloud2 msg_cloud = *sharedPtr;
	//Transformation tf
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
  tf::Quaternion q;
	//Actual Ros time for : 1. Pointcloud 2. New tf
	ros::Time actual_time = ros::Time::now();

	msg_cloud.header.stamp = actual_time;
	//Republishing pointcloud in "/cloud_timestamp " with new timestamp
	cout << "Publishing cloud with " << msg_cloud.height*msg_cloud.width << " points with new timestamp..." << endl;
	publish_cloud_time.publish(msg_cloud);

	//ESTA ROTACION SE HACIA EN X -- AHORA PARA LAS PRUEBAS (Z)
	cout << "Publishing transformation tf..." << endl;
  q.setRPY(degree2rad(msg.data), 0,0 ); 
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, actual_time, "laser", "velodyne"));
}


int main(int argc, char **argv) {
  ros::init(argc, argv, "motor_tf_pos");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/pololu_pos", 10, &stateCallback);
	publish_cloud_time = nh.advertise<sensor_msgs::PointCloud2>("/cloud_timestamp", 10);


  ros::spin();
  return 0;
}
