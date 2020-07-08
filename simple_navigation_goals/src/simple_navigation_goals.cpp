#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <sensor_msgs/BatteryState.h>
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include <boost/container/string.hpp>

// Ligne qui va servir à communiquer avec /move_base
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// Définition d'une classe qui va permettre de récupérer la tension de la batterie :
class Battery_Listener
{
  	public:
    	double tension;

    	void callback1(const sensor_msgs::BatteryStateConstPtr& msg);
};

void Battery_Listener::callback1(const sensor_msgs::BatteryStateConstPtr& msg) 
{
	tension = msg->voltage;
}

// Définition de la classe Choix_Listener qui va servir à récupérer les informations envoyée depuis l'appli Web
class Choice_Listener
{
  	public:
    	int choix;

    	void callback2(const std_msgs::Int32ConstPtr& choice);
};

void Choice_Listener::callback2(const std_msgs::Int32ConstPtr& choice) 
{
    	choix = choice->data;
}

int main(int argc, char **argv) 
{

	// Initialisation du noeud
	ros::init(argc, argv, "simple_navigation_goals");
	ros::NodeHandle nh;
	ros::Rate loop_rate(10);
	
	Battery_Listener battery_listener;
	Choice_Listener choice_listener;


	// On indique que l'on va lire les infos du topic /battery_state
   	ros::Subscriber sub1 = nh.subscribe<sensor_msgs::BatteryState>("battery_state", 1, &Battery_Listener::callback1, &battery_listener);
	// On indique que l'on va lire les infos du topic /choix
	ros::Subscriber sub2 = nh.subscribe<std_msgs::Int32>("choix", 1, &Choice_Listener::callback2, &choice_listener);
	// On indique que l'on va publier des messages sur le topic /messages
	ros::Publisher client_pub = nh.advertise<std_msgs::String>("messages",1);

        // démarage des programmes et du serveur liés à la navigation du robot
	MoveBaseClient ac("move_base", true);
	move_base_msgs::MoveBaseGoal goal;
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();
	ros::spinOnce();
		

	std_msgs::String message;
	int id;
	choice_listener.choix = -1;
		

	//On attend que le serveur action soit lancé
	while(!ac.waitForServer(ros::Duration(5.0))){
		ROS_INFO("\n Attente de l'initialisation du systeme\n");
		// On envoie le message pour l'interface web :
		message.data = "Attente de l'initialisation du systeme";
		client_pub.publish(message);
	}
	ROS_INFO("\n Initialisation du system reussie\n");
	message.data = "Initialisation du system reussie";
	client_pub.publish(message);


	while(true){

		// Création d'un vecteur qui va prendre la liste des noeuds en cours d'utilisation
		std::vector<std::string> V ;
		std::vector<std::string>::iterator it;

		ros::master::getNodes(V);
		bool alive = false;
		it = V.begin();
		// On va regarder si le noeud /simple_navigation_goals est encore vivant, si ce n'est pas le cas alors le noeud battery_monitoring l'a tué afin de forcer le robot à retourner au dock
		for (it=V.begin(); it<V.end(); it++){
			if (*it == "/simple_navigation_goals")
				alive = true; 
		}
		// Fonctionnement provisoire car il faut forcément que le turtle soit éteint pour le recharger donc à changer pour le meuble
		if (alive==false){
			std::cout << "\n Le robot va retourner à sa station de rechargement car il manque de batterie, veuillez l'éteindre et le mettre en charge \n";
			message.data = "Le robot va retourner à sa station de rechargement car il manque de batterie, veuillez l'éteindre et le mettre en charge";
			client_pub.publish(message);}
		//	ros::Duration(5).sleep();

		
		ros::spinOnce();    

		id = choice_listener.choix;
		switch (id){

			// Si on veut quitter :
			case 0: 
				return 0;
			// Position 1 :
			case 1:
				// Coordonnées du salon :
				goal.target_pose.pose.position.x = 3.00242334343;
			  	goal.target_pose.pose.position.y = -2.80841771349;
			  	goal.target_pose.pose.position.z = 0;
			  	goal.target_pose.pose.orientation.x = 0;
			  	goal.target_pose.pose.orientation.y = 0;
			  	goal.target_pose.pose.orientation.z = 0.702850648081;
			  	goal.target_pose.pose.orientation.w = 0.7113374491;

			 	std::cout << "\n Objectif envoye\n";
				message.data = "Objectif envoyé, le robot va se deplacer jusqu'au salon";
				client_pub.publish(message);

			  	// On envoie l'objectif au processus de navigation et on attend le résultat de l'action
			  	ac.sendGoal(goal);
			  	ac.waitForResult();
			  	// On vérifie s'il a bien attend l'objectif voulu ou non :
			  	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
			    		std::cout << "\n Le robot a bien atteint son objectif\n";
					message.data = "Le robot a bien atteint son objectif";
					client_pub.publish(message);}
			  	else {
			   		std::cout << "\n Le robot n'a pas atteint son objectif\n";
					message.data = "Le robot n'a pas atteint son objectif";
					client_pub.publish(message);}
			  //	ros::Duration(5).sleep();
				choice_listener.choix = -1;
			  	break;


			case 2:	  
				// Coordonnées de la cuisine :
				goal.target_pose.pose.position.x = -0.57942565439;
			  	goal.target_pose.pose.position.y = 1.64915032047;
			  	goal.target_pose.pose.position.z = 0;
			  	goal.target_pose.pose.orientation.x = 0;
			  	goal.target_pose.pose.orientation.y = 0;
			  	goal.target_pose.pose.orientation.z = 0.0618355841551;
			  	goal.target_pose.pose.orientation.w = 0.998086349236;
			  	
			  	std::cout << " \n Objectif envoye\n";
				message.data = "Objectif envoye, le robot va se deplacer jusqu'à la cuisine";
				client_pub.publish(message);
			  	ac.sendGoal(goal);
			  	ac.waitForResult();
			  	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
			    		std::cout << "\n Le robot a bien atteint son objectif\n";
					message.data = "Le robot a bien atteint son objectif";
					client_pub.publish(message);}
			  	else {
			   		std::cout << "\n Le robot n'a pas atteint son objectif\n";
					message.data = "Le robot n'a pas atteint son objectif";
					client_pub.publish(message);}
			  //	ros::Duration(5).sleep();
				choice_listener.choix = -1;
			  	break;
			
			case 3 :
				// Position proche du dock
				goal.target_pose.pose.position.x = -1.51377045578;
				goal.target_pose.pose.position.y = -5.40542565091;
				goal.target_pose.pose.position.z = 0;
				goal.target_pose.pose.orientation.x = 0;
			  	goal.target_pose.pose.orientation.y = 0;
			  	goal.target_pose.pose.orientation.z = -0.74017298069;
			  	goal.target_pose.pose.orientation.w = 0.672416506829;
		
				std::cout << " \n Objectif envoye\n";
				message.data = "Objectif envoyé, le robot va rejoindre son dock";
				client_pub.publish(message);
				ac.sendGoal(goal);
				ac.waitForResult();

				//Le robot va ensuite tourner sur lui-même jusqu'à voir le code de son dock, puis va rejoindre le dock :
				system("roslaunch turtlebot3_automatic_parking_vision turtlebot3_automatic_parking_vision.launch");
				std::cout << " \n Le robot est à son dock\n";
				message.data = "Le robot est à son dock";
				client_pub.publish(message);
				choice_listener.choix = -1;
				break;
		}
		loop_rate.sleep();

  	}
	return 0;
}
