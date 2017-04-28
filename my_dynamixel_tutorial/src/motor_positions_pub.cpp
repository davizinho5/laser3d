#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

void poseCallback(const turtlesim::PoseConstPtr& msg) {
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(0, 0, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, msg->position);
  transform.setRotation(q);
  
  br.sendTransform(tf::StampedTransform(transform, msg->timestamp, "base_link", "laser_link"));
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_tf_broadcaster");
  ros::NodeHandle n;

  ros::Subscriber sub = node.subscribe("/motor_states/laser_port", 10, &poseCallback);
  
  ros::spin();
  return 0;
}
