#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Float32.h>
#include <laser3d/pololu_pos_timestamp.h>

using namespace sensor_msgs;
using namespace std;
using namespace message_filters;
using namespace std_msgs;

//void callback(const PointCloud2ConstPtr& image, const Float32ConstPtr& cam_info)
//{
//  // Solve all of perception here...
//}

ros::Publisher publish_pos_time;
ros::Publisher publish_cloud_time;


void callback2(const std_msgs::Float32 & pos)
{
	boost::shared_ptr<sensor_msgs::PointCloud2 const> sharedPtr;
	sharedPtr = ros::topic::waitForMessage<sensor_msgs::PointCloud2>("velodyne_points", ros::Duration(0.25));

	sensor_msgs::PointCloud2 msg_imu = *sharedPtr;
	ros::Time actual_time = ros::Time::now();

	laser3d::pololu_pos_timestamp pos_timestamp;
	pos_timestamp.header.stamp = actual_time;
	pos_timestamp.pos = pos.data;

	msg_imu.header.stamp = actual_time;

	cout << "Publishing pos with timestamp..." << endl;
	publish_pos_time.publish(pos_timestamp);
	
	cout << "Publishing cloud with timestamp..." << endl;
	publish_cloud_time.publish(msg_imu);

//	//YA TENGO TIMESTAMPS -- EN QUE TOPICS PUBLICO
//	cout << "POS " << pos << " WIDTH " << msg_imu.width  << endl;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "pololu_cloud");

  ros::NodeHandle nh;

//  message_filters::Subscriber<PointCloud2> image_sub(nh, "velodyne_points", 1);
//  message_filters::Subscriber<Float32> info_sub(nh, "pololu_pos", 1);
//  TimeSynchronizer<PointCloud2, Float32> sync(image_sub, info_sub, 10);
//  sync.registerCallback(boost::bind(&callback, _1, _2));

	ros::Subscriber sub = nh.subscribe("pololu_pos", 1000, callback2);

  publish_pos_time = nh.advertise<laser3d::pololu_pos_timestamp>("/pololu_pos_timestamp", 10);
	publish_cloud_time = nh.advertise<sensor_msgs::PointCloud2>("/cloud_timestamp", 10);


  ros::spin();

  return 0;
}
