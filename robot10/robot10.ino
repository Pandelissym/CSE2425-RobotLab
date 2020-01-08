#include <Timer1.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <SoftwareSerial.h>


// USB-config
ros::NodeHandle  nh;

// Start Bluetooth-config
// Comment below if needed to switch to USB-connection
// If uncommented, comment USB-config

//class NewHardware : public ArduinoHardware {
//  public: NewHardware():ArduinoHardware(&Serial1, 57600){};
//}; 
//ros::NodeHandle_<NewHardware> nh;

// End Bluetooth-config


int forwardLeft = 6;
int forwardRight = 2;
int reverseLeft = 7;
int reverseRight = 3;
double time;
int enableLeft = 24;
int enableRight = 25;
int triggerPin = 23;
int echoPin = 22;
boolean stop_moving = false;
boolean isFirst = true;

void messageCb( const geometry_msgs::Twist& msg){
  resetTimer1();
    
  if(msg.linear.x > 0) {
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
    analogWrite(forwardLeft, 255);
    analogWrite(forwardRight, 255);
  } else if(msg.linear.x < 0) {
    analogWrite(forwardLeft, 0);
    analogWrite(forwardRight, 0);
    analogWrite(reverseLeft, 255);
    analogWrite(reverseRight, 255);
  }  else if(msg.angular.z > 0){
    analogWrite(forwardLeft, 62);
    analogWrite(forwardRight, 255);
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
  } else if(msg.angular.z < 0){
    analogWrite(forwardLeft, 255);
    analogWrite(forwardRight, 62);
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
  } else if(msg.linear.x == 0 && msg.angular.z == 0){
    analogWrite(forwardLeft, 0);
    analogWrite(forwardRight, 0);
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
  }
  
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );

void setup()
{ 
  pinMode(forwardLeft, OUTPUT);
  pinMode(forwardRight, OUTPUT);
  pinMode(reverseLeft, OUTPUT);
  pinMode(reverseRight, OUTPUT);
  pinMode(enableLeft, OUTPUT);
  pinMode(enableRight, OUTPUT);
  pinMode(13, OUTPUT);
  
  nh.initNode();
  nh.subscribe(sub);
  
  digitalWrite(enableLeft, HIGH);
  digitalWrite(enableRight, HIGH);
  
  digitalWrite(13, LOW);

  
  startTimer1(10000000L); //Prepare timer1 to check for messages every 10s
  time = millis();
}

void loop()
{    
  nh.spinOnce();
  digitalWrite(triggerPin, LOW);
//    delayMicroseconds(2);
////// Sets the trigPin on HIGH state for 10 micro seconds
//  digitalWrite(triggerPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(triggerPin, LOW);
////// Reads the echoPin, returns the sound wave travel time in microseconds
//  double duration = pulseIn(echoPin, HIGH);
////// Calculating the distance
//  double distance= duration*0.034/2;
//  
//  if (distance >= 5) {
//    digitalWrite(enableLeft, LOW);
//    digitalWrite(enableRight, LOW);
//  }  
}

ISR(timer1Event)
{
    analogWrite(forwardLeft, 0);
    analogWrite(forwardRight, 0);
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
  
  digitalWrite(13, HIGH);
  resetTimer1(); //reset timer1
}

