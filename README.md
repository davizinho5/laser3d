ROS Indigo version tested under Ubuntu 14.04 LTS

This software assembles laser scans or point clouds from a laser that is turned by a dynamixel motor and merges them into a bigger point cloud. It has the following dependencies:
- [Point cloud library](http://pointclouds.org/)
- [Simple DirectMedia Layer](http://www.libsdl.org/)
- [BOOST](http://www.boost.org/)
- [URG](http://www.hokuyo-aut.jp/) 
- Several ROS packages, check package.xml.

## USAGE

Install the depedencies, copy this the software into your catkin workspace and compile it. 

Connect the motor and the laser to the computer. In the case of a Hokuyo laser, it usually opens in ttyACM0 port. The Dynamixel motor normally uses ttyUSB0. In both cases, R/W permissions are needed. Use:

$ sudo chmod a+rw /dev/ttyACM0  
$ sudo chmod a+rw /dev/ttyUSB0

If the port number changes in your computer, change them in these commands and in the configuration parameters provided inside "motorised_hokuyo" ros package. 

When using a Velodyne laser, configure the network as provided by the manufacter. 

Finally, you can launch the driver the motor and the laser drivers by (choose depending on your laser). Besides, rviz is started with a proper config file.

$ roslaunch laser3d controller\_manager\_velodyne.launch  
$ roslaunch laser3d controller\_manager\_hokuyo.launch

Then, start the dynamixel motor controller by:

$ roslaunch laser3d start\_motor\_controller.launch 

If everything goes fine, the motor controller should end showing messages similar to this:

[INFO] [WallTime: 1494489930.102583] Controller laser_controller successfully started.  
[laser_controller_spawner-1] process has finished cleanly  
all processes on machine have died, roslaunch will exit shutting down processing monitor...  
... shutting down processing monitor complete  
done

If velodyne laser is used, the sensor::msgs PintCloud2 message provided has to be converted to sensor::msgs PintCloud, in order to do this launch:

$ roslaunch laser3d pc_converter.launch

Then, the assembler node has to be launched. Depending on the type of information provided by your laser you have to launch laser\_aseembler (hokuyo) or point\_cloud\_aseembler (velodyne).
 
$ roslaunch laser3d laser\_aseembler.launch  
$ roslaunch laser3d point\_cloud\_aseembler.launch

Finally, there is an testing node which commands the motor to move, the assembler to merge the information and finally publishes it so that it can be seen in rviz. 





