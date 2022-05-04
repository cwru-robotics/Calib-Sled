#include <ros.h>
#include <std_msgs/Int32.h>

#define DIR_X 52
#define DIR_Y 48
#define DIR_Z 44

#define STEP_X 40
#define STEP_Y 36
#define STEP_Z 32

#define BOTTOM_X 28
#define BOTTOM_Y 24
#define BOTTOM_Z 23

#define TOP_X 12
#define TOP_Y 10
#define TOP_Z 8

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
  if(debounceRead(TOP_Z)){
    digitalWrite(DIR_Z, HIGH);
    while(debounceRead(TOP_Z)){
      digitalWrite(STEP_Z, LOW);
      delay(1);
      digitalWrite(STEP_Z, HIGH);
      delay(1);
    }
  }// else{
    digitalWrite(DIR_Z, LOW);
    while(!debounceRead(TOP_Z)){
      digitalWrite(STEP_Z, LOW);
      delay(1);
      digitalWrite(STEP_Z, HIGH);
      delay(1);
    //}
  }
}


std_msgs::Int32 debug_msg;
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

  //Set direction to 0 volts wrt board
  digitalWrite(DIR_Z, LOW);
  
  nh.advertise(debug);
  debug_msg.data = 12345;
  nh.spinOnce();
  debug.publish(&debug_msg);
  nh.spinOnce();

  int c = 0;
  while(debounceRead(BOTTOM_Z)){
    digitalWrite(STEP_Z, LOW);
    delay(1);
    digitalWrite(STEP_Z, HIGH);
    delay(1);
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
