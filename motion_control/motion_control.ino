#define USE_USBCON
#include <ros.h>
#include <turtlesim/Spawn.h>
#include <std_srvs/Trigger.h>
#include <std_msgs/Float32.h>

#define DIR_X 52
#define DIR_Y 48
#define DIR_Z 44

#define STEP_X 40
#define STEP_Y 36
#define STEP_Z 32

#define BOTTOM_X 11
#define BOTTOM_Y 7
#define BOTTOM_Z 2

#define TOP_X 12
#define TOP_Y 8
#define TOP_Z 3

//X movement range is 234mm - 387mm = 153mm
//Y movement range is 169mm - 322mm = 153mm
//Z movement range is 200mm - 304mm = 104mm

#define TICKS_PER_METER_X 397620
#define TICKS_PER_METER_Y 397598
#define TICKS_PER_METER_Z 1569273

int debounceRead(int CODE){
  int one = digitalRead(CODE);
  delay(1);
  int two = digitalRead(CODE);
  if(one == two){
    return one;
  }
  else{
    delay(1);
    return debounceRead(CODE);
  }
}

ros::NodeHandle nh;
double position_x;
double position_y;
double position_z;

void reset(){
  int current_value = LOW;
  if(digitalRead(TOP_X)){
    digitalWrite(DIR_X, HIGH);
    while(digitalRead(TOP_X)){
      while(digitalRead(TOP_X)){
        nh.spinOnce();
        current_value = HIGH - current_value;
        digitalWrite(STEP_X, current_value);
        delayMicroseconds(400);
      }
      delayMicroseconds(800);
    }
  }
  delay(1000);
  digitalWrite(DIR_X, LOW);
  delay(1000);
  while(!digitalRead(TOP_X)){
    while(!digitalRead(TOP_X)){
      nh.spinOnce();
      current_value = HIGH - current_value;
      digitalWrite(STEP_X, current_value);
      delayMicroseconds(400);
    }
    delayMicroseconds(800);
  }

  if(digitalRead(TOP_Y)){
    digitalWrite(DIR_Y, HIGH);
    int current_value = LOW;
    while(digitalRead(TOP_Y)){
      while(digitalRead(TOP_Y)){
        nh.spinOnce();
        current_value = HIGH - current_value;
        digitalWrite(STEP_Y, current_value);
        delayMicroseconds(400);
      }
      delayMicroseconds(800);
    }
  }
  delay(1000);
  digitalWrite(DIR_Y, LOW);
  delay(1000);
  while(!digitalRead(TOP_Y)){
    while(!digitalRead(TOP_Y)){
      nh.spinOnce();
      current_value = HIGH - current_value;
      digitalWrite(STEP_Y, current_value);
      delayMicroseconds(400);
    }
    delayMicroseconds(800);
  }

  if(digitalRead(TOP_Z)){
    int current_value = LOW;
    digitalWrite(DIR_Z, HIGH);
    while(digitalRead(TOP_Z)){
      while(digitalRead(TOP_Z)){
        nh.spinOnce();
        current_value = HIGH - current_value;
        digitalWrite(STEP_Z, current_value);
        delayMicroseconds(400);
      }
      delayMicroseconds(800);
    }
  }
  delay(1000);
  digitalWrite(DIR_Z, LOW);
  delay(1000);
  while(!digitalRead(TOP_Z)){
    while(!digitalRead(TOP_Z)){
      nh.spinOnce();
      current_value = HIGH - current_value;
      digitalWrite(STEP_Z, current_value);
      delayMicroseconds(400);
    }
    delayMicroseconds(800);
  }
  
  //Establish this as 0 displacement
  position_x = 0.0;
  position_y = 0.0;
  position_z = 0.0;
}


std_msgs::Float32 debug_msg;
ros::Publisher debug("debug", &debug_msg);

void reset_messageCb( const std_srvs::TriggerRequest& req, std_srvs::TriggerResponse& res){
  reset();
  res.success = digitalRead(TOP_Z) && digitalRead(TOP_X) && digitalRead(TOP_Y);
}

