// Variables :
var twist;
var cmdVel;
var publishImmidiately = true;
var PC_IP;
var manager;
var save_msg;
var save_battery
var teleop;
//PC_IP = "192.168.1.33";
PC_IP = "192.168.43.47";
var ros = new ROSLIB.Ros({
    url: "ws://" + PC_IP + ":9090"
  });

// On commence par bloquer l'affichage de la partie caméra+joystick
document.getElementById('cam').style.display = 'block' ? 'none' : 'block';

// fonction qui va servir pour la publication de la vitesse des roues lors de l'usage du joystick
function moveAction(linear, angular) {
    if (linear !== undefined && angular !== undefined) {
        twist.linear.x = linear;
        twist.angular.z = angular;
    } else {
        twist.linear.x = 0;
        twist.angular.z = 0;
    }
    cmdVel.publish(twist);
}
// On initialise la variable listener qui va aller lire ce que le topic /messages publie
var listener = new ROSLIB.Topic({
    ros : ros,
    name : '/messages',
    messageType : 'std_msgs/String'
  });

// lecture puis affichage du message sur l'écran de l'utilisateur
listener.subscribe(function(message) {
    //console.log('Received message on ' + listener.name + ': ' + message.data);
    save_msg = message.data;
    //console.log('message is : ' + save_msg);
    document.getElementById("Message").innerHTML = save_msg;
});

// initialisation de talker qui va publier dans le topic /choix les comandes envoyée par l'utilisateur codés avec un entier
var talker = new ROSLIB.Topic({
    ros : ros,
    name : '/choix',
    messageType : 'std_msgs/Int32'
  });
// initialisation à -1 (car commande non valide)
var my_message = new ROSLIB.Message({
   data : -1
});

// Bouton salon : qui publie la valeur 1 sur le topic /choix afin d'avertir le robot qu'il doit aller au salon
function Salon() {
    document.getElementById("salon-btn");
    my_message.data=1;
    talker.publish(my_message);
    console.log(my_message.data);
}

function Cuisine() {
    document.getElementById("cuisine-btn");
    my_message.data=2;
    talker.publish(my_message);
    console.log(my_message.data);
}

function Dock() {
    document.getElementById("base-btn");
    my_message.data=3;
    talker.publish(my_message);
    console.log(my_message.data);
}

// fonction qui va publier dans /cmd_vel pour donenr des vitesses linéaire et angulaires 
    function initVelocityPublisher() {
    // Init message with zero values.
    twist = new ROSLIB.Message({
        linear: {
            x: 0,
            y: 0,
            z: 0
        },
        angular: {
            x: 0,
            y: 0,
            z: 0
        }
    });
    // Init topic object
    cmdVel = new ROSLIB.Topic({
        ros: ros,
        name: '/cmd_vel',
        messageType: 'geometry_msgs/Twist'
    });
    // Register publisher within ROS system
    cmdVel.advertise();
}

window.onload = function () {
    // adresse IP du master ROS
    //PC_IP = "192.168.43.47";

    // // Init handle for rosbridge_websocket
    ros = new ROSLIB.Ros({
        url: "ws://" + PC_IP + ":9090"
    });

    initVelocityPublisher();
    // get handle for video placeholder
    video = document.getElementById('video');
    // taille de la fenêtre de la caméra
    video.height = 308;
    video.width = 410;
    video.margin = 1;
    //video.height = 480;
    //video.width = 640;
    //video.margin = 100;
    // Source de la caméra
    video.src = "http://" + PC_IP + ":8080/stream?topic=/raspicam_node/image&type=mjpeg&quality=80";
    video.onload = function () {
        // joystick and keyboard controls will be available only when video is correctly loaded
        //createJoystick();
        //initTeleopKeyboard();
    };
}
/*
function initTeleopKeyboard() {
    // Use w, s, a, d keys to drive your robot

    // Check if keyboard controller was aready created
    if (teleop == null) {
        // Initialize the teleop.
        teleop = new KEYBOARDTELEOP.Teleop({
            ros: ros,
            topic: '/cmd_vel'
        });
    }
}
*/
  
// Récupération du niveau de batterie :
var battery = new ROSLIB.Topic({
    ros : ros,
    name : '/battery_level',
    messageType : 'std_msgs/Int32'
});

battery.subscribe(function(level) {
    // Affichage et mise à jour de la barre de batterie :
    battery_status.style = "width:"+level.data+"%";
    battery_status.innerHTML = level.data + '%';
});

// On affiche tout sauf le joystick et l'info liée au joystick + le bouton :
function Affichage_menu()
{
    document.getElementById("menu_principal");
    // Affichage :
    // main1 correspond au pourcentage de batterie et main2 correspond aux infos+boutons.
    // La caméra se trouve entre les deux
    document.getElementById('main1').style.display = 'none' ? 'block' : 'none';
    document.getElementById('main2').style.display = 'none' ? 'block' : 'none';
    // Bloquage
    // cam correspond au joystick+bouton+info la caméra se trouve avant
    document.getElementById('cam').style.display = 'block' ? 'none' : 'block';
}

    
// Affichage de menu avec caméra + joystick
function Affichage_cam()
{
    // On affiche la partie camera et joystick et on bloque le reste :
    document.getElementById("camera_joystick");
    document.getElementById('cam').style.display = 'none' ? 'block' : 'none';
    document.getElementById('main1').style.display = 'block' ? 'none' : 'block';
    document.getElementById('main2').style.display = 'block' ? 'none' : 'block';

    // Création du joystick
    function createJoystick() {
        // Check if joystick was aready created and if there is enough battery
        if (manager == null) {
            joystickContainer = document.getElementById('joystick');
            // joystck configuration, if you want to adjust joystick, refer to:
            // https://yoannmoinet.github.io/nipplejs/
            var options = {
                zone: joystickContainer,
                position: { left: 50 + '%', top: 60 + 'px' },
                mode: 'static',
                size: 100,
                color: '#0066ff',
                restJoystick: true,            
            };
    
            manager = nipplejs.create(options);
            // event listener for joystick move
            manager.on('move', function (evt, nipple) {
                fadeTime: 0;
                // nipplejs returns direction is screen coordiantes
                // we need to rotate it, that dragging towards screen top will move robot forward
                var direction = nipple.angle.degree - 90;
                if (direction > 180) {
                    direction = -(450- nipple.angle.degree);
                }
                // convert angles to radians and scale linear and angular speed
                // adjust if youwant robot to drvie faster or slower
                var lin = Math.cos(direction / 57.29) * nipple.distance * 0.005;
                var ang = Math.sin(direction / 57.29) * nipple.distance * 0.05;
                // nipplejs is triggering events when joystic moves each pixel
                // we need delay between consecutive messege publications to 
                // prevent system from being flooded by messages
                // events triggered earlier than 50ms after last publication will be dropped 
                if (publishImmidiately) {
                    publishImmidiately = false;
                    moveAction(lin, ang);
                    setTimeout(function () {
                        publishImmidiately = true;
                    }, 50);
                }
            });
            // event litener for joystick release, always send stop message
            manager.on('end', function () {
                moveAction(0, 0);
            });
        }
    }
    createJoystick();
}



