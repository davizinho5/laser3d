#include "ros/ros.h"
#include "laser_msgs/srv_laser.h"
#include "laser_msgs/srv_hokuyo.h"
#include <cstdlib>
#include <sensor_msgs/LaserScan.h>

#include <sstream>
#include <stdio.h>
#include <cmath>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
  ros::init(argc, argv, "cliente_hokuyo");
	ros::NodeHandle m;
	//ros::Subscriber sus_hokuyo = m.subscribe("topic_hokuyo", 1, topicHokuyo); 
   ros::ServiceClient client_estado = m.serviceClient<laser_msgs::srv_hokuyo>("srv_hokuyo");
   ros::ServiceClient client_parameter = m.serviceClient<laser_msgs::srv_hokuyo>("srv_parameter");

	laser_msgs::srv_hokuyo estado;
	laser_msgs::srv_hokuyo parameter;

	int res;
	int anguloMax, anguloMin;
	while (ros::ok())	{
		cout<<"ELEGIR OPCION"<<endl;
		cout<<endl;
		cout<<"0 - Apagar"<<endl;
		cout<<"1 - Encender"<<endl;
		cout<<"2 - Cambiar parametros"<<endl;
		cout<<endl;
		cout<<endl;
		cin>>res;
		
		if(res==1) {
			estado.request.option=1;
			client_estado.call(estado);   
		}
		else if(res==2)	{
			cout<<"Angulo Max: ";
			cin>>anguloMax;
			cout<<endl;
			cout<<"Angulo Min: ";
			cin>>anguloMin;
			cout<<endl;
			parameter.request.anguloMin=(anguloMin+45)/0.25;
			parameter.request.anguloMax=(anguloMax+45)/0.25;
			client_parameter.call(parameter); 
		}
		else {
			estado.request.option=0;
			client_estado.call(estado);
		}		
	}
}