void positon_messageCb( const turtlesim::SpawnRequest& req, turtlesim::SpawnResponse& res){
    //debug_msg.data=position_msg.data;
    //debug.publish(&debug_msg);
    //nh.spinOnce();
    double x = req.x;
    double y = req.y;
    double z = req.theta;

    if(x > position_x){//If position requested is > current position
    //digitalWrite(13, HIGH - digitalRead(13));
    //Set direction to 0 volts wrt board
    digitalWrite(DIR_X, LOW);
    //Find the difference between current position and desired position in mm.
    //Multiply by ticks per mm.
    double rotations = (x - position_x) * TICKS_PER_METER_X;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    
    //Rotate (up and then down output making a square wave) that many times.
    int current_value = LOW;
    for(double i = 0.0; i < rotations; i = i + 1.0){
      current_value = HIGH - current_value;
      digitalWrite(STEP_X, current_value);
      delayMicroseconds(400);
      nh.spinOnce();

//      if(!(debounceRead(TOP_X) && debounceRead(BOTTOM_X))){
//          digitalWrite(DIR_X, HIGH);
//          while(!(debounceRead(TOP_X) && debounceRead(BOTTOM_X))){
//            digitalWrite(STEP_X, LOW);
//            delay(1);
//            digitalWrite(STEP_X, HIGH);
//            delay(1);
//          }
//          break;
//          position_x = position_x + i / TICKS_PER_METER_X;
//      }
      //debug.publish(&debug_msg);
    }
  } else{//If position requested is < current position
    //Set direction to 5 volts wrt board
    digitalWrite(DIR_X, HIGH);
    double rotations = (position_x - x) * TICKS_PER_METER_X;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    //Rotate (up and then down output making a square wave) that many times.
    int current_value = LOW;
    for(double i = 0.0; i < rotations; i = i + 1.0){
      current_value = HIGH - current_value;
      digitalWrite(STEP_X, current_value);
      delayMicroseconds(400);
      nh.spinOnce();

//      if(!(debounceRead(TOP_X) && debounceRead(BOTTOM_X))){
//          digitalWrite(DIR_X, LOW);
//          while(!(debounceRead(TOP_X) && debounceRead(BOTTOM_X))){
//            digitalWrite(STEP_X, LOW);
//            delay(1);
//            digitalWrite(STEP_X, HIGH);
//            delay(1);
//          }
//          position_x = position_x - i / TICKS_PER_METER_X;
//          break;
//      }
    }
  }

    
  if(y > position_y){//If position requested is > current position
    //digitalWrite(13, HIGH - digitalRead(13));
    //Set direction to 0 volts wrt board
    digitalWrite(DIR_Y, LOW);
    //Find the difference between current position and desired position in mm.
    //Multiply by ticks per mm.
    double rotations = (y - position_y) * TICKS_PER_METER_Y;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    
    //Rotate (up and then down output making a square wave) that many times.
    int current_value = LOW;
    for(double i = 0.0; i < rotations; i = i + 1.0){
      current_value = HIGH - current_value;
      digitalWrite(STEP_Y, current_value);
      delayMicroseconds(400);
      nh.spinOnce();

//      if(!(debounceRead(TOP_Y) && debounceRead(BOTTOM_Y))){
//          digitalWrite(DIR_Y, HIGH);
//          while(!(debounceRead(TOP_Y) && debounceRead(BOTTOM_Y))){
//            digitalWrite(STEP_Y, LOW);
//            delay(1);
//            digitalWrite(STEP_Y, HIGH);
//            delay(1);
//          }
//          position_y = position_y + i / TICKS_PER_METER_Y;
//          break;
//      }
    }
  } else{//If position requested is < current position
    //Set direction to 5 volts wrt board
    digitalWrite(DIR_Y, HIGH);
    double rotations = (position_y - y) * TICKS_PER_METER_Y;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    //Rotate (up and then down output making a square wave) that many times.
    int current_value = LOW;
    for(double i = 0.0; i < rotations; i = i + 1.0){
      current_value = HIGH - current_value;
      digitalWrite(STEP_Y, current_value);
      delayMicroseconds(400);
      nh.spinOnce();

//      if(!(debounceRead(TOP_Y) && debounceRead(BOTTOM_Y))){
//          digitalWrite(DIR_Y, LOW);
//          while(!(debounceRead(TOP_X) && debounceRead(BOTTOM_Y))){
//            digitalWrite(STEP_Y, LOW);
//            delay(1);
//            digitalWrite(STEP_Y, HIGH);
//            delay(1);
//          }
//          position_y = position_y - i / TICKS_PER_METER_Y;
//          break;
//      }
    }
  }

    if(z > position_z){//If position requested is > current position
    //Set direction to 0 volts wrt board
    digitalWrite(DIR_Z, LOW);
    //Find the difference between current position and desired position in mm.
    //Multiply by ticks per mm.
    double rotations = (z - position_z) * TICKS_PER_METER_Z;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    
    //Rotate (up and then down output making a square wave) that many times.
    int current_value = LOW;
    for(double i = 0.0; i < rotations; i = i + 1.0){
      current_value = HIGH - current_value;
      digitalWrite(STEP_Z, current_value);
      delayMicroseconds(400);
      //debug_msg.data = i;
      //debug.publish(&debug_msg);

//      if(!(debounceRead(TOP_Z) && debounceRead(BOTTOM_Z))){
//          digitalWrite(DIR_Z, HIGH);
//          while(!(debounceRead(TOP_Z) && debounceRead(BOTTOM_Z))){
//            digitalWrite(STEP_Z, LOW);
//            delay(1);
//            digitalWrite(STEP_Z, HIGH);
//            delay(1);
//          }
//          position_z = position_z + i / TICKS_PER_METER_Z;
//          break;
//      }
    }
  } else{//If position requested is < current position
    //Set direction to 5 volts wrt board
    digitalWrite(DIR_Z, HIGH);
    double rotations = (position_z - z) * TICKS_PER_METER_Z;
    //debug_msg.data = rotations;
    //debug.publish(&debug_msg);
    //Rotate (up and then down output making a square wave) that many times.
    int current_value = LOW;
    for(double i = 0.0; i < rotations; i = i + 1.0){
      current_value = HIGH - current_value;
      digitalWrite(STEP_Z, current_value);
      delayMicroseconds(400);
      nh.spinOnce();
      //debug_msg.data = i;
      //debug.publish(&debug_msg);

//      if(!(debounceRead(TOP_Z) && debounceRead(BOTTOM_Z))){
//          digitalWrite(DIR_Z, LOW);
//          while(!(debounceRead(TOP_Z) && debounceRead(BOTTOM_Z))){
//            digitalWrite(STEP_Z, LOW);
//            delay(1);
//            digitalWrite(STEP_Z, HIGH);
//            delay(1);
//          }
//          position_z = position_z - i / TICKS_PER_METER_Z;
//          break;
//      }
    }
  }

  //Save where we are as the new position.
  position_x = x;
  position_y = y;
  position_z = z;

  res.name = "";
}


