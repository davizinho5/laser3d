#include "ros/ros.h"
#include "laser_msgs/srv_laser.h"
#include "laser_msgs/srv_hokuyo.h"
#include <cstdlib>
#include <sensor_msgs/LaserScan.h>

#include <sstream>
#include <stdio.h>
#include <cmath>
#include <vector>

// THIS IS ONLY A TESTING NODE

using namespace std;

void topicHokuyo(const sensor_msgs::LaserScan::ConstPtr& scan_hokuyo) {
	ROS_INFO("TOPIC NUEVO: %d", (int)scan_hokuyo->ranges.size());  
}

int main(int argc, char **argv) {
   ros::init(argc, argv, "sus_hokuyo");
	 ros::NodeHandle nh;
	 ros::Subscriber sus_hokuyo = nh.subscribe("topic_hokuyo", 1, topicHokuyo); 

	 ros::spin();
}
