#!/bin/bash

# Lancement du master ROS
gnome-terminal -x roscore &
sleep 5
# les ssh sur la Pi pour la caméra et Lidar+core
#gnome-terminal -x sshpass -p 'turtlebot' ssh pi@192.168.1.41 "roslaunch turtlebot3_bringup turtlebot3_robot.launch; bash;" &
#gnome-terminal -x sshpass -p 'turtlebot' ssh pi@192.168.1.41  "roslaunch turtlebot3_bringup turtlebot3_rpicamera.launch; bash;" &
gnome-terminal -x sshpass -p 'turtlebot' ssh pi@192.168.43.38 "roslaunch turtlebot3_bringup turtlebot3_robot.launch; bash;" &
gnome-terminal -x sshpass -p 'turtlebot' ssh pi@192.168.43.38  "roslaunch turtlebot3_bringup turtlebot3_rpicamera.launch; bash;" &
#gnome-terminal -x sshpass -p 'turtlebot' ssh pi@192.168.43.38  "roslaunch raspicam_node camerav2_410x308_30fps.launch; bash;" &
sleep 20
# on publie l'image_raw de la camera (de base on a seulement la version compressée disponible)
gnome-terminal -x rosrun image_transport republish compressed in:=raspicam_node/image raw out:=raspicam_node/image &
sleep 5
# utilisation de image_proc pour avoir la version rectifiée afin d'utiliser la lecture de marqueurs pour le dock
ROS_NAMESPACE=raspicam_node rosrun image_proc image_proc image_raw:=image _approximate_s=true _queue_size:=20 &
sleep 5
# démarrage de la navigation stack
gnome-terminal -x roslaunch turtlebot3_navigation turtlebot3_navigation.launch map_file:=$HOME/carte.yaml &
# démarage du noeud de gestion de la batterie
gnome-terminal -x rosrun battery_monitoring battery_monitoring &
gnome-terminal -x rosrun simple_navigation_goals simple_navigation_goals &
gnome-terminal -x rosrun web_video_server web_video_server &
gnome-terminal -x roslaunch rosbridge_server rosbridge_websocket.launch &
gnome-terminal --working-directory=Web_App/ -x live-server . --port=5500 --no-browser & # --no-browser
exit 0