ros::ServiceServer<turtlesim::SpawnRequest, turtlesim::SpawnResponse> move_sub("motion_command", &positon_messageCb );
ros::ServiceServer<std_srvs::TriggerRequest, std_srvs::TriggerResponse> reset_sub("reset", &reset_messageCb );

void setup()
{
  //Set up the control pins as outputs.
  pinMode(DIR_X, OUTPUT);
  pinMode(DIR_Y, OUTPUT);
  pinMode(DIR_Z, OUTPUT);
  pinMode(STEP_X, OUTPUT);
  pinMode(STEP_Y, OUTPUT);
  pinMode(STEP_Z, OUTPUT);
  //Set up the encoder listen pins as inputs.
  pinMode(TOP_X, INPUT);
  pinMode(TOP_Y, INPUT);
  pinMode(TOP_Z, INPUT);
  pinMode(BOTTOM_X, INPUT);
  pinMode(BOTTOM_Y, INPUT);
  pinMode(BOTTOM_Z, INPUT);

  //Set direction to 0 volts wrt board
  digitalWrite(DIR_X, LOW);
  digitalWrite(DIR_Y, LOW);
  digitalWrite(DIR_Z, LOW);
  
  //Setup ROS node
  nh.initNode();

  //reset();
  
  nh.advertiseService(move_sub);
  nh.advertiseService(reset_sub);
  //nh.advertise(debug);
  //debug_msg.data = 66;
  //debug.publish(&debug_msg);
  nh.spinOnce();
}

void loop()
{
  nh.spinOnce();
  delay(1);
}
