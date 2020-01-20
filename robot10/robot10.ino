
#include <Timer1.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <SoftwareSerial.h>


// USB-config
//ros::NodeHandle  nh;


//// Start Bluetooth-config
//// Comment below if needed to switch to USB-connection
//// If uncommented, comment USB-config

class NewHardware : public ArduinoHardware {
  public: NewHardware():ArduinoHardware(&Serial1, 57600){};
}; 
ros::NodeHandle_<NewHardware> nh;

//End Bluetooth-config


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
int distance = 0;

void goForward() {
  analogWrite(reverseLeft, 0);
  analogWrite(reverseRight, 0);
  analogWrite(forwardLeft, 80);
  analogWrite(forwardRight, 80);
}

void goBackward() {
  analogWrite(forwardLeft, 0);
  analogWrite(forwardRight, 0);
  analogWrite(reverseLeft, 80);
  analogWrite(reverseRight, 80);
}

void goLeft() {
  analogWrite(forwardLeft, 30);
  analogWrite(forwardRight, 80);
  analogWrite(reverseLeft, 0);
  analogWrite(reverseRight, 0);
}

void goRight() {
  analogWrite(forwardLeft, 80);
  analogWrite(forwardRight, 30);
  analogWrite(reverseLeft, 0);
  analogWrite(reverseRight, 0);  
}

void stopMoving() {
  analogWrite(forwardLeft, 0);
  analogWrite(forwardRight, 0);
  analogWrite(reverseLeft, 0);
  analogWrite(reverseRight, 0);
}

void messageCb( const geometry_msgs::Twist& msg) {
  resetTimer1();
    
  if(msg.linear.x > 0 && msg.angular.z == 0) {
    goForward();
  } else if(msg.linear.x < 0) {
    goBackward();
  }  else if(msg.angular.z > 0) {
    goLeft();
  } else if(msg.angular.z < 0) {
    goRight();
  } else if(msg.linear.x == 0 && msg.angular.z == 0) {
    stopMoving();
  }
  
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );


void setup() { 
  pinMode(forwardLeft, OUTPUT);
  pinMode(forwardRight, OUTPUT);
  pinMode(reverseLeft, OUTPUT);
  pinMode(reverseRight, OUTPUT);
  pinMode(enableLeft, OUTPUT);
  pinMode(enableRight, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  nh.initNode();
  nh.subscribe(sub);
  
  digitalWrite(enableLeft, HIGH);
  digitalWrite(enableRight, HIGH);
  
  digitalWrite(13, LOW);

  
  startTimer1(10000000L); //Prepare timer1 to check for messages every 10s
  time = millis();
}

void loop() {    
  nh.spinOnce();
  
  digitalWrite(triggerPin, HIGH);
  digitalWrite(triggerPin, LOW);
 
  distance = pulseIn(echoPin, HIGH);
  distance = distance / 2.0 * 0.0343;
   if(distance < 10) {
      stopMoving();
    }
  
    
}

ISR(timer1Event){
  stopMoving();
  
  //digitalWrite(13, HIGH);
  resetTimer1(); //reset timer1
}



