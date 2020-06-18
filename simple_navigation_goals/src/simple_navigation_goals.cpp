#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <sensor_msgs/BatteryState.h>

// Ligne qui va servir à communiquer avec /move_base
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// Définition d'une classe qui va permettre de récupérer la tension de la batterie :
class Listener 
{
  	public:
    	double tension;

    	void callback(const sensor_msgs::BatteryStateConstPtr& msg);
};

void Listener::callback(const sensor_msgs::BatteryStateConstPtr& msg) 
{
	tension = msg->voltage;
}

int main(int argc, char **argv) 
{
     
	// Initialisation du noeud : 
	ros::init(argc, argv, "simple_navigation_goals");
    	ros::NodeHandle nh;
	ros::Rate loop_rate(10); 
    	Listener listener;
    	// On indique que l'on va lire les infos du topic battery_state
   	ros::Subscriber sub = nh.subscribe<sensor_msgs::BatteryState>("battery_state", 1, &Listener::callback, &listener);
   	int pos;
           
	MoveBaseClient ac("move_base", true);
	move_base_msgs::MoveBaseGoal goal;
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	//On attend que le serveur action soit lancé
	while(!ac.waitForServer(ros::Duration(5.0))){
		ROS_INFO("\n Attente de l'initialisation du systeme\n");
	}
	ROS_INFO("\n Initialisation du system reussie\n");


	while(true){
		// Création d'un vecteur qui va prendre la liste des noeuds en cours d'utilisation
		std::vector<std::string> V ;
		std::vector<std::string>::iterator it;

		ros::master::getNodes(V);
		bool alive = false;
		it = V.begin();
		// On va regarder si le noeud est encore vivant, si ce n'est pas le cas alors le noeud battery_monitoring l'a tué afin de forcer le robot à retourner au dock
		for (it=V.begin(); it<V.end(); it++){
			if (*it == "/simple_navigation_goals")
				alive = true; 
		}
		// Fonctionnement provisoire car il faut forcément que le turtle soit éteind pour le recharger donc à changer pour le meuble
		if (alive==false)
			std::cout << "\n Le robot va retourner à sa station de rechargement car il manque de batterie, veuillez l'éteindre et le mettre en charge \n";
			ros::Duration(5).sleep();

		// Affichage du menu principal
		system("clear");
		std::cout << "-----------------------MENU---------------------\n\n  Selectionnez un choix puis appuyez sur entree :\n\n 0 : Quitter\n 1 : Envoyer le robot dans la cuisine\n 2 : Envoyer le robot dans le salon\n 3 : Afficher la tension de la batterie\n 4 : Controler le robot avec le clavier\n 5 : Envoyer le robot au dock\n-------------------------------------------------\n\n Votre choix : ";
		std::cin >> pos;
		std::cout << "\n";
		switch (pos){

			// Si on veut quitter :
			case 0: 
				return 0;
			// Position 1 :
			case 1:
				// Coordonnées de la cuisine :
				goal.target_pose.pose.position.x = -5.19920349121;
			  	goal.target_pose.pose.position.y = -3.74059724808;
			  	goal.target_pose.pose.position.z = 0;
			  	goal.target_pose.pose.orientation.x = 0;
			  	goal.target_pose.pose.orientation.y = 0;
			  	goal.target_pose.pose.orientation.z = 0.984546276999;
			  	goal.target_pose.pose.orientation.w = 0.175124608345;
			  	
			  	std::cout << "\n Objectif envoye\n";
			  	// On envoie l'objectif au processus de navigation et on attend le résultat de l'action
			  	ac.sendGoal(goal);
			  	ac.waitForResult();
			  	// On vérifie s'il a bien attend l'objectif voulu ou non :
			  	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
			    		std::cout << "\n Le robot a bien atteint son objectif\n";
			  	else
			   		std::cout << "\n Le robot n'a pas atteint son objectif\n";
			  	ros::Duration(5).sleep();
			  	break;


			case 2:	  
				goal.target_pose.pose.position.x = -3.89844059944;
			  	goal.target_pose.pose.position.y = 0.833602428436;
			  	goal.target_pose.pose.position.z = 0;
			  	goal.target_pose.pose.orientation.x = 0;
			  	goal.target_pose.pose.orientation.y = 0;
			  	goal.target_pose.pose.orientation.z = 0.554674529612;
			  	goal.target_pose.pose.orientation.w = 0.832067404842;
			  	
			  	std::cout << " \n Objectif envoye\n";
			  	ac.sendGoal(goal);
			  	ac.waitForResult();
			  	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
			    		std::cout << " \n Le robot a bien atteint son objectif\n";
			  	else
			    		std::cout << " \n Le robot n'a pas atteint son objectif\n";
			  	ros::Duration(5).sleep();
			  	break;

			case 3 :  
			    	// Affichage de la tension actuelle de la batterie
			    	if (ros::ok()){ 
			    		ros::spinOnce();
		 	    		std::cout << "\n La tension de la batterie est de : " << listener.tension << " V\n";
			    		ros::Duration(5).sleep();
			    		loop_rate.sleep();}
			  	break;

			case 4 :
				//Utilisation de teleop
				system("roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch");
				break;
				
			case 5 :
				// Position proche du dock
				goal.target_pose.pose.position.x = -1.64440190792;
				goal.target_pose.pose.position.y = -0.205766946077;
				goal.target_pose.pose.position.z = 0;
				goal.target_pose.pose.orientation.x = 0;
			  	goal.target_pose.pose.orientation.y = 0;
			  	goal.target_pose.pose.orientation.z = -0.560109994579;
			  	goal.target_pose.pose.orientation.w = 0.828418248213;
		
				std::cout << " \n Objectif envoye\n";
				ac.sendGoal(goal);
				ac.waitForResult();
				if(ac.getState() != actionlib::SimpleClientGoalState::SUCCEEDED){
			    		std::cout << " \n Le robot n'a reussi a atteindre son objectif\n";
					break;}
				//Le robot va ensuite tourner sur lui-même jusqu'à voir le code de son dock, puis va rejoindre le dock :
				system("roslaunch turtlebot3_automatic_parking_vision turtlebot3_automatic_parking_vision.launch");
				std::cout << " \n Le robot est à son dock\n";
				break;
		}

  	}
	return 0;
}
