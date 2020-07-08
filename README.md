# Navigation goals and battery monitoring with autonomous TurtleBot3

These files are the files I use with ROS for my project around the TurtleBot3 (Waffle Pi).

battery_monitoring and simple_navigation_goals are the codes I created.
The first is constantly watching the voltage of the battery and when this voltage is too low, the robot goes somewhere where it is supposed to be plugged to its charger. The second one was inspired with this tutorial for sending the robot to specific points : http://wiki.ros.org/navigation/Tutorials/SendingSimpleGoals.


The others files (except Web_App) are the nodes provided by Robotis : https://emanual.robotis.com/docs/en/platform/turtlebot3/overview/ that I use for controling the TurtleBot3.

For these files, I modified the following things :
  - amcl.launch in turtlebot3_navigation for setting the initial pose of the robot (in front of the dock);
  - turtlebot3_teleop_key for having the instructions in french
  - automatic_parking_vision (the node) line 240, with that, the node kill itself after going to the base

The turtle.sh is a bash file that I use for launching all I need (master, nodes on the RaspberryPi via ssh, transforms on the camera, Navigation stack, my nodes and my web app).

Web_App is folder containing a html and a js codes.
I followed the next tuto :https://medium.com/husarion-blog/bootstrap-4-ros-creating-a-web-ui-for-your-robot-9a77a8e373f9. 
It permitted me to put some buttons that can send goals on the TB3's map, have the percentage of battrey, and have a joystick+camera to control the robot remotely.
To use my files, you can refer to the pdf "Document_deploiement_TurtleBot3" (in french) which is basicly all you need to do to make the system working.
