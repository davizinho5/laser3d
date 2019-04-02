#include <ros/ros.h>
#include <dynamixel_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/PointCloud.h>


using namespace std;
using namespace sensor_msgs;

double pi = 3.14159265359;
ros::Publisher publish_cloud_time;
double ang = .0396;
double total_number_clouds = 70.0;
int count_clouds = 0;
double yaw_inicial = 0.0;

double degree2rad(const double & d)
{
	return pi * d / 180;
} 

void stateCallback(const sensor_msgs::Imu & msg) {
	//Wait for PointCloud
	boost::shared_ptr<sensor_msgs::PointCloud const> sharedPtr;
  static tf::TransformBroadcaster br;
 
  float position = msg.orientation_covariance[0];
  
  sensor_msgs::PointCloud msg_cloud;
	ros::Time actual_time = ros::Time::now();
//	try {
		sharedPtr = ros::topic::waitForMessage<sensor_msgs::PointCloud>("velodyne_points_converted", ros::Duration(0.3));

		if(sharedPtr != NULL)
			msg_cloud = *sharedPtr;


//}
//  catch (std::exception e)
//  {
//        ROS_ERROR("No point clound messages received\n");

//  }

//	cout << "NUBE " << count_clouds << endl;count_clouds++;
	//Transformation tf

  double r=3.14159/2,p=0,y=3.14159/2;

//  Quaternion (const Vector3 &axis, const tfScalar &angle)
  
  tf::Vector3 vector(1.1347284e-04,0.0031,-0.0394702);
	double angulo = ((position*-ang)/180.0);

  cout << "ANGULO " << angulo << endl;

	tf::Quaternion quat_offset(vector,angulo);

  tf::Quaternion q_imu;
//  tf::Quaternion q_orig(0.0,0.707,0.0,0.707);
//  tf::Quaternion q_orig(0.707,0.0,0.707,0.0);
  quaternionMsgToTF(msg.orientation,q_imu);

	tf::Matrix3x3 m(q_imu);
	double roll, pitch, yaw;
	m.getRPY(roll, pitch, yaw);

//  if(count_clouds == 0) {
//   yaw_inicial = yaw;
//	 count_clouds++;
//  }

  tf::Quaternion q_orig,q_orig_2;//Quaternion (const tfScalar &yaw, const tfScalar &pitch, const tfScalar &roll) __attribute__((deprecated))
  q_orig.setRPY(roll,pitch,yaw);
  q_orig_2.setRPY(roll,pitch,yaw);

  tf::Quaternion q_rot = tf::createQuaternionFromRPY(0,y,2*y); //Rotating -90º


  msg_cloud.header.stamp = actual_time;
  publish_cloud_time.publish(msg_cloud);

	cout << roll << " " << pitch << " " << yaw << endl;

  tf::Quaternion q_new = q_orig*q_rot*quat_offset;//*q_rot;//*q_rot;//*quat_offset; //New orientation FALTA OFFSET
  tf::Quaternion q_new_2 = q_imu*q_rot;//*quat_offset;

//  cout << "ANGULO 1 " << q_new.getAngle()  << endl;
//  cout << "ANGULO 2 " << q_new_2.getAngle() << endl;

  tf::Transform transform2( q_new, tf::Vector3(0.0, 0.05, 0.175));
  tf::Transform transform3( q_new_2, tf::Vector3(0.3, 0.0, 0.42)); 
  
  br.sendTransform(tf::StampedTransform(transform2, ros::Time::now(), "base_link", "imu_link"));
  br.sendTransform(tf::StampedTransform(transform3, ros::Time::now(), "base_link", "imu_link_2"));

  tf::Transform transform;
  transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
  tf::Quaternion q;



	//ESTA ROTACION SE HACIA EN X -- AHORA PARA LAS PRUEBAS (Z)
	cout << "Publishing transformation tf..." << " " << position << endl;
//  tf::Quaternion q_motor;
	q.setRPY(degree2rad(-position), 0,0); //AQUI ESTABA PUESTO : -position (antes del 01-04-19)
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "imu_link", "velodyne"));




}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "motor_tf_pos");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/imu_laser_pub", 10, &stateCallback);
  publish_cloud_time = nh.advertise<sensor_msgs::PointCloud>("/cloud_timestamp", 10);


  ros::spin();
  return 0;
}
