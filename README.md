ROS Indigo version tested under Ubuntu 14.04 LTS

This software is compound by two ros packages
- laser_msgs: contains the necessaty messages to communicate the information. 
- motorised_hokuyo: contains ros nodes to use the system.

The software has the following dependencies:
- [Point cloud library](http://pointclouds.org/)
- [Simple DirectMedia Layer](http://www.libsdl.org/)
- [BOOST](http://www.boost.org/)
- [URG](http://www.hokuyo-aut.jp/) 
- urg-c ROS node: 
- DXL library, included in the 3rd party sofware folder in case you need to recompile it. Just use the Makefile provided anc copy the headers and library files into dynamixel folder.

## USAGE

Install the depedencies and copy this the software into your catkin workspace and compile it. 

Connect the motor and the laser to the computer. In the case of a Hokuyo laser, it usually opens in ttyACM0 port. The Dynamixel motor normally uses ttyUSB0. In both cases, R/W permissions are needed. Use:

$ sudo chmod a+rw /dev/ttyACM0  
$ sudo chmod a+rw /dev/ttyUSB0

If the port number changes in your computer, change them in these commands and in the configuration parameters provided inside "motorised_hokuyo" ros package. 

If the port number doesn't change among differente usages, ports can be enabled by writing a 'rules' files in /etc/udev/rules.d. Name the file:
laser.rules 

and include the following information:

KERNEL==“ttyACM*”,   
SUBSYSTEMS==“usb”,  
ATTRS{manufacturer}==“Hokuyo Data Flex for USB”,  
SYMLINK+=“laser”  
KERNEL==“ttyUSB*”,  
SUBSYSTEMS==“usb”,  
ATTRS{manufacturer}==“Linux 3.2.0-44-generic-pae uhci hcd”,
SYMLINK+=“motor”  

Then, type in the console:  
$ sudo service udev reload

Finally, you can launch the nodes which control the motor and the laser by:  
$ roslaunch motorised_hokuyo laser3D.launch

And ask for a new 3D scan by calling:
$ rosrun motorised_hokuyo bin\_cliente

This will create a PCL point cloud *.pcd which contains the laser scans.


