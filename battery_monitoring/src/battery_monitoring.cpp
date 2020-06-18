#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <sensor_msgs/BatteryState.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

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
    	ros::init(argc, argv, "battery_monitoring");
    	ros::NodeHandle nh;
    	ros::Rate loop_rate(10); 
    	Listener listener;
    	ros::Subscriber sub = nh.subscribe<sensor_msgs::BatteryState>("battery_state", 1, &Listener::callback, &listener);
    	MoveBaseClient ac("move_base", true);
     	move_base_msgs::MoveBaseGoal goal;

    	goal.target_pose.header.frame_id = "map";
    	goal.target_pose.header.stamp = ros::Time::now();

    	while(!ac.waitForServer(ros::Duration(5.0))){
    		ROS_INFO("\n Attente de l'initialisation du systeme");
    	}
    	ROS_INFO("\n Initialisation du systeme reussie");

    	while(true){
    		ros::spinOnce();
		// Permet d'attendre que battery_state initialise la valeur correcte de la tension (car au début elle est indiquée à 0V)
    		while (listener.tension < 3)
			loop_rate.sleep();
    		// Si le robot est déchargé (pour le moment valeur de 11.05 arbitraire  car le robot s'éteint à 11) :
    		if (listener.tension < 11.05){
			// On empeche la personne d'envoyer de nouveaux objectifs au robot :
			system("rosnode kill /simple_navigation_goals");
			
			// Position proche du dock
			goal.target_pose.pose.position.x = -1.64440190792;
			goal.target_pose.pose.position.y = -0.205766946077;
			goal.target_pose.pose.position.z = 0;
			goal.target_pose.pose.orientation.x = 0;
		  	goal.target_pose.pose.orientation.y = 0;
		  	goal.target_pose.pose.orientation.z = -0.560109994579;
		  	goal.target_pose.pose.orientation.w = 0.828418248213;	

			ac.sendGoal(goal);
			ROS_INFO("\nLe robot va rejoindre son dock car il n'a plus de batterie");
			ac.waitForResult();

			//Le robot va ensuite tourner sur lui-même jusqu'à voir le code de son dock, puis va rejoindre le dock :
			system("roslaunch turtlebot3_automatic_parking_vision turtlebot3_automatic_parking_vision.launch");
			// Une fois que le robot est à son dock, on ferme tous les terminaux ce qui va quitter ROS
			system("gnome-terminal -x xdotool getactivewindow windowkill");	
			return 0;

       		}
   	}
   
}
