#include "ros/ros.h"
#include "laser_msgs/srv_laser.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
 
#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/ros/conversions.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/io.h>
#include <tinyxml.h>

#include <unistd.h> 

 using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "cliente");
  ros::NodeHandle nh;

	pcl::PointCloud<pcl::PointXYZ> cloud;
	pcl::PCDWriter writer;
	
  ros::ServiceClient client1 = nh.serviceClient<laser_msgs::srv_laser>("srv_laser");

  laser_msgs::srv_laser srv1;

  //Motor movement parameters
	int velocidadPosicion;
	int velocidadMedida;
	int posicionInicial;
	int posicionFinal;

	//Laser Parameters 
	int anguloMin;
	int anguloMax;

	nh.getParam("/dynamixel/speed/positioning",    velocidadPosicion);	
	nh.getParam("/dynamixel/speed/measuring",  velocidadMedida);	
	nh.getParam("/dynamixel/position/inicial", posicionInicial);	
	nh.getParam("/dynamixel/position/final",   posicionFinal);	
	nh.getParam("/hokuyo/angulo/minimo",  anguloMin);	
	nh.getParam("/hokuyo/angulo/maximo",  anguloMax);	

	srv1.request.positionSpeed=velocidadPosicion;
	srv1.request.measureSpeed=velocidadMedida;
	srv1.request.initialPosition=(float)posicionInicial;
	srv1.request.finalPosition=(float)posicionFinal;
	srv1.request.anguloMin=anguloMin;
	srv1.request.anguloMax=anguloMax;
	
	client1.call(srv1);
	
  sensor_msgs::PointCloud2 cloud2;
	cloud2=srv1.response.cloud;

	pcl::fromROSMsg(cloud2, cloud);
	writer.write("nube.pcd",cloud);

	int pid;
	pid=fork();

	if (pid==0) {
		execl("/usr/bin/pcd_viewer", "pcd_viewer", "nube.pcd", NULL);
	}
	return 0;
 }
