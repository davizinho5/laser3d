#include <ros/ros.h>

#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud_conversion.h>
#include <laser_assembler/AssembleScans.h>

#include <pcl/io/pcd_io.h>
#include <pcl_ros/transforms.h>
#include <pcl/point_types.h>
#include <pcl/conversions.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>

#include <std_msgs/Float64.h>

ros::Publisher pub;

using namespace std;


void printTf(tf::Transform tf) {
    tf::Vector3 tfVec;
    tf::Matrix3x3 tfR;
    tf::Quaternion quat;
    tfVec = tf.getOrigin();
    cout<<"vector from reference frame to to child frame: "<<tfVec.getX()<<","<<tfVec.getY()<<","<<tfVec.getZ()<<endl;
    tfR = tf.getBasis();
    cout<<"orientation of child frame w/rt reference frame: "<<endl;
    tfVec = tfR.getRow(0);
    cout<<tfVec.getX()<<","<<tfVec.getY()<<","<<tfVec.getZ()<<endl;
    tfVec = tfR.getRow(1);
    cout<<tfVec.getX()<<","<<tfVec.getY()<<","<<tfVec.getZ()<<endl;    
    tfVec = tfR.getRow(2);
    cout<<tfVec.getX()<<","<<tfVec.getY()<<","<<tfVec.getZ()<<endl; 
    quat = tf.getRotation();
    cout<<"quaternion: " <<quat.x()<<", "<<quat.y()<<", "
            <<quat.z()<<", "<<quat.w()<<endl;   
}


void cloudCallback(const sensor_msgs::PointCloud2& msg) {
  tf::TransformListener tf_listener;
  tf::StampedTransform transform;
  sensor_msgs::PointCloud2 out;

  //tf_listener.lookupTransform (std::string("base_link"), msg.header.frame_id, ros::Time(0), transform);
  tf_listener.waitForTransform(msg.header.frame_id, "base_link",  ros::Time(0), ros::Duration(2));
  tf_listener.lookupTransform (msg.header.frame_id, "base_link",  ros::Time(0), transform);


  cout << transform.frame_id_ << " HIJO: " << transform.child_frame_id_ << endl;
  tf::Transform tf(transform.getBasis(),transform.getOrigin());
  printTf(tf);
//transformPointCloud(const char [11], const PointCloud2&, sensor_msgs::PointCloud2*, tf::TransformListener*)’
//transformPointCloud (const std::string &target_frame, const sensor_msgs::PointCloud2 &in, sensor_msgs::PointCloud2 &out, const tf::TransformListener &tf_listener)

//pcl_ros::transformPointCloud(const string&, const PointCloud2&, sensor_msgs::PointCloud2&, const tf::TransformListener&)
  
//  msg.header.stamp = ros::Time(0);
  //pcl_ros::transformPointCloud(std::string("base_link"), msg, out, tf_listener);
  //pcl_ros::transformPointCloud(std::string("base_link"), transform, msg, out);
  cout << "MSG ID: " << msg.header.frame_id << endl;
  pcl_ros::transformPointCloud("base_link", transform, msg, out);
//  out.header.frame_id = "base_link";
  pub.publish(out);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_pc_transformer");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/velodyne_points", 10, &cloudCallback);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("/TransformedCloudToBase", 1);

  // Spin
  ros::spin ();

  return 0;
}



