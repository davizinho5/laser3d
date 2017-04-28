#include <ros/ros.h>
#include <laser_assembler/AssembleScans.h>
#include "std_msgs/Float64.h"

using namespace laser_assembler;

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_motor_controller");
  ros::NodeHandle n;

  ros::service::waitForService("assemble_scans");
  ros::ServiceClient client = n.serviceClient<AssembleScans>("assemble_scans");
  AssembleScans srv;  
  srv.request.begin = ros::Time(0,0);
  srv.request.end   = ros::Time::now();

  ros::Publisher pub = n.advertise<std_msgs::Float64>("/laser_controller/command", 10);


  if (client.call(srv))
    printf("Got cloud with %u points\n", srv.response.cloud.points.size());
  else
    printf("Service call failed\n");
  return 0;
}
