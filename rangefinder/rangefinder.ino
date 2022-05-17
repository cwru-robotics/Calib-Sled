#include <ros.h>
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

int DIR = DIR_X;
int STEP = STEP_X;
int TOP = TOP_X;
int BOTTOM = BOTTOM_X;

ros::NodeHandle nh;

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

void reset(){
  int current_value = LOW;
  if(digitalRead(TOP)){
    digitalWrite(DIR, HIGH);
    while(digitalRead(TOP)){
      current_value = HIGH - current_value;
      digitalWrite(STEP, current_value);
      delayMicroseconds(400);
    }
  }// else{
    delay(1000);
    digitalWrite(DIR, LOW);
    delay(1000);
    while(!digitalRead(TOP)){
      current_value = HIGH - current_value;
      digitalWrite(STEP, current_value);
      delayMicroseconds(400);
    //}
  }
}


std_msgs::UInt32 debug_msg;
ros::Publisher debug("debug", &debug_msg);

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
    current_value = HIGH - current_value;
    digitalWrite(STEP, current_value);
    delayMicroseconds(400);
    c++;
  }

  debug_msg.data = c;
  debug.publish(&debug_msg);
  nh.spinOnce();
}

void loop()
{
  nh.spinOnce();
  delay(1);
}
