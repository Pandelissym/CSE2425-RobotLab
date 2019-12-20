/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <SoftwareSerial.h>


// USB-config
//ros::NodeHandle  nh;

// Start Bluetooth-config
// Comment below if needed to switch to USB-connection
// If uncommented, comment USB-config

class NewHardware : public ArduinoHardware {
  public: NewHardware():ArduinoHardware(&Serial1, 57600){};
}; 
ros::NodeHandle_<NewHardware> nh;

// End Bluetooth-config


int forwardLeft = 2;
int forwardRight = 6;
int reverseLeft = 7;
int reverseRight = 3;
int enableLeft = 25;
int enableRight = 24;


void messageCb( const geometry_msgs::Twist& msg){
  if(msg.linear.x > 0) {
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
    analogWrite(forwardLeft, 255);
    analogWrite(forwardLeft, 255);
  } else if(msg.linear.x < 0) {
    analogWrite(forwardLeft, 0);
    analogWrite(forwardLeft, 0);
    analogWrite(reverseLeft, 255);
    analogWrite(reverseRight, 255);
  } else {
    analogWrite(forwardLeft, 0);
    analogWrite(forwardLeft, 0);
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
}

void loop()
{  
  nh.spinOnce();
  delay(1);
}

