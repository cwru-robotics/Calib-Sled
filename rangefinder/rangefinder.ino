#define USE_USBCON
#include <ros.h>
#include <turtlesim/Spawn.h>
#include <std_srvs/Trigger.h>
#include <std_msgs/Float32.h>
#include <std_msgs/UInt32.h>

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

int DIR = DIR_Z;
int STEP = STEP_Z;
int TOP = TOP_Z;
int BOTTOM = BOTTOM_Z;

ros::NodeHandle nh;

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
}


std_msgs::UInt32 debug_msg;
ros::Publisher debug("debug", &debug_msg);

void reset_messageCb( const std_srvs::TriggerRequest& req, std_srvs::TriggerResponse& res){
  reset();
  res.success = digitalRead(TOP_Z) && digitalRead(TOP_X) && digitalRead(TOP_Y);
}

void doYourThangCallback(const turtlesim::SpawnRequest& req, turtlesim::SpawnResponse& res){
  reset();

  delay(1000);

  //Set direction to 0 volts wrt board
  digitalWrite(DIR, LOW);
  delay(1000);
  
  nh.advertise(debug);
  debug_msg.data = 12345;
  nh.spinOnce();
  debug.publish(&debug_msg);
  nh.spinOnce();

  long unsigned int c = 0;
  int current_value = LOW;
  while(digitalRead(BOTTOM)){
    while(digitalRead(BOTTOM)){
      current_value = HIGH - current_value;
      digitalWrite(STEP, current_value);
      delayMicroseconds(600);
      c++;
      nh.spinOnce();
    }
    delayMicroseconds(800);
  }

  char buf[256];
  itoa(c, buf, 10);
  res.name = buf;
  debug.publish(&debug_msg);
}

ros::ServiceServer<turtlesim::SpawnRequest, turtlesim::SpawnResponse> move_sub("evaluate", &doYourThangCallback );
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
  
  //Setup ROS node
  nh.initNode();

  nh.advertiseService(move_sub);
  nh.advertiseService(reset_sub);

  nh.spinOnce();
}

void loop()
{
  nh.spinOnce();
  delay(1);
}
