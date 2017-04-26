#include <iostream>
#include <stdio.h>	
#include <cstdlib>
#include <cstdio>
#include <sstream>

#include "ros/ros.h"
#include <sensor_msgs/LaserScan.h>
#include "laser_msgs/srv_hokuyo.h"

#include <urg_node/URGConfig.h>
#include <urg/UrgCtrl.h>
#include <urg/RangeSensorParameter.h>

using namespace std;
using namespace qrk;

//const char device[];// = "/dev/ttyACM0";
UrgCtrl urg;
bool state=0;
RangeSensorParameter parameters;

bool serverHokuyoOnOff(laser_msgs::srv_hokuyo::Request &req, laser_msgs::srv_hokuyo::Response &res) {
	switch (req.option) {
	   case 1:
			state=1;
			//ROS_INFO("ENCENDIDO");
			//urg.setLaserOutput(true);		  	
			break;
		
	   case 0:
			state=0;
			//ROS_INFO("APAGADO");
			//urg.setLaserOutput(false);		  
			break;  
		
		default:
			break;
	}

	return true;
}

bool serverHokuyoParametros(laser_msgs::srv_hokuyo::Request &req, laser_msgs::srv_hokuyo::Response &res) {
  parameters = urg.parameter();
	parameters.area_max=req.anguloMax;
	parameters.area_min=req.anguloMin;
	urg.setParameter(parameters);
	urg.loadParameter();

	return true;
}

int main(int argc, char **argv) {
  //Init ROS
  ros::init(argc, argv, "nodo_hokuyo");
  ros::NodeHandle n;
	ros::NodeHandle nh("~");
  ros::ServiceServer ser_hokuyo = n.advertiseService("srv_hokuyo", serverHokuyoOnOff);
  ros::ServiceServer ser_parametros = n.advertiseService("srv_parameter", serverHokuyoParametros);	
  ros::Publisher pub_hokuyo = n.advertise<sensor_msgs::LaserScan>("topic_hokuyo", 1);
     
	int num;
  nh.getParam("/hokuyo/portACM/number", num); 
  char port[] = "/dev/ttyACM ";
  sprintf(&port[11], "%d", num);
  //port[11] = num;
  if (! urg.connect(port)) {    
    ROS_ERROR("UrgCtrl::connect: %s\n", urg.what());
    exit(1);
	}
  
  sensor_msgs::LaserScan scan;
  vector<long> data;
  long timestamp = 0;
	
  while (ros::ok()) {
    if (state==1) {
      int m = urg.capture(data, &timestamp);
			scan.ranges.resize((float)(m)); 
			
			//Se guarda en scan.ranges las capturas 
			for (int i = 0; i < m; ++i) {
				scan.ranges[i] = (float)data[i];
			}
 			pub_hokuyo.publish(scan);
    }
    ros::spinOnce();	  
  }
	ros::spin();
}

