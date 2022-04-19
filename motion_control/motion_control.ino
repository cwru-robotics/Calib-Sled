#include <ros.h>
#include <turtlesim/Spawn.h>
#include <std_msgs/Float32.h>

//175 ticks per mm

ros::NodeHandle nh;
double position_x;
double position_y;
double position_z;


std_msgs::Float32 debug_msg;
ros::Publisher debug("debug", &debug_msg);

void messageCb( const turtlesim::SpawnRequest& req, turtlesim::SpawnResponse& res){
    //debug_msg.data=position_msg.data;
    //debug.publish(&debug_msg);
    //nh.spinOnce();
    double x = req.x;
    double y = req.y;
    double z = req.theta;

    if(x > position_x){//If position requested is > current position
    digitalWrite(13, HIGH - digitalRead(13));
    //Set direction to 0 volts wrt board
    digitalWrite(22, LOW);
    //Find the difference between current position and desired position in mm.
    //Multiply by ticks per mm.
    double rotations = (x - position_x) * 175000.0;
    debug_msg.data = rotations;
    debug.publish(&debug_msg);
    
    //Rotate (up and then down output making a square wave) that many times.
    for(double i = 0.0; i < rotations; i = i + 1.0){
      digitalWrite(27, LOW);
      delay(1);
      digitalWrite(27, HIGH);
      delay(1);
    debug.publish(&debug_msg);
    }
  } else{//If position requested is < current position
    //Set direction to 5 volts wrt board
    digitalWrite(22, HIGH);
    double rotations = (position_x - x) * 175000.0;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    //Rotate (up and then down output making a square wave) that many times.
    for(double i = 0.0; i < rotations; i = i + 1.0){
      digitalWrite(27, LOW);
      delay(1);
      digitalWrite(27, HIGH);
      delay(1);
    }
  }

    
  if(y > position_y){//If position requested is > current position
    digitalWrite(13, HIGH - digitalRead(13));
    //Set direction to 0 volts wrt board
    digitalWrite(10, LOW);
    //Find the difference between current position and desired position in mm.
    //Multiply by ticks per mm.
    double rotations = (y - position_y) * 175000.0;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    
    //Rotate (up and then down output making a square wave) that many times.
    for(double i = 0.0; i < rotations; i = i + 1.0){
      digitalWrite(8, LOW);
      digitalWrite(13, LOW);
      delay(1);
      digitalWrite(8, HIGH);
      digitalWrite(13, HIGH);
      delay(1);
    }
  } else{//If position requested is < current position
    //Set direction to 5 volts wrt board
    digitalWrite(10, HIGH);
    double rotations = (position_y - y) * 175000.0;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    //Rotate (up and then down output making a square wave) that many times.
    for(double i = 0.0; i < rotations; i = i + 1.0){
      digitalWrite(8, LOW);
      digitalWrite(13, LOW);
      delay(1);
      digitalWrite(8, HIGH);
      digitalWrite(13, HIGH);
      delay(1);
    }
  }

    if(z > position_z){//If position requested is > current position
    //Set direction to 0 volts wrt board
    digitalWrite(7, LOW);
    //Find the difference between current position and desired position in mm.
    //Multiply by ticks per mm.
    double rotations = (z - position_z) * 175000.0;
    debug_msg.data = rotations;
    debug.publish(&debug_msg);
    
    //Rotate (up and then down output making a square wave) that many times.
    for(double i = 0.0; i < rotations; i = i + 1.0){
      digitalWrite(5, LOW);
      delay(1);
      digitalWrite(5, HIGH);
      delay(1);
      debug_msg.data = i;
      debug.publish(&debug_msg);
    }
  } else{//If position requested is < current position
    //Set direction to 5 volts wrt board
    digitalWrite(7, HIGH);
    double rotations = (position_z - z) * 175000.0;
    debug_msg.data = rotations;
    debug.publish(&debug_msg);
    //Rotate (up and then down output making a square wave) that many times.
    for(double i = 0.0; i < rotations; i = i + 1.0){
      digitalWrite(5, LOW);
      delay(1);
      digitalWrite(5, HIGH);
      delay(1);
      debug_msg.data = i;
      debug.publish(&debug_msg);
    }
  }

  //Save where we are as the new position.
  position_x = x;
  position_y = y;
  position_z = z;

  res.name = "";
}


ros::ServiceServer<turtlesim::SpawnRequest, turtlesim::SpawnResponse> sub("motion_command", &messageCb );

void setup()
{
  //---Manually make sure that the motor is at full extension at the start (maximum amount of screw rod visible).---
  
  //Set up the three control pins as outputs.
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  //Setup ROS node
  nh.initNode();
  nh.advertiseService(sub);
  nh.advertise(debug);
  //debug_msg.data = 66;
  //debug.publish(&debug_msg);
  nh.spinOnce();
  
  //Set direction to 0 volts wrt board
  digitalWrite(10, LOW);
  digitalWrite(22, LOW);
  digitalWrite(7, LOW);
  //Assume we are at 0 displacement.
  position_x = 0.0;
  position_y = 0.0;
  position_z = 0.0;

}

void loop()
{
  nh.spinOnce();
  delay(1);
}
