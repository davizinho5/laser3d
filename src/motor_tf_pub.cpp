#include <ros/ros.h>
#include <dynamixel_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

void stateCallback(const dynamixel_msgs::JointStateConstPtr& msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
  tf::Quaternion q;
  q.setRPY(msg->current_pos, 0, 0);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "laser_link", "laser"));
}


int main(int argc, char **argv) {
  ros::init(argc, argv, "motor_tf_pos");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/laser_controller/state", 10, &stateCallback);

  ros::spin();
  return 0;
}
