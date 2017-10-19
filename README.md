ROS Kinetic version tested under Ubuntu 16.04 LTS.

In ROS Kinetic, Velodyne drivers are not included in the official release, I got them from this repository: 

https://github.com/ros-drivers/velodyne

This software assembles laser scans or point clouds from a laser that is turned by a dynamixel motor and merges them into a bigger point cloud. It has the following dependencies:
- [Point cloud library](http://pointclouds.org/)
- [BOOST](http://www.boost.org/)
- [URG](http://www.hokuyo-aut.jp/) 
- [SDL library] 
- Several ROS packages, check package.xml.

Install the depedencies, copy this the software into your catkin workspace and compile it.  

## USAGE

Connect the motor and the laser to the computer. In the case of a Hokuyo laser, it usually opens in ttyACM0 port. The Dynamixel motor normally uses ttyUSB0. In both cases, R/W permissions are needed. Use:

$ sudo chmod a+rw /dev/ttyUSB0  
$ sudo chmod a+rw /dev/ttyACM0      (only with hokuyo)

If the port number changes in your computer, change them in these commands and in the configuration parameters provided inside "motorised_hokuyo" ros package. 

When using a Velodyne laser, configure the network as provided by the manufacturer. 

Then, you can launch the driver of the motor and the laser drivers by (choose depending on your laser). Besides, rviz is started with a proper config file.

(velodyne) $ roslaunch laser3d controller\_manager\_velodyne.launch  
(hokuyo)   $ roslaunch laser3d controller\_manager\_hokuyo.launch  

Then, start the dynamixel motor controller by:

$ roslaunch laser3d start\_motor\_controller.launch 

If everything goes fine, the motor controller should end showing messages similar to this:

[INFO] [WallTime: 1494489930.102583] Controller laser_controller successfully started.  
[laser_controller_spawner-1] process has finished cleanly  
all processes on machine have died, roslaunch will exit shutting down processing monitor...  
... shutting down processing monitor complete  
done

If velodyne laser is used, the sensor::msgs PointCloud2 message provided has to be converted to sensor::msgs PointCloud, in order to do this launch:

$ roslaunch laser3d pc_converter.launch

Then, the assembler node has to be launched. Depending on the type of information provided by your laser you have to launch laser\_aseembler (hokuyo) or point\_cloud\_aseembler (velodyne).
 
(velodyne) $ roslaunch laser3d point\_cloud\_assembler.launch  
(hokuyo)   $ roslaunch laser3d laser\_assembler.launch  

Finally, there is an testing node which commands the motor to move, asks the assembler to merge the information and finally publishes it so that it can be seen in rviz. for calling this node:

(velodyne) $ rosrun laser3d bin\_laser\_pc\_assemble\_req  
(hokuyo)   $ rosrun laser3d bin\_laser\_scan\_assemble\_req  

Extra! There is and example launch file to call a pointcloud filter nodelet. In this case, the parameters are set to filter data on the z-axis between -1.0 and 10.5 meters, and downsample the data with a leaf size of 0.01 meters. Call:

$ roslaunch laser3d voxelgrid.launch

Extra 2! If you want to save the data into a point cloud, just call the next node:  

rosrun pcl\_ros pointcloud\_to\_pcd input:=/assembled\_cloud2

