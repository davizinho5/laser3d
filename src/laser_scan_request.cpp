#include <ros/ros.h>
#include <laser_assembler/AssembleScans.h>
#include <dynamixel_msgs/JointState.h>
#include <dynamixel_controllers/SetSpeed.h>
#include <sensor_msgs/PointCloud.h>
#include <std_msgs/Float64.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_motor_controller");
  ros::NodeHandle nh;

// The laser_scan_assembler accumulates laser scans by listening to the appropriate topic 
//   and accumulating messages in a ring buffer of a specific size. 
// When the assemble_scans service is called,
//   the contents of the current buffer that fall between two times are converted into a single cloud and returned.

  ros::service::waitForService("assemble_scans");
  ros::ServiceClient client = nh.serviceClient<laser_assembler::AssembleScans>("assemble_scans");

  ros::Publisher pub_command = nh.advertise<std_msgs::Float64>("/laser_controller/command", 10);
  ros::Publisher pub_cloud = nh.advertise<sensor_msgs::PointCloud>("/assembled_cloud", 10);
 
  std_msgs::Float64 motor_pos; 
  dynamixel_msgs::JointStateConstPtr sharedPtr;
  
  // Check actual position and set next position command
  do{
    sharedPtr = ros::topic::waitForMessage<dynamixel_msgs::JointState>("/laser_controller/state", ros::Duration(0.2));	
  }while(sharedPtr == NULL);

  if(sharedPtr->current_pos > 3.0) 
    motor_pos.data = 0.0;
  else
    motor_pos.data = 3.1516; // PI + allowed error

  // Set the speed of the motor
  ros::service::waitForService("/laser_controller/set_speed");
  ros::ServiceClient speed_client = nh.serviceClient<dynamixel_controllers::SetSpeed>("/laser_controller/set_speed");
  dynamixel_controllers::SetSpeed srv_req;
  nh.getParam("/laser_controller/joint_speed", srv_req.request.speed);
 
  if (speed_client.call(srv_req)) {
    std::cout << "Speed set in the motor: " << srv_req.request.speed << std::endl;
  } else {
    ROS_ERROR("Failed to call service add_two_ints");
    return 1;
  }
  std::cout << srv_req.request.speed << std::endl;

  laser_assembler::AssembleScans srv;  
  // assemble from "NOW"
  srv.request.begin = ros::Time::now();
  // command the motor to move  
  pub_command.publish(motor_pos);

  // check the motor has finished
  do{
    sharedPtr = ros::topic::waitForMessage<dynamixel_msgs::JointState>("/laser_controller/state", ros::Duration(0.3));	
    //std::cout << "Posicion actual " << sharedPtr->current_pos - motor_pos.data << std::endl;
  }while( fabs(sharedPtr->current_pos - motor_pos.data) > 0.01); // 0.5 degrees error accepted

  // assemble untill "NOW"
  srv.request.end = ros::Time::now();
  if (client.call(srv)) {
    std::cout << "Got cloud with " <<  srv.response.cloud.points.size() << " points\n";
    pub_cloud.publish(srv.response.cloud);
  }
  else {
    printf("Service call failed\n");
  }
  return 0;

}

