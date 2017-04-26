cmake_minimum_required(VERSION 2.8.3)
project(laser_msgs)

# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS   
 message_generation
 roscpp 
 rospy
 geometry_msgs 
 std_msgs 
 std_srvs 
 pcl_msgs
)

find_package( SDL REQUIRED )
find_package( PCL 1.7 REQUIRED )
find_package( Boost 1.40 COMPONENTS regex )
#find_package( libdxl REQUIRED )

#uncomment if you have defined messages
add_message_files(
 FILES
 srv_laser_scan.msg
)

add_service_files(
  FILES
  srv_dynamixel.srv
  srv_hokuyo.srv
  srv_laser.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(DEPENDENCIES geometry_msgs std_msgs pcl_msgs )

catkin_package(
    CATKIN_DEPENDS message_runtime geometry_msgs std_msgs
)

