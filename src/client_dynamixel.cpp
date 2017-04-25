#include "ros/ros.h"
#include "laser3D/srv_dynamixel.h"
#include <cstdlib>
 using namespace std;



 int main(int argc, char **argv)
 {
   ros::init(argc, argv, "cliente_dynamixel");

   ros::NodeHandle n;
	ros::ServiceClient client_move = n.serviceClient<laser3D::srv_dynamixel>("srv_move");
   ros::ServiceClient client_position = n.serviceClient<laser3D::srv_dynamixel>("srv_position");
   ros::ServiceClient client_errors = n.serviceClient<laser3D::srv_dynamixel>("srv_errors");



   laser3D::srv_dynamixel move;
   laser3D::srv_dynamixel pos;
   laser3D::srv_dynamixel errors;

	while (ros::ok())
	{
		cout <<"Indicar posicion: ";
		cin >> move.request.position_i;
		cout << "\n";


		//Variables motor
		int positionSpeed;
		int measureSpeed;
		float initialPosition;
		float finalPosition;
		bool sentido_giro;
		
		ROS_INFO("MOVIENDO MOTOR");
		move.request.position_i=initialPosition;
		move.request.speed=positionSpeed;
		//Sentido de giro previsto
		client_position.call(pos);
		if(pos.response.position_o>move.request.position_i)		sentido_giro=0;
		else																		sentido_giro=1;
		//Mover
		client_move.call(move);

		do{
			client_position.call(pos);
			//ROS_INFO("Posicion: %f", pos.response.position_o);
		}
		while((sentido_giro==0) ? pos.response.position_o>initialPosition+1 : pos.response.position_o<initialPosition-1);



 }


