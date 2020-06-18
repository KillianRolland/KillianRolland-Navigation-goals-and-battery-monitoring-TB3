#KillianRolland-Navigation-goals-and-battery-monitoring-TB3

These files are the files I use with ROS for my project around the TurtleBot3 (Waffle Pi).

battery_monitoring and simple_navigation_goals are the codes I created.
The first is constantly watching the voltage of the battery and when this voltage is too low, the robot goes somewhere where he is supposed to be plugged to its charger. The second one was inspired with this tutorial : http://wiki.ros.org/navigation/Tutorials/SendingSimpleGoals.

It has a Menu that appears in the terminal for the user. You can check the voltage, send the robots to defined positions on the map, ask the robot to go back to his base or control it with the teleop node.

The others files (except Web_App) are the nodes provided by Robotis : https://emanual.robotis.com/docs/en/platform/turtlebot3/overview/ that I use for controling the TurtleBot3.

For these files, I modified the following things :
  - amcl.launch in turtlebot3_navigation for setting the initial pose of the robot (in front of the dock);
  - turtlebot3_teleop_key for having the instructions in french
  - automatic_parking_vision (the node) line 240, with that, the node kill itself after going to the base

Web_App his my current work, I am trying to replace the usage of the terminal by a graphical user interface using a Web application.
For the moment, I followed the next tuto :https://medium.com/husarion-blog/bootstrap-4-ros-creating-a-web-ui-for-your-robot-9a77a8e373f9. With that I can control the TB3 with a Joystick and see the camera view.

The comments of the codes are mostly in french.
